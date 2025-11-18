#include "input.h"

#include <math.h>

#include "adc.h"
#include "utility.h"

void joystick_read(joystick_xy_t* joystick) {
  joystick->x = adc_read(ADC_CH1);
  joystick->y = adc_read(ADC_CH0);
  LOG_DBG("Raw joystick values: X = %d, Y = %d", joystick->x, joystick->y);
}

joystick_dir_t joystick_get_dir(joystick_xy_t* joystick, uint8_t threshold) {
  // joystick_dir_t dir = NEUTRAL;
  if (joystick->x > threshold) {
    LOG_DBG("Joystick direction: RIGHT\n");
    return RIGHT;
  } else if (joystick->x < -threshold) {
    LOG_DBG("Joystick direction: LEFT\n");
    return LEFT;
  } else if (joystick->y > threshold) {
    LOG_DBG("Joystick direction: UP\n");
    return UP;
  } else if (joystick->y < -threshold) {
    LOG_DBG("Joystick direction: DOWN\n");
    return DOWN;
  } else {
    LOG_DBG("Joystick direction: NEUTRAL\n");
    return NEUTRAL;
  }
}
