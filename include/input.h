#pragma once
#include <stdbool.h>
#include <stdint.h>

// #include "adc.h"
typedef struct {
  int x;
  int y;
  bool btn;
} joystick_xy_t;

typedef enum { LEFT, RIGHT, UP, DOWN, NEUTRAL } joystick_dir_t;

typedef struct {
  int16_t x;
  int16_t y;
} touch_pad_xy_t;

// typedef struct ADC
// {
//     struct joystick;
//     struct touch_pad;
// };
// typedef struct ADC {
//   joystick_xy_t joystick;
//   touch_pad_xy_t touch_pad;
// } ADC;

void map_joystick(joystick_xy_t *joystick);
// ADC adc_read_all();
void joystick_read(joystick_xy_t *joystick);
touch_pad_xy_t touch_pad_read();
