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
#include "avr.h"
#include "input.h"
#include "oled.h"
#include "spi.h"
#include "sram.h"
#include "uart.h"
#include "utility.h"

void init_sys();
void init_gpio();

volatile uint8_t oled_ctrl_flag = 0;

int main() {
  sei();
  DDRD |= (1 << PD4);  // Set PD0 as output for testing
  init_sys();
  oled_init();
  oled_clear();
  while (1) {
    if (oled_ctrl_flag) {
      oled_update();
    }
  }
  // uint8_t packet[3] = {0x05, 1, 1};
  // PORTB &= ~(1 << PB4);
  // spi_transmit_packet(packet, 3);
  // // spi_transmit(0x05);
  // // spi_transmit(0);
  // // spi_transmit(1);
  // PORTB |= (1 << PB4);
  // joystick_xy_t joystick_data;
  // while (1) {
  //   // joystick_data = avr_get_joystick();

  //   // LOG_INF("Joystick data: x = %d, y = %d, btn = %d", joystick_data.x,
  //   //         joystick_data.y, joystick_data.btn);
  //   _delay_ms(100);
  // }

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
  DDRB |= (1 << PB4) | (1 << PB3);   // avr_cs, display_cs as output
  PORTB |= (1 << PB4) | (1 << PB3);  // select display
}

ISR(TIMER0_COMP_vect) { oled_ctrl_flag = 1; }