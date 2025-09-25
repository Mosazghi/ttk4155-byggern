#include "input.h"

#include <math.h>

#include "adc.h"
#include "utility.h"

ADC adc = {0};
static long map(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
void map_joystick(joystick_xy_t *joystick) {
  LOG_DBG("Mapping joystick values: X = %d, Y = %d", joystick->x, joystick->y);
  if (joystick == NULL) {
    return;  // Error: Null pointer
  }
  // joystick->x = (int)((joystick->x * (uint16_t) 200) / 241) - 100;
  // joystick->y =   (int)((joystick->y * (uint16_t) 200) / 241) - 100;
  joystick->x = map(joystick->x, 0, 241, -100, 100);
  joystick->y = map(joystick->y, 0, 241, -100, 100);
}

void joystick_read(joystick_xy_t *joystick) {
  joystick->x = adc_read(ADC_CH1);
  joystick->y = adc_read(ADC_CH0);
  LOG_DBG("Raw joystick values: X = %d, Y = %d", joystick->x, joystick->y);
  // joystick.btn = !((PINF >> PINF5) & 0x01); // Active low
}

joystick_dir_t joystick_get_dir(joystick_xy_t joystick, uint8_t threshold) {
  // joystick_dir_t dir = NEUTRAL;
  if (joystick.x > threshold) {
    LOG_DBG("Joystick direction: RIGHT\n");
    return RIGHT;
  } else if (joystick.x < -threshold) {
    LOG_DBG("Joystick direction: LEFT\n");
    return LEFT;
  } else if (joystick.y > threshold) {
    LOG_DBG("Joystick direction: UP\n");
    return UP;
  } else if (joystick.y < -threshold) {
    LOG_DBG("Joystick direction: DOWN\n");
    return DOWN;
  } else {
    LOG_DBG("Joystick direction: NEUTRAL\n");
    return NEUTRAL;
  }
}

// Adjusts the value to be 0 if within the deadzone threshold
static uint8_t deadzone_adjust(joystick_xy_t *joystick, uint8_t threshold) {
  if (joystick == NULL) {
    return 1;  // Error: Null pointer
  }

  if (sqrt(joystick->x * joystick->x) < threshold) {
    joystick->x = 0;
  }

  if (sqrt(joystick->y * joystick->y) < threshold) {
    joystick->y = 0;
  }

  return 0;
}

// Reads all inputs and returns an ADC struct with the values
ADC adc_read_all() {
  joystick_read(&adc.joystick);
  deadzone_adjust(&adc.joystick, 15);
  map_joystick(&adc.joystick);
  joystick_get_dir(adc.joystick, 50);
  // adc.touch_pad = touch_pad_read();
  return adc;
}

touch_pad_xy_t touch_pad_read() {
  touch_pad_xy_t touch_pad;
  touch_pad.x = adc_read(ADC_CH2);
  touch_pad.y = adc_read(ADC_CH3);

  return touch_pad;
}
