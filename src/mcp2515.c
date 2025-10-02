#include "mcp2515.h"

#include <stdint.h>

#include "spi.h"

void mcp2515_init() {}

uint8_t mcp2515_read(uint8_t address) {
  spi_slave_select(SPI_MCP2515);
  spi_transmit(MCP_READ);
  spi_transmit(address);
  uint8_t data = spi_receive();
  spi_slave_deselect();
  return data;
}

void mcp2515_write(uint8_t address, uint8_t data) {
  spi_slave_select(SPI_MCP2515);
  spi_transmit(MCP_WRITE);
  spi_transmit(address);
  spi_transmit(data);
  spi_slave_deselect();
}
