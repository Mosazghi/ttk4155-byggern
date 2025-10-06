#include "mcp2515.h"

typedef struct  {
    uint16_t id;
    char data_length;
    char data[8];
} can_message_t;

void can_transmit(can_message_t *message);

