#define F_CPU 4915200UL
#include "avr.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#include "spi.h"
#include "utility.h"

long map(long x, long in_min, long in_max, long out_min, long out_max) {
  long ret = (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
  return ret;
}

inline void avr_init() { avr_timer_init_10hz(); }

uint8_t avr_transfer(uint8_t data) {
  uint8_t rx_buffer;
  spi_device_handle_t dev = {
      .ss_port_num = &PORTB,
      .ss_pin_num = AVR_SS_PIN,
  };
  spi_transfer_t transfer = {
      .tx_buf = &data,
      .rx_buf = &rx_buffer,
      .len = 1,
  };
  spi_transfer(&dev, &transfer);

  return rx_buffer;
}

joystick_xy_t avr_get_joystick() {
  joystick_xy_t joystick;
  avr_transfer(0x03);  // Command to read joystick data
  _delay_us(40);
  uint8_t buf[3];
  spi_transfer_t transfer = {
      .tx_buf = NULL,
      .rx_buf = buf,
      .len = 3,
  };
  spi_device_handle_t dev = {
      .ss_port_num = &PORTB,
      .ss_pin_num = AVR_SS_PIN,
  };
  spi_transfer(&dev, &transfer);
  joystick.x = buf[0];
  joystick.y = buf[1];
  joystick.btn = buf[2];
  joystick.x = map(joystick.x, 54, 201, -100, 100);
  joystick.y = map(joystick.y, 54, 201, -100, 100);

  return joystick;
}

buttons_t avr_get_buttons() {
  buttons_t buttons;
  avr_transfer(0x04);  // Command for reading button data
  _delay_us(40);
  buttons.right = avr_transfer(0xFF);
  _delay_us(2);
  buttons.left = avr_transfer(0xFF);
  _delay_us(2);
  buttons.nav = avr_transfer(0xFF);

  return buttons;
}

touch_pad_t avr_get_touch_pad() {
  touch_pad_t touch_pad;
  avr_transfer(0x01);  // Command for reading touch_pad data
  _delay_us(40);
  touch_pad.x = avr_transfer(0xFF);
  _delay_us(2);
  touch_pad.y = avr_transfer(0xFF);
  _delay_us(2);
  touch_pad.size = avr_transfer(0xFF);

  return touch_pad;
}

touch_slider_t avr_get_touch_slider() {
  touch_slider_t touch_slider;
  avr_transfer(0x02);  // Command for reading touch_slider data
  _delay_us(40);
  touch_slider.x = avr_transfer(0xFF);
  _delay_us(2);
  touch_slider.size = avr_transfer(0xFF);

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