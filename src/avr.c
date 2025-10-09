#include "avr.h"

#include <avr/interrupt.h>
#include <avr/io.h>

#include "spi.h"
#define F_CPU 4915200UL
#include <stddef.h>
#include <util/delay.h>

#include "utility.h"

static spi_device_handle_t spi_avr_dev = {
    .ss_port_num = &PORTB,
    .ss_pin_num = AVR_SS_PIN,
};

inline long map(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

uint8_t deadzone_adjust(joystick_xy_t *joystick, uint8_t threshold) {
  if (joystick == NULL) {
    return 1;  // Error: Null pointer
  }

  if (sqrt(joystick->x * joystick->x) < threshold) {
    joystick->x = 0;
  }

  if (sqrt(joystick->y * joystick->y) < threshold) {
    joystick->y = 0;
  }

  return 0;
}

void avr_init() {
  joystick_xy_t joystick = avr_get_joystick();
  deadzone_adjust(&joystick, 15);
  avr_timer_init_10hz();
}

// Low-level SPI transfer without SS control
uint8_t avr_transfer(uint8_t data) {
  uint8_t rx_buffer;
  spi_transfer_t transfer = {
      .tx_buf = &data,
      .rx_buf = &rx_buffer,
      .len = 1,
  };
  spi_transfer(&spi_avr_dev, &transfer);

  return rx_buffer;
}

joystick_xy_t avr_get_joystick() {
  joystick_xy_t joystick;
  uint8_t joy_data[4];
  uint8_t cmd[] = {0x03};
  spi_transfer_t transfer = {
      .tx_buf = cmd,
      .rx_buf = joy_data,
      .len = 4,
  };
  spi_transfer(&spi_avr_dev, &transfer);

  joystick.x = joy_data[1];
  joystick.y = joy_data[2];
  joystick.btn = joy_data[3];

  joystick.x = map(joystick.x, 54, 201, -100, 100);
  joystick.y = map(joystick.y, 54, 201, -100, 100);

  return joystick;
}

buttons_t avr_get_buttons() {
  buttons_t buttons;
  uint8_t cmd[] = {0x04};
  uint8_t rx_buffer[4];
  spi_transfer_t transfer = {
      .tx_buf = cmd,
      .rx_buf = rx_buffer,
      .len = 4,
  };

  spi_transfer(&spi_avr_dev, &transfer);

  buttons.right = rx_buffer[1];
  buttons.left = rx_buffer[2];
  buttons.nav = rx_buffer[3];

  return buttons;
}

touch_pad_t avr_get_touch_pad() {
  touch_pad_t touch_pad;
  uint8_t cmd[] = {0x01};
  uint8_t rx_buffer[4];
  spi_transfer_t transfer = {
      .tx_buf = cmd,
      .rx_buf = rx_buffer,
      .len = 4,
  };
  spi_transfer(&spi_avr_dev, &transfer);

  touch_pad.x = rx_buffer[1];
  touch_pad.y = rx_buffer[2];
  touch_pad.size = rx_buffer[3];

  return touch_pad;
}

touch_slider_t avr_get_touch_slider() {
  touch_slider_t touch_slider;
  const uint8_t cmd[] = {0x02};
  uint8_t rx_buffer[4];
  spi_transfer_t transfer = {
      .tx_buf = cmd,
      .rx_buf = rx_buffer,
      .len = 4,
  };
  spi_transfer(&spi_avr_dev, &transfer);

  touch_slider.x = rx_buffer[1];
  touch_slider.size = rx_buffer[2];

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