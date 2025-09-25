#include <oled.h>

#include "fonts.h"

static oled_ctx ctx = {0};

static void oled_write(uint8_t data, oled_write_mode_t type) {
  PIN_WRITE(PORTB, OLED_CS, LOW);
  PIN_WRITE(PORTB, OLED_CMD, type == CMD ? LOW : HIGH);
  spi_transmit(data);
}

static void oled_write_data_packet(const uint8_t* data, int size) {
  PIN_WRITE(PORTB, OLED_CS, LOW);
  PIN_WRITE(PORTB, OLED_CMD, HIGH);
  spi_transmit_packet(data, size);
}

int oled_init(void) {
  ctx.font_size = SMALL;
  DDRB |= (1 << PB2);
  PORTB &= ~(1 << PB3);  // Display CS
  PORTB &= ~(1 << PB2);  // OLED cmd display
  uint8_t oled_init_array[] = {
      0xAE,  // display off
      0xA8,  // multiplex ratio mode: 63
      0x3F,  //
      0xD5,  // dispay division ratio / osc. freq. mode
      0x80,
      0xd3,  // display offset
      0x00,
      0x40,  // display start line 0
      0x20,  // set addressing mode
      0x02,  // enable page mode
      0xA1,  // segment remap (SEG0=COM127)
      0xC8,  // common output scan direction: com63-com0
      0xDA,  // common pads hardware: alternative
      0x12,  //
      0x81,  // contrast control
      0xFF,  //
      0xDB,  // VCOM deselect level mode
      0x34,  //
      0xA4,  // out follows RAM content
      0xA6,  // set normal display
      0xAF   // display on
  };

  spi_transmit_packet(oled_init_array, ARRAY_LENGTH(oled_init_array));
  PORTB |= (1 << PB2);  // Display cmd
  PORTB |= (1 << PB3);  // Display CS
                        // 000100 (1 << PB4)
                        //
                        //  000100 (1 << PB4)
                        //| 001000 (PORTB)
                        //  001000
  oled_clear();

  return 0;  // Return 0 on success
}

void oled_clear(void) {
  for (int i = 0; i < 8; i++) {
    oled_write(0xB0 + i, CMD);  // Set page adress to page0
    oled_write(0x00, CMD);      // set oled lower column line
    oled_write(0x10, CMD);      // Set oled higher column line
    for (int j = 0; j < 128; j++) {
      oled_write(0x00, DATA);  // writing blank to each page
    }
  }
}

void oled_go_to_column(int column) {
  oled_write(0x00 + (column % 16), CMD);
  oled_write(0x10 + (column / 16), CMD);
}

void oled_set_font(oled_font_size_t font_size) { ctx.font_size = font_size; }

void oled_set_cursor(int x, int y) {
  /* Outside scope cases */
  if (x > PAGE_HEIGHT) {
    x = PAGE_HEIGHT;
  }
  if (y > SEG_WIDTH) {
    y = SEG_WIDTH;
  }
  if (y < 0) {
    y = 0;
  }
  if (x < 0) {
    x = 0;
  }

  oled_write(0xB0 + x, CMD);
  oled_go_to_column(y);
}

static void store_letter(uint8_t* buffer, int pos) {
  switch (ctx.font_size) {
    case SMALL:
      for (int i = 0; i < (int)SMALL; i++) buffer[i] = pgm_read_byte(&font4[pos][i]);
      break;
    case MEDIUM:
      for (int i = 0; i < (int)MEDIUM; i++) buffer[i] = pgm_read_byte(&font5[pos][i]);
      break;
    case LARGE:
      for (int i = 0; i < (int)LARGE; i++) buffer[i] = pgm_read_byte(&font8[pos][i]);
      break;
    default:
      for (int i = 0; i < (int)SMALL; i++) buffer[i] = pgm_read_byte(&font4[pos][i]);
      break;
  }
}

void oled_printf(const char* str) {
  size_t size = strlen(str);
  uint8_t msg[size][(int)ctx.font_size];
  int pos = 0;

  /* Convert str to font */
  for (size_t i = 0; i < size; i++) {
    pos = (str[i] - '0') + 16;
    store_letter(msg[i], pos);
  }

  /* Display words */
  for (size_t i = 0; i < size; i++) {
    oled_write_data_packet(msg[i], ARRAY_LENGTH(msg[i]));
  }
}