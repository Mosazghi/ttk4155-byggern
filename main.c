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

int main() {
  DDRB |= (1 << PB0); 
  uart_init(MY_UBRR);
  char data[100];
  int i = 0;
  while(1) {
    scanf("%s", data); 
    if(data) {
      printf("Rec: %s\n\r", data);
      if (strcmp(data, "y") == 0)  {
        PORTB ^= (1 << PB0); 
      }
    }
    _delay_ms(100);
  }

  return 0;
}
