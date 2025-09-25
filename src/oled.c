#include "fonts.h"
#include <oled.h>

typedef enum { CMD, DATA } oled_write_mode_t;

const uint8_t a[] = {0b01111100, 0b01111110, 0b00010011, 0b00010011,
                     0b01111110, 0b01111100, 0b00000000, 0b00000000};
const uint8_t a_normal[] = {0b01111110, 0b00010001, 0b00010001, 0b00010001,
                            0b01111110};
const uint8_t a_small[] = {0b01111000, 0b00010100, 0b01111000, 0b00000000};
const uint8_t s[] = {0b00100110, 0b01100111, 0b01001101, 0b01011001,
                     0b01111011, 0b00110010, 0b00000000, 0b00000000};

static void oled_write(uint8_t data, oled_write_mode_t type) {
  spi_slave_select(OLED);
  PIN_WRITE(PORTB, OLED_CMD, type == CMD ? LOW : HIGH);
  LOG_DBG("Transmitting from oled_write: %#x", data);
  spi_transmit(data);
}

static void oled_write_data_packet(const uint8_t *data, int size) {
  spi_slave_select(OLED);
  PIN_WRITE(PORTB, OLED_CMD, HIGH);
  spi_transmit_packet(data, size);
}

int oled_init(void) {
  DDRB |= (1 << OLED_CMD);
  spi_slave_select(OLED); // Display CS
  PORTB &= ~(1 << OLED_CMD); // OLED cmd display PB2
  uint8_t oled_init_array[] = {
      0xAE, // display off
      0xA8, // multiplex ratio mode: 63
      0x3F, //
      0xD5, // dispay division ratio / osc. freq. mode
      0x80,
      0xd3, // display offset 
      0x00, 
      0x40, // display start line 0 
      0x20, // set addressing mode
      0x02, // enable page mode
      0xA1, // segment remap (SEG0=COM127)
      0xC8, // common output scan direction: com63-com0
      0xDA, // common pads hardware: alternative
      0x12, //
      0x81, // contrast control
      0xFF, //
      0xDB, // VCOM deselect level mode
      0x34, //
      0xA4, // out follows RAM content
      0xA6, // set normal display
      0xAF  // display on
  };

  spi_transmit_packet(oled_init_array, ARRAY_LENGTH(oled_init_array));
  PORTB |= (1 << OLED_CMD); // Display cmd
  spi_slave_deselect();
                       // 000100 (1 << PB4)
                       //
                       //  000100 (1 << PB4)
                       //| 001000 (PORTB)
                       //  001000
  oled_clear();

  return 0; // Return 0 on success
}

void oled_clear(void) {
  for (int i = 0; i < 8; i++) {
    oled_write(0xB0 + i, CMD); // Set page adress to page0
    oled_write(0x00, CMD);     // set oled lower column line
    oled_write(0x10, CMD);     // Set oled higher column line
    for (int j = 0; j < 128; j++) {
      oled_write(0x00, DATA); // writing blank to each page
    }
  }
}

void oled_print() {
  oled_write(0xB0, CMD); // Set page adress to page0
  oled_go_to_column(0);
  oled_write_data_packet(a, ARRAY_LENGTH(a));
  oled_write(0xB3, CMD); // Set page adress to page0
  oled_go_to_column(26);
  oled_write_data_packet(a_normal, ARRAY_LENGTH(a_normal));
  oled_write(0xB6, CMD); // Set page adress to page0
  oled_go_to_column(64);
  oled_write_data_packet(a_normal, ARRAY_LENGTH(a_normal));
}

void oled_go_to_column(int column) {
  oled_write(0x00 + (column % 16), CMD);
  oled_write(0x10 + (column / 16), CMD);
}
