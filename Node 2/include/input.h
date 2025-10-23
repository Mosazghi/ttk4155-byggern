#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef struct {
  int8_t x;
  int8_t y;
  bool btn;
} joystick_xy_t;

typedef struct __attribute__((packed)) {
  uint8_t x;
  uint8_t y;
  uint8_t size;
} touch_pad_t;

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
} button_t;

typedef struct {
  joystick_xy_t joystick;
  button_t buttons;
  touch_pad_t touch_pad;
} input_t;
