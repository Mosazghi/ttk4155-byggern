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

  while (1) {
    PORTB ^= (1 << PB0);
    sram_write_string(0, "Hello, SRAM!");
    char buffer[20];
    sram_read_string(0, buffer, sizeof(buffer));
    // Write the address of EEARL + first bit of EEARH to UART
    LOG_INF("EEARL = %02X, EEARH = %02X, Full address = %04X", EEARL, (EEARH & 0x01), ((EEARH & 0x01) << 8) | EEARL);
    LOG_INF("Read from SRAM: %s", buffer);
    _delay_ms(200);
  }
  return 0;
}

void init_sys() {
  uart_init(MY_UBRR);
  ext_ram_init();
  LOG_INF("System initialized.\n");
}