#include "mcp2515.h"

#include <stdint.h>

#include "avr/io.h"
#include "spi.h"
#include "utility.h"

static spi_device_handle_t spi_mcp2515_dev = {
    .ss_port = &PORTB,
    .ss_pin = MCP_SS_PIN,
};

void mcp2515_reset() {
  spi_slave_select(&spi_mcp2515_dev);

  spi_transmit(MCP_RESET);

  spi_slave_deselect(&spi_mcp2515_dev);
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

  // mcp2515 interupt
  mcp2515_bitmodify(MCP_CANINTE, 0x01, MCP_RX0IE);
  mcp2515_bitmodify(MCP_CANINTF, 0x01, MCP_RX0IF);

  // Atmega interupt
  SREG |= (1 << ATMEGA_GLOBAL_INTERUPT);  // Enable extern interupt
  MCUCR |= (1 << ISC01) | (1 << ISC00);   // Falling edge set intrupt
  GICR |= (1 << INT0);                    // Setup intrupt on INT0_vector
  DDRD |= (1 << PD2);
  return 0;
}

uint8_t mcp2515_read(uint8_t address) {
  uint8_t data;
  spi_slave_select(&spi_mcp2515_dev);

  spi_transmit(MCP_READ);
  spi_transmit(address);

  data = spi_receive();

  spi_slave_deselect(&spi_mcp2515_dev);
  return data;
}

void mcp2515_write(uint8_t address, uint8_t data) {
  spi_slave_select(&spi_mcp2515_dev);

  spi_transmit(MCP_WRITE);
  spi_transmit(address);
  spi_transmit(data);

  spi_slave_deselect(&spi_mcp2515_dev);
}

uint8_t mcp2515_read_status(uint8_t reg) {
  uint8_t data;
  spi_slave_select(&spi_mcp2515_dev);

  spi_transmit(MCP_READ_STATUS);
  spi_transmit(reg);
  data = spi_receive();

  spi_slave_deselect(&spi_mcp2515_dev);
  return data;
}

void mcp2515_RTS(int buffer) {
  spi_slave_select(&spi_mcp2515_dev);

  buffer = buffer % 3;
  uint8_t data = MCP_RTS_TX0;
  if (buffer == 0) {
    data = MCP_RTS_TX0;
  } else if (buffer == 1) {
    data = MCP_RTS_TX1;
  } else if (buffer == 2) {
    data = MCP_RTS_TX2;
  }
  spi_transmit(data);

  spi_slave_deselect(&spi_mcp2515_dev);
}

void mcp2515_bitmodify(uint8_t address, uint8_t mask, uint8_t data) {
  spi_slave_select(&spi_mcp2515_dev);
  spi_transmit(MCP_BITMOD);
  spi_transmit(address);  // Register to be edited
  spi_transmit(mask);     // 1 byte: bit=1: edit allowed
  spi_transmit(data);     // New bit values to be set
  spi_slave_deselect(&spi_mcp2515_dev);
}

// modes: MODE_NORMAL, MODE_LOOPBACK, MODE_CONFIG
void mcp2515_setmode(uint8_t mode) { mcp2515_bitmodify(MCP_CANCTRL, 0x70, mode); }
