#include "mcp2515.h"

#include <stdint.h>

#include "avr/io.h"
#include "spi.h"
#include "utility.h"

#define MCP_F_FREQ 16000000UL  // 16MHz

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

  _delay_ms(3);

  uint8_t value = mcp2515_read(MCP_CANSTAT);
  if ((value & MODE_MASK) != MODE_CONFIG) {
    LOG_ERR("MCP2515 is NOT in configuration mode after reset!\n");
    return 1;
  }
  //  We are setting a 1 Mbps CAN speed with standard timing:
  //   - BRP = 7
  //   - PHASE1 = 6
  //   - PHASE2 = 6
  //   - PROP = 3
  //   - SJW = 1
  //   - SMP = 1
  uint8_t BRP = 0x07;  // BRP = 7
  uint8_t SJW = 0x01;  // SJW = 1
  uint8_t cnf1_mode = (SJW << 6) | (BRP);
  mcp2515_write(MCP_CNF1, cnf1_mode);

  uint8_t BTLMODE = 0x01;  // BTLMODE = 1 (enables programmable PHASE2)
  uint8_t SAM = 0x01;      // Single sample mode = 1
  uint8_t PHSEG1 = 0x06;   // PHASE1 = 6 TQ
  uint8_t PROPSEG = 0x03;  // PROPSEG = 3 TQ
  uint8_t cnf2_mode = (BTLMODE << 7) | (SAM << 6) | (PHSEG1 << 3) | (PROPSEG);
  mcp2515_write(MCP_CNF2, cnf2_mode);

  uint8_t SOF = 0x00;     // Disable Start-of-Frame
  uint8_t WAKFIL = 0x00;  // Disable wake-up filter
  uint8_t PHSEG2 = 0x06;  // PHASE2 = 6 TQ
  uint8_t cnf3_mode = (SOF << 7) | (WAKFIL << 6) | (PHSEG2);
  mcp2515_write(MCP_CNF3, cnf3_mode);  // SJW = 2, BRP = 0
  // mcp2515_write(MCP_CNF1, SJW1);
  // // Prop = 2, PS1 = 8
  // mcp2515_write(MCP_CNF2, 0xB9);
  // // PS2 = 4
  // mcp2515_write(MCP_CNF3, 0x03);

  // Set mask & filter to accept Node2 messages
  // Node1 ID: 0x200 = 0100 0000 000
  // Node2 ID: 0x400 = 1000 0000 000
  // receive all messages
  // mcp2515_write(MCP_RXB0CTRL, 0x60);
  // ACCEPT ALL
  mcp2515_write(MCP_RXB0CTRL, 0x60);  // RXM1:RXM0 = 11
  mcp2515_write(MCP_RXB1CTRL, 0x60);  // same for RXB1

  // mcp2515_write(MCP_RXB0CTRL, 0x40 | 0x20);
  mcp2515_bitmodify(MCP_CANINTE, MCP_RX1IF | MCP_RX0IF, 0xFF);

  SREG |= (1 << ATMEGA_GLOBAL_INTERUPT);  // Enable extern interupt
  MCUCR |= (1 << ISC01);
  MCUCR &= ~(1 << ISC00);
  GICR |= (1 << INT0);
  DDRD &= ~(1 << PD2);

  mcp2515_setmode(MODE_NORMAL);

  _delay_ms(10);

  uint8_t val = mcp2515_read(MCP_CANSTAT);

  if ((val & MODE_MASK) != MODE_NORMAL) {
    LOG_ERR("MCP2515 is NOT in NORMAL mode after reset!\n");
    return 1;
  }

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
void mcp2515_setmode(uint8_t mode) { mcp2515_bitmodify(MCP_CANCTRL, 0b11100000, mode); }
