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

#include "game_menu.h"
#include "oled.h"
#include "spi.h"
#include "sram.h"
#include "uart.h"
#include "utility.h"
volatile uint8_t oled_ctrl_flag = 0;
volatile uint8_t input_ctrl_flag = 0;
buttons_t buttons;
void init_sys();
void init_gpio();
#include "avr.h"
#include "game_logic.h"

int main() {
  init_sys();
  // int i = 0;
  while (1) {
    if (input_ctrl_flag) {
      input_ctrl_flag = 0;
      buttons = avr_get_buttons();
      menu_loop(&buttons);
    }
    if (oled_ctrl_flag) {
      oled_ctrl_flag = 0;
      if (g_game_state.is_in_game) {
        game_loop(&buttons);
      } else {
        update_display();
      }
    }
    _delay_ms(50);
  }
}

void init_sys() {
  uart_init(MY_UBRR);
  ext_ram_init();
  init_gpio();
  spi_init();
  oled_init();
  menu_init();
  avr_init();
  LOG_INF("System initialized.\n");
}

void init_gpio() {
  DDRB |= (1 << PB4) | (1 << PB3);  // avr_cs, display_cs as output
  DDRD |= (1 << PD3);               // TEMP: PD4 output
  DDRD |= (1 << PD4);               // TEMP: PD4 output

  spi_slave_deselect();
}

ISR(TIMER0_COMP_vect) { oled_ctrl_flag = 1; }

ISR(TIMER3_COMPA_vect) { input_ctrl_flag = 1; }