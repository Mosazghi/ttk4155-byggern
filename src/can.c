#include "can.h"

void can_transmit(can_message_t *message) {
    mcp2515_write(MCP_TXB0SIDH, message->id >> 8);
    mcp2515_write(MCP_TXB0SIDL, message->id << 5);
    mcp2515_write(MCP_TXB0DLC, message->data_length);
    
    for (int i = 0; i < message->data_length; i++) {
        mcp2515_write(MCP_TXB0D0+i, message->data[i]);
    }
}