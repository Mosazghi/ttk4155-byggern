#define F_CPU 4915200UL
#define BAUD 9600
#define MY_UBRR F_CPU / 16 / BAUD - 1
#ifndef __AVR_ATmega162__
#define __AVR_ATmega162__
#endif
#include "adc.h"
#include "avr.h"
#include "input.h"
#include "spi.h"
#include "sram.h"
#include "uart.h"
#include "utility.h"
#include <avr/io.h>
#include <util/delay.h>

void init_sys();
void init_gpio();

int main() {
  init_sys();

  joystick_xy_t joystick_data;
  while (1) {
    joystick_data = avr_get_joystick();

    LOG_INF("Joystick data: x = %d, y = %d, btn = %d", joystick_data.x,
            joystick_data.y, joystick_data.btn);
    _delay_ms(100);
  }

  return 0;
}

void init_sys() {
  uart_init(MY_UBRR);
  ext_ram_init();
  adc_timer_init();
  init_gpio();
  spi_init();
  LOG_INF("System initialized.\n");
}

void init_gpio() {
  DDRB |= (1 << PB5) | (1 << PB4); // avr_cs, display_cs as output
  PORTB ^= (1 << PB4);             // select AVR
  PORTB ^= (1 << PB5);             // select AVR
}
