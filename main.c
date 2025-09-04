#define F_CPU 4915200UL
#define BAUD 9600
#define MY_UBRR F_CPU/16/BAUD-1
#ifndef __AVR_ATmega162__
#define __AVR_ATmega162__
#endif 
#include <avr/io.h>
#include <util/delay.h>
#include "uart.h"

int main() {
  DDRB |= (1 << PB0); 
  uart_init(MY_UBRR);

  int i = 0;
  while(1) {
    PORTB ^= (1 << PB0); 
    printf("Hello world %d\r\n", i++);
    _delay_ms(200);
  }

  return 0;
}
