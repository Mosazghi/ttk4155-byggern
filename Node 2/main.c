#include <stdarg.h>
#include <stdio.h>
#include <sys/types.h>

#include "can.h"
#include "pwm.h"
#include "sam.h"
#include "servo.h"
#include "time.h"
#include "uart.h"
#define BAUDRATE 115200
#define F_CPU 84000000

can_init_t can_config = {
    .brp = 84,  // Baud Rate Prescaling (83)
    .phase1 = 6,
    .phase2 = 6,
    .propag = 3,
    .sjw = 1,
    .smp = 1,
    .interrupt = false,
};

int main() {
  SystemInit();

  WDT->WDT_MR = WDT_MR_WDDIS;  // Disable Watchdog Timer
  uart_init(F_CPU, BAUDRATE);
  can_init(can_config);
  can_msg_t msg = {0};
  input_t input = {0};
  printf("version 5\n\r");

  piob_output_init(13);
  pwm_init(PWM_CH1, PB13, 50);

  while (1) {
    if (can_rx(&msg)) {
      can_parse_input_msg(&msg, &input);
      int x_us = pos_to_us(input.joystick.x);
      printf("X: %d -> %d us\n\r", input.joystick.x, x_us);
      pwm_set_pulseWidth(PWM_CH1, x_us, 50);
    }
    time_spinFor(msecs(10));
  }
}
