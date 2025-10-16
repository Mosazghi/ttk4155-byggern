#include "spi.h"

// clang-format off
#include "utility.h"
#include <util/delay.h>
#include <stddef.h>
#define SPI_DUMMY_BYTE 0xFF


void spi_init(spi_config_t *handle) {
  // Set MOSI, SCK, !SS as Output
  DDRB |= (1 << handle->mosi_pin) | (1 << handle->sck_pin);
  DDRB &= ~(1 << handle->miso_pin);  // MISO as Input
  // Enable SPI, Set as Master, set clock rate fck/16
  SPCR |= (1 << SPE) | (1 << MSTR) | (1 << SPR0);
  // SPCR |= (handle->clock_div << SPR0);
}


void spi_transmit(uint8_t data) {
    SPDR = data; 
    while (!(SPSR & (1 << SPIF))); 
}

uint8_t spi_receive(void) {
  SPDR = SPI_DUMMY_BYTE; 
  while (!(SPSR & (1 << SPIF))); 
  return SPDR;
}

void spi_slave_select(spi_device_handle_t *dev) { 
  *(dev->ss_port) &= ~(1 << dev->ss_pin); 
}

void spi_slave_deselect(spi_device_handle_t *dev) { 
  *(dev->ss_port) |= (1 << dev->ss_pin); 
}
