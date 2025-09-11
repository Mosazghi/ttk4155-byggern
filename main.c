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
void init_sys();

int main() {
  DDRB |= (1 << PB0); 

  init_sys();

  char data[255];
  sram_write_string(0x1400, "Hello, world!");
  sram_read_string(0x1400, data, sizeof(data));
  printf("SRAM read: %s\n\r", data);
  // sprintf(data, "%s", sram_read(0x1400));
  // printf("SRAM read: %s\n", data);
  // while(1) {
  //   PORTB ^= (1 << PB0); 
  //   _delay_ms(100);
  // }

  return 0;
}

void init_sys() {
  uart_init(MY_UBRR);
  ext_ram_init();
}