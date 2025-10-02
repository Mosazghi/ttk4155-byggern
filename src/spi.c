#include "spi.h"

#include <util/delay.h>

#include "utility.h"

void spi_init() {
  // Set MOSI, SCK, !SS as Output
  DDRB = (1 << PB5) | (1 << PB7) | (1 << PB4) | (1 << AVR_SS_PIN) | (1 << DISPLAY_SS_PIN) |
         (1 << MCP_SS_PIN);
  DDRB &= ~(1 << PB6);  // MISO as Input
  // Enable SPI, Set as Master, set clock rate fck/16
  SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}

void spi_transmit(uint8_t data) {
  // Start transmission
  SPDR = data;
  // Wait for transmission to complete
  while (!(SPSR & (1 << SPIF)));
}

uint8_t spi_receive() {
  SPDR = 0xFF;  // Send dummy byte to initiate SPI read
  // Wait for transmission to complete
  while (!(SPSR & (1 << SPIF)));
  return SPDR;  // Return received data
}
// data: input data. Ex.: [0x01, 0x02, 0x03, ...]
void spi_transmit_packet(const uint8_t *data, int length) {
  for (int i = 0; i < length; i++) {
    LOG_DBG("Writing command: %2X (SPI)", data[i]);
    spi_transmit(data[i]);
  }
}

void spi_slave_select(spi_slave_t device) {
  if (device == SPI_OLED) {
    PORTB &= ~(1 << DISPLAY_SS_PIN);
  } else if (device == SPI_MCP2515) {
    PORTD &= ~(1 << MCP_SS_PIN);
  } else {
    PORTB &= ~(1 << AVR_SS_PIN);
  }
}

void spi_slave_deselect() {
  PORTB |= (1 << DISPLAY_SS_PIN) | (1 << AVR_SS_PIN);
  PORTD |= (1 << MCP_SS_PIN);
}
