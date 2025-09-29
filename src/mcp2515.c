#include "mcp2515.h"

#include <stdint.h>

#include "spi.h"

void mcp2515_init() {}

uint8_t mcp2515_read(uint8_t address) {
  spi_select(SPI_MCP2515);
  spi_transmit(MCP_READ);
  spi_transmit(address);
  uint8_t data = spi_receive();
  spi_deselect(SPI_MCP2515);
  return data;

  void mcp2515_write(uint8_t address, uint8_t data) {}
