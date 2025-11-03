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
joystick_dir_t joystick_get_dir(joystick_xy_t *joystick, uint8_t threshold);
