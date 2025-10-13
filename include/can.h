#include "mcp2515.h"

typedef struct {
  uint16_t id;
  int data_length;
  uint8_t data[8];
} can_message_t;

void can_transmit(can_message_t *message);
can_message_t can_receive();
