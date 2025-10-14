#include <avr/io.h>
typedef enum {
  CAN_ERROR_NONE = 0,
  CAN_ERROR_MSG_NULL,
} can_error_t;

typedef struct {
  uint16_t id;
  int data_length;
  uint8_t data[8];
} can_message_t;

can_error_t can_transmit(can_message_t *message);
can_message_t can_receive();
