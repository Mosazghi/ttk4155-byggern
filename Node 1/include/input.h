#pragma once
#include <stdbool.h>
#include <stdint.h>

typedef struct {
  int x;
  int y;
  bool btn;
} joystick_xy_t;

typedef enum { NEUTRAL, LEFT, RIGHT, UP, DOWN } joystick_dir_t;

typedef struct {
  int16_t x;
  int16_t y;
} touch_pad_xy_t;

/**
 * @brief Maps raw joystick values to calibrated range
 * @param joystick Pointer to the joystick structure to map
 */
void map_joystick(joystick_xy_t *joystick);
/**
 * @brief Reads the current joystick position and button state
 * @param joystick Pointer to the joystick structure to populate
 */
void joystick_read(joystick_xy_t *joystick);
/**
 * @brief Reads the current touch pad position
 * @return The current touch pad coordinates
 */
touch_pad_xy_t touch_pad_read();
/**
 * @brief Determines the joystick direction based on the current position and a threshold
 * @param joystick Pointer to the joystick structure
 * @param threshold The threshold value to determine direction
 * @return The joystick direction
 */
joystick_dir_t joystick_get_dir(joystick_xy_t *joystick, uint8_t threshold);
