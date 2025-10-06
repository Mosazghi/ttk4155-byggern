#include "spi.h"

// clang-format off
#include "utility.h"
#include <util/delay.h>


void spi_init(spi_config_t *handle) {
  // Set MOSI, SCK, !SS as Output
  DDRB |= (1 << PB5) | (1 << PB7);
  DDRB &= ~(1 << PB6);  // MISO as Input
  // Enable SPI, Set as Master, set clock rate fck/16
  SPCR |= (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}

void spi_transfer(spi_device_handle_t *dev, spi_transfer_t *transfer) {
  spi_slave_select(dev);

  const uint8_t *tx_buf = NULL;
  if (transfer->tx_buf != NULL) {
    tx_buf = (const uint8_t *)transfer->tx_buf;
  }

  for (size_t i = 0; i < transfer->len; i++) {
    uint8_t data = 0xFF;
    if (tx_buf != NULL) {
      data = tx_buf[i];
    }
    // LOG_INF("Transferring byte: 0x%02X\n", data);
    SPDR = data;
    while (!(SPSR & (1 << SPIF)));
    if (transfer->rx_buf != NULL) {
      // wINF("Received byte: 0x%02X\n--------\n", SPDR);
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
