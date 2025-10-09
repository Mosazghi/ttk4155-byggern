#include "spi.h"

// clang-format off
#include "utility.h"
#include <util/delay.h>


void spi_init(spi_config_t *handle) {
  // Set MOSI, SCK, !SS as Output
  DDRB |= (1 << handle->mosi_pin_num) | (1 << handle->sck_pin_num);
  DDRB &= ~(1 << handle->miso_pin_num);  // MISO as Input
  // Enable SPI, Set as Master, set clock rate fck/16
  SPCR |= (1 << SPE) | (1 << MSTR) | (1 << SPR0);
  // SPCR |= (handle->clock_div << SPR0);
}

void spi_transfer(spi_device_handle_t *dev, spi_transfer_t *transfer) {
  spi_slave_select(dev);

  const uint8_t *tx_buf = NULL;
  if (transfer->tx_buf != NULL) {
    tx_buf = (const uint8_t *)transfer->tx_buf;
  }

  for (int i = 0; i < transfer->len; i++) {
    uint8_t data = 0xFF;
    if (tx_buf != NULL) {
      data = tx_buf[i];
    }
    SPDR = data;
    while (!(SPSR & (1 << SPIF)));
    // skip the first return byte
    if (transfer->rx_buf != NULL && i != 0) {
      transfer->rx_buf[i] = SPDR;
    }
  }

  spi_slave_deselect(dev);
}

void spi_slave_select(spi_device_handle_t *dev) { 
  *(dev->ss_port_num) &= ~(1 << dev->ss_pin_num); 
}

void spi_slave_deselect(spi_device_handle_t *dev) { 
  *(dev->ss_port_num) |= (1 << dev->ss_pin_num); 
}
