#include "avr.h"

#include <avr/interrupt.h>
#include <avr/io.h>

#include "spi.h"
#define F_CPU 4915200UL
#include <stddef.h>
#include <util/delay.h>

#include "utility.h"

static spi_device_handle_t spi_avr_dev = {
    .ss_port = &PORTB,
    .ss_pin = AVR_SS_PIN,
};

inline long map(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

uint8_t deadzone_adjust(joystick_xy_t* joystick, uint8_t threshold) {
  if (joystick == NULL) {
    return 1;  // Error: Null pointer
  }

  if (ABS(joystick->x) < threshold) {
    joystick->x = 0;
  }

  if (ABS(joystick->y) < threshold) {
    joystick->y = 0;
  }

  return 0;
}

void avr_init() {
  joystick_xy_t joystick = avr_get_joystick();
  deadzone_adjust(&joystick, 15);
  avr_timer_init_10hz();
}

joystick_xy_t avr_get_joystick() {
  joystick_xy_t joystick;
  spi_slave_select(&spi_avr_dev);

  spi_transmit(0x03);

  joystick.x = spi_receive();
  joystick.y = spi_receive();
  joystick.btn = spi_receive();

  spi_slave_deselect(&spi_avr_dev);

  joystick.x = map(joystick.x, 54, 201, -100, 100);
  joystick.y = map(joystick.y, 54, 201, -100, 100);

  return joystick;
}

buttons_t avr_get_buttons() {
  buttons_t buttons;
  spi_slave_select(&spi_avr_dev);

  spi_transmit(0x04);

  buttons.right = spi_receive();
  buttons.left = spi_receive();
  buttons.nav = spi_receive();

  spi_slave_deselect(&spi_avr_dev);
  return buttons;
}

touch_pad_t avr_get_touch_pad() {
  touch_pad_t touch_pad;

  spi_slave_select(&spi_avr_dev);

  spi_transmit(0x01);

  touch_pad.x = spi_receive();
  touch_pad.y = spi_receive();
  touch_pad.size = spi_receive();

  spi_slave_deselect(&spi_avr_dev);
  return touch_pad;
}

touch_slider_t avr_get_touch_slider() {
  touch_slider_t touch_slider;
  spi_slave_select(&spi_avr_dev);

  spi_transmit(0x02);

  touch_slider.x = spi_receive();
  touch_slider.size = spi_receive();

  spi_slave_deselect(&spi_avr_dev);
  return touch_slider;
}

void avr_timer_init_10hz() {
  cli();

  TCCR3B |= (1 << WGM32) | (1 << CS32) | (1 << CS30);  // CTC, 1024 prescaler
  ETIMSK |= (1 << OCIE3A);                             // IE: IV executed when OCF3A in TIFR is set
                                                       // Set when TCNT3 = OCR3A
  OCR3A = HZ_TO_TIMER(5);                              // compare register: 10hz = 459
  sei();
}
