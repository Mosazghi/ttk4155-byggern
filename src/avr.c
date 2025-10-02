#include "avr.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#define F_CPU 4915200UL
#include <util/delay.h>

#include "spi.h"
#include "utility.h"

long map(long x, long in_min, long in_max, long out_min, long out_max) {
  long ret = (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
  return ret;
}

void avr_init() { avr_timer_init_10hz(); }

void avr_write(uint8_t data) {
  spi_slave_select(SPI_AVR);
  spi_transmit(data);  // Read AVR info
                       // PORTB ^= (1 << PB4); // select AVR
}

joystick_xy_t avr_get_joystick() {
  joystick_xy_t joystick;
  avr_write(0x03);  // Command to read joystick data
  _delay_us(40);
  joystick.x = spi_receive();
  _delay_us(2);
  joystick.y = spi_receive();
  _delay_us(2);
  joystick.btn = spi_receive();
  PORTB ^= (1 << PB4);  // Deselect AVR
  joystick.x = map(joystick.x, 54, 201, -100, 100);
  joystick.y = map(joystick.y, 54, 201, -100, 100);
  return joystick;
}

buttons_t avr_get_buttons() {
  buttons_t buttons;
  avr_write(0x04);  // Command for reading button data
  _delay_us(40);
  buttons.right = spi_receive();
  _delay_us(2);
  buttons.left = spi_receive();
  _delay_us(2);
  buttons.nav = spi_receive();
  spi_slave_deselect();

  return buttons;
}

touch_pad_t avr_get_touch_pad() {
  touch_pad_t touch_pad;
  avr_write(0x01);  // Command for reading touch_pad data
  _delay_us(40);
  touch_pad.x = spi_receive();
  _delay_us(2);
  touch_pad.y = spi_receive();
  _delay_us(2);
  touch_pad.size = spi_receive();
  spi_slave_deselect();

  return touch_pad;
}

touch_slider_t avr_get_touch_slider() {
  touch_slider_t touch_slider;
  avr_write(0x02);  // Command for reading touch_slider data
  _delay_us(40);
  touch_slider.x = spi_receive();
  _delay_us(2);
  touch_slider.size = spi_receive();
  spi_slave_deselect();

  return touch_slider;
}

void avr_timer_init_10hz() {
  cli();

  TCCR3B |= (1 << WGM32) | (1 << CS32) | (1 << CS30);  // CTC, 1024 prescaler
  ETIMSK |= (1 << OCIE3A);                             // IE: IV executed when OCF3A in TIFR is set
                                                       // Set when TCNT3 = OCR3A
  OCR3A = HZ_TO_TIMER(10);                             // compare register: 10hz = 459
  sei();
}