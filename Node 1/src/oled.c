#include <avr/interrupt.h>
#include <oled.h>
#include <string.h>

#include "avr.h"
#include "fonts.h"
#include "spi.h"
#include "sram.h"
#include "utility.h"

static void oled_init_timer_30hz();
void set_cursor(int x, int y);
static oled_ctx_t ctx = {0};

static spi_device_handle_t dev = {
    .ss_port = &PORTB,
    .ss_pin = OLED_CS,
};
/****************************************************************
 *                      STATIC FUNCTIONS                       *
 ****************************************************************/

static void oled_write(uint8_t data, oled_write_mode_t type) {
  PIN_WRITE(PORTB, OLED_CMD, type == OLED_CMD_M ? LOW : HIGH);

  spi_slave_select(&dev);

  spi_transmit(data);

  spi_slave_deselect(&dev);
}

static void oled_write_array(uint8_t* data, size_t len, oled_write_mode_t type) {
  PIN_WRITE(PORTB, OLED_CMD, type == OLED_CMD_M ? LOW : HIGH);

  spi_slave_select(&dev);

  while (len--) {
    spi_transmit(*data++);
  }

  spi_slave_deselect(&dev);
}

void oled_write_sram(uint16_t addr, uint8_t data) {
  if (addr < ADDR_START && addr > ADDR_END) {
    LOG_ERR("Outside of display sram scope (oled_write_sram)");
  }
  sram_write(addr, data);
}

void oled_clear_raw() {
  for (int page = 0; page < 8; page++) {
    set_cursor(page, 0);
    for (int col = 0; col < 128; col++) {
      oled_write(0xFF, OLED_DATA_M);
    }
  }
}

static void oled_go_to_column(int column) {
  oled_write(0x00 + (column % 16), OLED_CMD_M);
  oled_write(0x10 + (column / 16), OLED_CMD_M);
}

void set_cursor(int x, int y) {
  /* Outside scope cases */
  x = CLAMP(x, 0, SEG_WIDTH);
  y = CLAMP(y, 0, PAGE_HEIGHT);

  oled_write(0xB0 + y, OLED_CMD_M);
  oled_go_to_column(x);
}

static void store_letter(uint8_t* buffer, int pos) {
  switch (ctx.font_size) {
    case FONT_S:
      for (int i = 0; i < (int)FONT_S; i++) buffer[i] = pgm_read_byte(&font4[pos][i]);
      break;
    case FONT_M:
      for (int i = 0; i < (int)FONT_M; i++) buffer[i] = pgm_read_byte(&font5[pos][i]);
      break;
    case FONT_L:
      for (int i = 0; i < (int)FONT_L; i++) buffer[i] = pgm_read_byte(&font8[pos][i]);
      break;
    default:
      for (int i = 0; i < (int)FONT_S; i++) buffer[i] = pgm_read_byte(&font4[pos][i]);
      break;
  }
}

static uint16_t cursor_to_addr(int x, int y) { return ADDR_START + (uint16_t)(OLED_WIDTH * y + x); }

static void oled_init_timer_30hz() {
  cli();
  // CTC mode (TOP = OCR1A), Normal port operation, clk/1024 prescaler
  TCCR0 = (1 << WGM12) | (1 << CS02) | (1 << CS00);

  // Enable Output Compare Match interrupt
  TIMSK |= (1 << OCIE0);

  /*
    use the following: (f_CPU / prescaler * target_freq) - 1
    (4915200 / 1024 * 30) - 1 = 159
  */
  OCR0 = 159;
  sei();
}

/****************************************************************
 *                      GLOABL FUNCTIONS                       *
 ****************************************************************/

void oled_init(void) {
  ctx.font_size = FONT_S;
  DDRB |= (1 << PB2);
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
  oled_write_array(oled_init_array, ARR_LEN(oled_init_array), OLED_CMD_M);
  PORTB |= (1 << PB2);  // Display cmd
  oled_clear();
  oled_init_timer_30hz();
}

void oled_clear(void) {
  int addr = ADDR_START;
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 128; j++) {
      oled_write_sram(addr++, 0x00);  // writing blank in sram
    }
  }
}

inline void oled_set_font(oled_font_size_t font_size) { ctx.font_size = font_size; }

void oled_printf(const char* str, int x, int y) {
  const size_t size = strlen(str);
  uint8_t msg[size][(int)ctx.font_size];
  int pos = 0;

  /* Convert str to font */
  for (size_t i = 0; i < size; i++) {
    pos = (str[i] - '0') + 16;
    store_letter(msg[i], pos);
  }

  x = CLAMP(x, 0, OLED_WIDTH - 1);
  y = CLAMP(y, 0, PAGE_HEIGHT);

  /* Display words */
  for (size_t i = 0; i < size; i++) {
    sram_transmit_packet(cursor_to_addr(x, y) + ((int)ctx.font_size * i), msg[i],
                         (int)ctx.font_size);
  }
}

void oled_display() {  // TODO: LOGIC ERROR
  uint8_t data = 0;
  for (int i = 0; i < 8; i++) {
    set_cursor(0, i);  // NOT NEEDED TO FIND X
    for (int j = 0; j < 128; j++) {
      uint16_t temp = (ADDR_START + j) + (OLED_WIDTH * i);
      data = sram_read(temp);
      oled_write(data, OLED_DATA_M);
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
  addr = ADDR_START + x + (y / 8 * OLED_WIDTH);
  point = sram_read(addr);
  point |= (1 << (y % 8));  // perseve data if already on

  sram_transmit_packet(addr, &point, sizeof(uint8_t));
}

void oled_draw_line(int x_start, int y_start, int x_end, int y_end) {
  /* Limit */
  x_start = CLAMP(x_start, 0, OLED_WIDTH - 1);
  y_start = CLAMP(y_start, 0, OLED_HEIGHT - 1);
  x_end = CLAMP(x_end, 0, OLED_WIDTH - 1);
  y_end = CLAMP(y_end, 0, OLED_HEIGHT - 1);

  /* Calculate line using Bresenham's algorithm */
  const int dx = ABS(x_end - x_start);
  const int sx = x_start < x_end ? 1 : -1;
  const int dy = -ABS(y_end - y_start);
  const int sy = y_start < y_end ? 1 : -1;
  int error = dx + dy;
  int e2 = 0;

  while (1) {
    oled_draw_pixel(x_start, y_start);
    e2 = 2 * error;

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
