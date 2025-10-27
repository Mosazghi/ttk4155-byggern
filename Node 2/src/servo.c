#include "servo.h"

#include "pwm.h"

long map(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// Set pin n on port p as output
void piob_output_init(int pin) {
  // Enable PIOB timer
  PMC->PMC_PCER0 = (1 << ID_PIOB);

  // Enable pin control by PIO
  PIOB->PIO_PER = (1 << pin);

  // Set pin as output
  PIOB->PIO_OER = (1 << pin);
}

void piob_set_pin_low(int pin) { PIOB->PIO_CODR = (1 << pin); }

void piob_set_pin_high(int pin) { PIOB->PIO_SODR = (1 << pin); }

// Converts joystick value to pulsewidth in us
int pos_to_us(int joystick_value) {
  joystick_value = CLAMP(joystick_value, joystick_min, joystick_max);
  int us = SERVO_CENTERED + (joystick_value * 5);  // [1000,2000]us
  return us;
}

// void joystick_move_servo(joystick_xy_t *joystick) {
//     int x = pos_to_us(joystick->x);
//     pwm_set_pulseWidth(PWM_CH1, x, 50);
// }
