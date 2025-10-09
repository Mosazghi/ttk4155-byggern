#include "can.h"

void can_transmit(can_message_t *message) {
    mcp2515_write(MCP_TXB0SIDH, message->id >> 8);
    mcp2515_write(MCP_TXB0SIDL, message->id << 5);
    mcp2515_write(MCP_TXB0DLC, message->data_length);
    
    for (int i = 0; i < message->data_length; i++) {
        mcp2515_write(MCP_TXB0D0+i, message->data[i]);
    }
}

can_message_t can_receive() {
    can_message_t message;
    int id_h, id_l, i;
    id_h = mcp2515_read(MCP_RXB0SIDH);
    id_l = mcp2515_read(MCP_RXB0SIDL);

    message.id = ((id_h << 3) | (id_l >> 5));

    message.data_length = mcp2515_read(MCP_RXB0DLC);

    for (i = 0; i < message.data_length; i++) {
        message.data[i] = mcp2515_read(MCP_RXB0D0+i);
    }
    return message;
}