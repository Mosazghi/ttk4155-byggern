#pragma once

#define AVR_SS_PIN PB4
#include <stdint.h>

#include "input.h"

/**
 * @brief Button from the IO board.
 */
typedef struct __attribute__((packed)) {
  union {
    uint8_t right;
    struct {
      uint8_t R1 : 1;
      uint8_t R2 : 1;
      uint8_t R3 : 1;
      uint8_t R4 : 1;
      uint8_t R5 : 1;
      uint8_t R6 : 1;
    };
  };
  union {
    uint8_t left;
    struct {
      uint8_t L1 : 1;
      uint8_t L2 : 1;
      uint8_t L3 : 1;
      uint8_t L4 : 1;
      uint8_t L5 : 1;
      uint8_t L6 : 1;
      uint8_t L7 : 1;
    };
  };
  union {
    uint8_t nav;
    struct {
      uint8_t NB : 1;
      uint8_t NR : 1;
      uint8_t ND : 1;
      uint8_t NL : 1;
      uint8_t NU : 1;
    };
  };
} buttons_t;

/**
 * @brief Touch pad from the IO board.
 */
typedef struct __attribute__((packed)) {
  uint8_t x;
  uint8_t y;
  uint8_t size;
} touch_pad_t;

/**
 * @brief Touch slider from the IO board.
 */
typedef struct __attribute__((packed)) {
  uint8_t x;
  uint8_t y;
  uint8_t size;
} touch_slider_t;

/**
 * @brief Initializes the AVR IO Board.
 */
void avr_init();
/**
 * @brief Gets the current joystick position.
 * @return The current joystick position.
 */
joystick_xy_t avr_get_joystick();
/**
 * @brief Gets the current touch pad position.
 * @return The current touch pad position.
 */
touch_pad_t avr_get_touch_pad();
/**
 * @brief Gets the current touch slider position.
 * @return The current touch slider position.
 */
touch_slider_t avr_get_touch_slider();
/**
 * @brief Gets the current button states.
 * @return The current button states.
 */
buttons_t avr_get_buttons();
/**
 * @brief Initializes the timer to trigger at 10Hz.
 */
void avr_timer_init_10hz();
