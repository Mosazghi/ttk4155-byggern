#define F_CPU 16000000UL
#ifndef __AVR_ATmega162__
#define __AVR_ATmega162__
#endif 
#include <avr/io.h>
#include <util/delay.h>

int main() {
  DDRB |= (1 << PB0); // Set PB0 as output

  while(1) {
    PORTB ^= (1 << PB0); // Toggle pin PB0
    _delay_ms(5000);
  }

  return 0;
}
