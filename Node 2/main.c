#include "sam.h"
#define F_CPU 84000000
#include <stdarg.h>
#include <stdio.h>
#include "servo.h"
#include "pwm.h"

/*
 * Remember to update the Makefile with the (relative) path to the uart.c file.
 * This starter code will not compile until the UART file has been included in
 the Makefile.
 * If you get somewhat cryptic errors referencing functions such as _sbrk,
 * _close_r, _write_r, _fstat etc, you have most likely not done that correctly.

 * If you get errors such as "arm-none-eabi-gcc: no such file", you may need to
 reinstall the arm gcc packages using
 * apt or your favorite package manager.
 */
// #include "../path_to/uart.h"
#include "uart.h"
#define BAUDRATE 115200
#define F_CPU 84000000

int main() {
  SystemInit();

  WDT->WDT_MR = WDT_MR_WDDIS;  // Disable Watchdog Timer
                               //

  // Uncomment after including uart above
  // uart_init(/*cpufreq*/, /*baud*/);
  uart_init(F_CPU, BAUDRATE);
  piob_output_init(13);
  printf("Hello World\n\r");
  pwm_init(PWM_CH1, PB13, 50);

  int x = -50;
  int x_us = pos_to_us(x);
  pwm_set_pulseWidth(PWM_CH1, x_us, 50);



  while (1) {
    // piob_set_pin_high(13);
     for (volatile int i = 0; i < 1000000; i++); // delay
     printf("toggle");
    // piob_set_pin_low(13);
     for (volatile int i = 0; i < 1000000; i++); // delay

  }
}
