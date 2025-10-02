#include <stdint.h>
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
#include "oled.h"
#include "spi.h"
#include "sram.h"
#include "uart.h"
#include "utility.h"

void init_sys();
void init_gpio();
volatile uint8_t oled_ctrl_flag = 0;
int main() {
  init_sys();
  int i = 0;
  while (1) {
    if (oled_ctrl_flag) {
      oled_ctrl_flag = 0;
      char buffer[32];
      snprintf(buffer, 32, "Count: %d", i++);
      LOG_INF("INTERRUPT\n");
      oled_set_font(LARGE);
      oled_printf(buffer, 0, 0);
      oled_set_font(MEDIUM);
      oled_printf("Testing", 0, 1);
      oled_set_font(LARGE);
      oled_printf("This is longg", 0, 2);
      oled_display();
    }
    _delay_ms(10);
  }
  return 0;
}

void test_draw(int i) {
  oled_draw_line(0, 0, 128, 0);  // top
  oled_printf("TEST", 26, i);
  oled_printf("ANNET", 64, 3);
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

ISR(TIMER0_COMP_vect) { oled_ctrl_flag = 1; }
