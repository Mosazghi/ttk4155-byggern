#include "mcp2515.h"

#include <stdint.h>

#include "spi.h"

void mcp2515_init() {
  spi_init();
  _delay_ms(1);
  spi_slave_select(SPI_MCP2515);
  spi_transmit(MCP_RESET);
  spi_slave_deselect();
  _delay_ms(1);
}

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

char mcp2515_read_status(char data) {
  spi_slave_select(SPI_MCP2515);
  spi_transmit(MCP_READ_STATUS);
  char data = spi_receive();
  spi_slave_deselect();
  return data;
}

// Request to send
void mcp2515_RTS(int buffer){       // Buffers 0-2
  spi_slave_select(SPI_MCP2515);
  buffer = buffer % 3;
  char data = MCP_RTS_TX0;
  if (buffer == 0){
    data = MCP_RTS_TX0;
  }
  else if (buffer == 1){
    data = MCP_RTS_TX1;
  }
  else if (buffer == 2){
    data = MCP_RTS_TX2;
  }
  spi_transmit(data);
  spi_slave_deselect();
}

void mcp2515_bitmod(uint8_t address, uint8_t mask, uint8_t data) {
  spi_slave_select(SPI_MCP2515);
  spi_transmit(MCP_BITMOD);
  spi_transmit(address);            // Register to be edited
  spi_transmit(mask);               // 1 byte: bit=1: edit allowed
  spi_transmit(data);               // New bit values to be set
  spi_slave_deselect();
}

// modes: MODE_NORMAL, MODE_LOOPBACK
void mcp2515_setmode(uint8_t mode){
  mcp2515_bitmod(MCP_CANCTRL, 0x70, mode);
}