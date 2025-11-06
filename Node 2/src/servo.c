#include "servo.h"

#include "pwm.h"

#include "dsp.h"
// Set pin n on port p as output
void piob_output_init(int pin) {
  // Enable PIOB timer
  PMC->PMC_PCER0 |= (1 << ID_PIOB);

  // Enable pin control by PIO
  PIOB->PIO_PER |= (1 << pin);

  // Set pin as output
  PIOB->PIO_OER |= (1 << pin);
}

void piob_set_pin_low(int pin) { PIOB->PIO_CODR |= (1 << pin); }

void piob_set_pin_high(int pin) { PIOB->PIO_SODR |= (1 << pin); }

// Converts joystick value to pulsewidth in us
int pos_to_us(int joystick_value) {
     const int DEADZONE = 5;
    if (abs(joystick_value) < DEADZONE) joystick_value = 0;
  joystick_value = CLAMP(joystick_value, joystick_min, joystick_max);
  int us = SERVO_CENTERED + (joystick_value * 5);  // [1000,2000]us
  return us;
}

// void joystick_move_servo(joystick_xy_t *joystick) {
//     int x = pos_to_us(joystick->x);
//     pwm_set_pulseWidth(PWM_CH1, x, 50);
// }

int remap(uint8_t value) {
  int remap = map(value, 0, 255, -100, 100);
  return remap;
}
