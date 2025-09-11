#define F_CPU 4915200UL
#define BAUD 9600
#define MY_UBRR F_CPU/16/BAUD-1
#ifndef __AVR_ATmega162__
#define __AVR_ATmega162__
#endif 
#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include <util/delay.h>
#include "uart.h"
#include "sram.h"
#include "utility.h"
#include "adc.h"
void init_sys();

int main() {
  DDRB |= (1 << PB0); 

  init_sys();
  adc_init();

  // adc_read(0); 
  // char data[255];
  // int i = 0;
  while (1) {
    PORTB ^= (1 << PB0);
    _delay_ms(1000);
  }
  return 0;
}

void init_sys() {
  uart_init(MY_UBRR);
  ext_ram_init();
}