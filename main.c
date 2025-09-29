#define F_CPU 4915200UL
#define BAUD 9600
#define MY_UBRR F_CPU / 16 / BAUD - 1
#ifndef __AVR_ATmega162__
#define __AVR_ATmega162__
#endif
#include "adc.h"
#include "avr.h"
#include "input.h"
#include "menu.h"
#include "oled.h"
#include "spi.h"
#include "sram.h"
#include "uart.h"
#include "utility.h"
#include <avr/io.h>
#include <util/delay.h>

void init_sys();
void init_gpio();

static menu_state_t menu_state;
static buttons_t buttons;
// menu_t menu;

int main() {
  init_sys();
  oled_init();
  oled_clear();
  // menu_state_t menu_state;
  menu_init(&menu_state, NULL);

  while (1) {
    buttons = avr_get_buttons();
    // LOG_INF("Current Menu: %s\n", menu_state.current_menu->title);
    // LOG_INF("Current Selection: %s\n", menu.items->label);
    hat_test_loop(buttons);
    _delay_ms(200);
  }

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
  DDRB |= (1 << PB4) | (1 << PB3); // avr_cs, display_cs as output
  spi_slave_deselect();
}
