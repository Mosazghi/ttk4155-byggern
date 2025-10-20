#include "can.h"

#include "mcp2515.h"
#include "utility.h"

can_error_t can_transmit(can_message_t *message) {
  if (message == NULL) return CAN_ERROR_MSG_NULL;

  mcp2515_write(MCP_TXB0SIDH, (message->id >> 3) & 0xFF);
  mcp2515_write(MCP_TXB0SIDL, (message->id & 0x07) << 5);

  mcp2515_write(MCP_TXB0DLC, message->data_length & 0x0F);

  for (int i = 0; i < message->data_length; i++) {
    mcp2515_write(MCP_TXB0D0 + i, message->data[i]);
  }

  mcp2515_RTS(0);

  return CAN_ERROR_NONE;
}

can_message_t can_receive() {
  can_message_t message;
  uint8_t id_h = mcp2515_read(MCP_RXB0SIDH);
  uint8_t id_l = mcp2515_read(MCP_RXB0SIDL);

  message.id = (id_h << 3) | ((id_l >> 5));

  message.data_length = mcp2515_read(MCP_RXB0DLC);

  for (int i = 0; i < message.data_length; i++) {
    message.data[i] = mcp2515_read(MCP_RXB0D0 + i);
  }

  // Clear RX0IF flag
  mcp2515_bitmodify(MCP_CANINTF, MCP_RX0IF, 0x00);
  mcp2515_bitmodify(MCP_CANINTF, MCP_RX1IF, 0x00);
  return message;
}
