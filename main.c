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
#include "game_menu.h"
#include "oled.h"
#include "spi.h"
#include "sram.h"
#include "uart.h"
#include "utility.h"
volatile uint8_t oled_ctrl_flag = 0;
buttons_t buttons;
void init_sys();
void init_gpio();

int main() {
  init_sys();
  // oled_set_font(SMALL);
  // int i = 0;
  while (1) {
    if (oled_ctrl_flag) {
      oled_ctrl_flag = 0;
      buttons = avr_get_buttons();
      menu_loop(buttons);
      update_display();
    }
    _delay_ms(50);
  }
}

void init_sys() {
  uart_init(MY_UBRR);
  ext_ram_init();
  adc_timer_init();
  init_gpio();
  spi_init();
  oled_init();
  menu_init();
  LOG_INF("System initialized.\n");
}

void init_gpio() {
  DDRB |= (1 << PB4) | (1 << PB3);  // avr_cs, display_cs as output
  spi_slave_deselect();
}

ISR(TIMER0_COMP_vect) { oled_ctrl_flag = 1; }
