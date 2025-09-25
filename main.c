#define F_CPU 4915200UL
#define BAUD 9600
#define MY_UBRR F_CPU / 16 / BAUD - 1
#ifndef __AVR_ATmega162__
#define __AVR_ATmega162__
#endif
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#include "adc.h"
#include "avr.h"
#include "input.h"
#include "oled.h"
#include "spi.h"
#include "sram.h"
#include "uart.h"
#include "utility.h"

void init_sys();
void init_gpio();

volatile uint8_t oled_ctrl_flag = 0;

int main() {
  sei();
  DDRD |= (1 << PD4);  // Set PD0 as output for testing
  init_sys();
  oled_init();
  oled_clear();

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
  DDRB |= (1 << PB4) | (1 << PB3);  // avr_cs, display_cs as output
  spi_slave_deselect();
}

ISR(TIMER0_COMP_vect) { oled_ctrl_flag = 1; }