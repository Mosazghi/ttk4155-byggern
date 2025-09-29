#include <math.h>
#include <oled.h>
#include <string.h>

#include "fonts.h"
#include "spi.h"
#include "sram.h"
#include "utility.h"

static oled_ctx ctx = {0};

static void oled_write(uint8_t data, oled_write_mode_t type) {
  PIN_WRITE(PORTB, OLED_CS, LOW);
  PIN_WRITE(PORTB, OLED_CMD, type == CMD ? LOW : HIGH);
  spi_transmit(data);
}

static void oled_write_sram(uint16_t addr, uint8_t data) {
  if (addr < ADDR_START && addr > ADDR_END) {
    LOG_ERR("Outside of display sram scope (oled_write_sram)");
  }
  sram_write(addr, data);
}

static void oled_write_data_packet(const uint8_t* data, int size) {
  PIN_WRITE(PORTB, OLED_CS, LOW);
  PIN_WRITE(PORTB, OLED_CMD, HIGH);
  spi_transmit_packet(data, size);
}

static void oled_write_data_packet_sram(uint16_t addr, const uint8_t* data, size_t size) {
  // PIN_WRITE(PORTB, OLED_CS, LOW);
  // PIN_WRITE(PORTB, OLED_CMD, HIGH);
  sram_transmit_packet(addr, data, size);
  //   // 0x1400-0x1800 (1kb-2kb or 1kb space)
}

static void set_cursor(int x, int y) {
  /* Outside scope cases */
  x = CLAMP(x, 0, SEG_WIDTH);
  y = CLAMP(y, 0, PAGE_HEIGHT);

  oled_write(0xB0 + y, CMD);
  oled_go_to_column(x);
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
  int addr = ADDR_START;
  for (int i = 0; i < 8; i++) {
    set_cursor(0, i);
    for (int j = 0; j < 128; j++) {
      oled_write(0x00, DATA);         // writing blank to each page
      oled_write_sram(addr++, 0x00);  // writing blank in sram
    }
  }
  LOG_INF("finished clearing sram at %#X", addr);
}

void oled_go_to_column(int column) {
  oled_write(0x00 + (column % 16), CMD);
  oled_write(0x10 + (column / 16), CMD);
}

void oled_set_font(oled_font_size_t font_size) { ctx.font_size = font_size; }

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

static uint16_t cursor_to_addr(int x, int y) { return ADDR_START + (uint16_t)(128 * y + x); }

void oled_printf(const char* str, int x, int y) {
  size_t size = strlen(str);
  uint8_t msg[size][(int)ctx.font_size];
  int pos = 0;

  /* Convert str to font */
  for (size_t i = 0; i < size; i++) {
    pos = (str[i] - '0') + 16;
    store_letter(msg[i], pos);
  }

  LOG_INF("cursor address is starting at: %#X", cursor_to_addr(x, y));

  /* Display words */
  for (size_t i = 0; i < size; i++) {
    oled_write_data_packet_sram(cursor_to_addr(x, y) + ((int)ctx.font_size * i), msg[i],
                                (int)ctx.font_size);
    // oled_write_data_packet_sram(ADDR_START + ((int)ctx.font_size * i), msg[i],
    // (int)ctx.font_size);
  }
}

void oled_display() {
  uint8_t data = 0;
  uint8_t prev_data = 0;
  uint16_t x = ADDR_START;

  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 128; j++) {
      uint16_t temp = (ADDR_START + j) + (0x80 * i);
      data = sram_read(temp);
      if (data != 0x00) {
        // If the prev. data is 0 AND we have valid data, then we have to calculate x-pos again.
        if (prev_data == 0x00) {
          x = (temp - ADDR_START) - (128 * i);
          set_cursor(x, i);
        }
        oled_write(data, DATA);
      }
      prev_data = data;
    }
  }
}

void oled_draw_pixel(int x, int y) {
  uint8_t point = 0;
  uint16_t addr = ADDR_START;

  /* Limit */
  x = CLAMP(x, 0, OLED_WIDTH - 1);
  y = CLAMP(y, 0, OLED_HEIGHT - 1);

  /* Calculate pixel position */
  point = (1 << (uint8_t)(y % 8));
  addr = ADDR_START + x + (y / 8 * 128);

  oled_write_data_packet_sram(addr, &point, sizeof(point));
}

void oled_draw_line(int x_start, int y_start, int x_end, int y_end) {
  /* Limit */
  x_start = CLAMP(x_start, 0, OLED_WIDTH);
  y_start = CLAMP(y_start, 0, OLED_HEIGHT);
  x_end = CLAMP(x_end, 0, OLED_WIDTH);
  y_end = CLAMP(y_end, 0, OLED_HEIGHT);

  /* Calculate line using Bresenham's algorithm */
  int dx = ABS(x_end - x_start);
  int sx = x_start < x_end ? 1 : -1;
  int dy = -ABS(y_end - y_start);
  int sy = y_start < y_end ? 1 : -1;
  int error = dx + dy;
  int e2 = 0;

  while (1) {
    oled_draw_pixel(x_start, y_start);
    e2 = 2 * error;
    LOG_INF("x: %d, y: %d", x_start, y_start);

    if (x_start == x_end && y_start == y_end) break;

    LOG_INF("e2: %d, dy: %d, dx: %d", e2, dy, dx);
    if (e2 >= dy) {
      if (x_start == x_end) break;
      error += dy;
      x_start += sx;
    }
    if (e2 <= dx) {
      if (y_start == y_end) break;
      error += dx;
      y_start += sy;
    }
  }
}