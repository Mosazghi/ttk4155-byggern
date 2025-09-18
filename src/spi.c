#include "spi.h"

void spi_init() {
  // Set MOSI, SCK, !SS as Output
  DDRB = (1 << PB5) | (1 << PB7) | (1 << PB4) | (1 << AVR_SS_PIN) |
         (1 << DISPLAY_SS_PIN);
  DDRB &= ~(1 << PB6); // MISO as Input
  // Enable SPI, Set as Master, set clock rate fck/16
  SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}

void spi_transmit(uint8_t data) {
  // Start transmission
  SPDR = data;
  // Wait for transmission to complete
  while (!(SPSR & (1 << SPIF)))
    ;
}

uint8_t spi_recveive() {
  SPDR = 0xFF; // Send dummy byte to initiate SPI read
  // Wait for transmission to complete
  while (!(SPSR & (1 << SPIF)))
    ;
  return SPDR; // Return received data
}