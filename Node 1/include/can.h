#pragma once
#include <avr/io.h>

/**
 * @brief CAN error codes
 * TODO: Add more.
 */
typedef enum {
  CAN_ERROR_NONE = 0,
  CAN_ERROR_MSG_NULL,
} can_error_t;

/**
 * @brief CAN message structure
 */
typedef struct {
  unsigned int id;
  uint8_t data[8];
  int data_length;
} can_message_t;

/**
 * @brief Transmit a CAN message
 * @param message Pointer to the CAN message to transmit
 * @return CAN error code indicating success or failure
 */
can_error_t can_transmit(can_message_t *message);
/**
 * @brief Receive a CAN message
 * @return The received CAN message
 */
can_message_t can_receive();
