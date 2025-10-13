#include "can.h"

#include "utility.h"

void can_transmit(can_message_t *message) {
  mcp2515_write(MCP_TXB0SIDH, (message->id >> 3));           // 8 MSB
  mcp2515_write(MCP_TXB0SIDL, ((message->id & 0x07) << 5));  // Get 3 LSB then left shift 5 places
  mcp2515_write(MCP_TXB0DLC, message->data_length);

  for (int i = 0; i < message->data_length; i++) {
    mcp2515_write(MCP_TXB0D0 + i, message->data[i]);
  }
  mcp2515_RTS(0);  // Request to send on buffer 0
}

can_message_t can_receive() {
  while (!(mcp2515_read(MCP_CANINTF) & MCP_RX0IF)) {
    LOG_INF("No CAN msg received. Waiting...\n");
    // return (can_message_t){0};  // Return an empty message if no message is received
  }

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

  return message;
}
