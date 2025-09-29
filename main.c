#define F_CPU 4915200UL
#define BAUD 9600
#define MY_UBRR F_CPU / 16 / BAUD - 1
#ifndef __AVR_ATmega162__
#define __AVR_ATmega162__
#endif
#include <avr/io.h>
#include <util/delay.h>

#include "adc.h"
#include "oled.h"
#include "spi.h"
#include "sram.h"
#include "uart.h"
#include "utility.h"

void init_sys();
void init_gpio();

int main() {
  init_sys();
  oled_clear();
  // oled_set_font(SMALL);
  // oled_printf("Hello world", 0, 0);
  // oled_set_font(MEDIUM);
  // oled_printf("Testing", 0, 1);
  // oled_set_font(LARGE);
  // oled_printf("This is longg", 0, 2);
  oled_draw_pixel(0, 0);
  oled_draw_pixel(128, 0);

  oled_draw_pixel(0, 64);
  oled_draw_pixel(128, 64);

  oled_draw_pixel(64, 32);
  oled_draw_pixel(2, 0);
  oled_draw_pixel(24, 32);
  oled_display();
  while (1) {
    _delay_ms(10);
  }
  return 0;
}

void init_sys() {
  uart_init(MY_UBRR);
  ext_ram_init();
  adc_timer_init();
  init_gpio();
  spi_init();
  oled_init();
  LOG_INF("System initialized.\n");
}

void init_gpio() {
  DDRB |= (1 << PB4) | (1 << PB3);   // avr_cs, display_cs as output
  PORTB |= (1 << PB4) | (1 << PB3);  // select display
}
