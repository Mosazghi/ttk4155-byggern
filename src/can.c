#include "can.h"

#include "mcp2515.h"
#include "utility.h"

can_error_t can_transmit(can_message_t *message) {
  if (message == NULL) return CAN_ERROR_MSG_NULL;

  // TODO: verify these are corerct way of doing this
  mcp2515_write(MCP_TXB0SIDH, (uint8_t)(message->id >> 3));  // 8 MSB
  mcp2515_write(MCP_TXB0SIDL, ((message->id & 0x07) << 5));  // Get 3 LSB then left shift 5 places
  mcp2515_write(MCP_TXB0DLC, message->data_length);

  for (int i = 0; i < message->data_length; i++) {
    mcp2515_write(MCP_TXB0D0 + i, message->data[i]);
  }
  mcp2515_RTS(0);  // Request to send on buffer 0
  return CAN_ERROR_NONE;
}

can_message_t can_receive() {
  can_message_t message;
  uint8_t id_h, id_l;
  id_h = mcp2515_read(MCP_RXB0SIDH);
  id_l = mcp2515_read(MCP_RXB0SIDL);

  message.id = (id_h << 3) | (id_l >> 5);  // Corrected ID extraction

  message.data_length = mcp2515_read(MCP_RXB0DLC);
  LOG_INF("LFG!! Received CAN message with ID: %#x, Data Length: %d\n", message.id,
          message.data_length);
  for (int i = 0; i < message.data_length; i++) {
    message.data[i] = mcp2515_read(MCP_RXB0D0 + i);
  }
  mcp2515_bitmodify(MCP_CANINTF, 0x01, MCP_RX0IF);
  return message;
}
