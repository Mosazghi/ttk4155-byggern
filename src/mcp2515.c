#include "mcp2515.h"

#include <stdint.h>

#include "spi.h"
#include "utility.h"
static spi_device_handle_t SPI_MCP2515 = {
    .ss_port_num = &PORTB,
    .ss_pin_num = MCP_SS_PIN,
};

void mcp2515_reset() {
  uint8_t tx_buf[] = {MCP_RESET};
  spi_transfer_t transfer = {
      .tx_buf = tx_buf,
      .rx_buf = NULL,
      .len = 1,
  };

  spi_transfer(&SPI_MCP2515, &transfer);
}

uint8_t mcp2515_init() {
  LOG_INF("Initializing MCP2515...\n");
  mcp2515_reset();
  _delay_ms(1);
  // mcp2515_setmode(MODE_CONFIG);  // Enable config mode
  uint8_t value = mcp2515_read(MCP_CANSTAT);
  if ((value & MODE_MASK) != MODE_CONFIG) {
    LOG_ERR("MCP2515 is NOT in configuration mode after reset!\n");
    return 1;
  }

  // TODO: TIMER settings

  // Enable interrupt
  // mcp2515_write(MCP_CANINTE, MCP_RX0IE);

  // Set mask & filter to accept Node2 messages
  // Node1 ID: 0x200 = 0100 0000 000
  // Node2 ID: 0x400 = 1000 0000 000

  // Set filter/mask
  // mcp2515_write(MCP_RXF0SIDH, 0x80);  // Set filter = 1000 0000
  // mcp2515_write(MCP_RXM0SIDH, 0x80);  // Set mask = 1000 0000

  mcp2515_setmode(MODE_LOOPBACK);  // Disable config mode
  _delay_ms(1);
  return 0;
}

uint8_t mcp2515_read(uint8_t address) {
  // spi_slave_select(SPI_MCP2515);
  uint8_t data[3];
  spi_transfer_t transfer = {
      .tx_buf = (uint8_t[]){MCP_READ, address, 0xFF},
      .rx_buf = data,
      .len = 3,
  };

  // spi_transmit(MCP_READ);
  // spi_transmit(address);
  spi_transfer(&SPI_MCP2515, &transfer);
  // uint8_t data = spi_receive();
  // spi_slave_deselect();
  return data[2];
}

void mcp2515_write(uint8_t address, uint8_t data) {
  // spi_slave_select(SPI_MCP2515);
  // spi_transmit(MCP_WRITE);
  // spi_transmit(address);
  // spi_transmit(data);
  spi_transfer_t transfer = {
      .tx_buf = (uint8_t[]){MCP_WRITE, address, data},
      .rx_buf = 0,
      .len = 3,
  };
  spi_transfer(&SPI_MCP2515, &transfer);
  // spi_slave_deselect();
}

uint8_t mcp2515_read_status(uint8_t data) {
  uint8_t rx_buf[2];
  // spi_slave_select(SPI_MCP2515);
  // spi_transmit(MCP_READ_STATUS);
  // char data = spi_receive();
  // spi_slave_deselect();
  spi_transfer_t transfer = {
      .tx_buf = (uint8_t[]){MCP_READ_STATUS, data},
      .rx_buf = rx_buf,
      .len = 2,
  };
  spi_transfer(&SPI_MCP2515, &transfer);
  return rx_buf[1];
}

// Request to send
void mcp2515_RTS(int buffer) {  // Buffers 0-2
  // spi_slave_select(SPI_MCP2515);
  buffer = buffer % 3;
  uint8_t data[1] = {MCP_RTS_TX0};
  if (buffer == 0) {
    data[0] = MCP_RTS_TX0;
  } else if (buffer == 1) {
    data[0] = MCP_RTS_TX1;
  } else if (buffer == 2) {
    data[0] = MCP_RTS_TX2;
  }
  // spi_transmit(data);
  // spi_slave_deselect();
  spi_transfer_t transfer = {
      .tx_buf = data,
      .rx_buf = 0,
      .len = 1,
  };
  spi_transfer(&SPI_MCP2515, &transfer);
}

void mcp2515_bitmodify(uint8_t address, uint8_t mask, uint8_t data) {
  // spi_slave_select(SPI_MCP2515);
  // spi_transmit(MCP_BITMOD);
  // spi_transmit(address);  // Register to be edited
  // spi_transmit(mask);     // 1 byte: bit=1: edit allowed
  // spi_transmit(data);     // New bit values to be set
  // spi_slave_deselect();
  spi_transfer_t transfer = {
      .tx_buf = (uint8_t[]){MCP_BITMOD, address, mask, data},
      .rx_buf = 0,
      .len = 4,
  };
  spi_transfer(&SPI_MCP2515, &transfer);
}

// modes: MODE_NORMAL, MODE_LOOPBACK, MODE_CONFIG
void mcp2515_setmode(uint8_t mode) { mcp2515_bitmodify(MCP_CANCTRL, 0x70, mode); }
