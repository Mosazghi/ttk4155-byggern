#pragma once

#include <stdint.h>

#include "input.h"

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

typedef struct __attribute__((packed)) {
  uint8_t x;
  uint8_t y;
  uint8_t size;
} touch_pad_t;

typedef struct __attribute__((packed)) {
  uint8_t x;
  uint8_t y;
  uint8_t size;
} touch_slider_t;

joystick_xy_t avr_get_joystick();
touch_pad_t avr_get_touch_pad();
touch_slider_t avr_get_touch_slider();
buttons_t avr_get_buttons();
