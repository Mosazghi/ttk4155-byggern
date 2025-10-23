#include "main.h"

#include <stdint.h>

#include "avr/io.h"
#include "include/oled.h"

volatile uint8_t oled_ctrl_flag = 0;
volatile uint8_t input_ctrl_flag = 0;
volatile uint8_t can_ctrl_flag = 0;

buttons_t buttons;
joystick_xy_t joystick;
touch_pad_t touch_pad;
uint8_t init_sys();
void init_gpio();
#include "avr.h"
#include "can.h"

int main() {
  spi_config_t spi_cfg = {
      .mosi_pin = PB5,
      .sck_pin = PB7,
      .miso_pin = PB6,
      .clock_div = F_DIV_16,
  };

  spi_init(&spi_cfg);
  if (init_sys() != 0) {
    LOG_ERR("System initialization failed!\n");
    return 1;
  }

  can_message_t msg = {.id = 299, .data = {0}, .data_length = 7};

  oled_clear();
  oled_printf("hello world", 32, 3);
  oled_display();

  while (1) {
    if (input_ctrl_flag) {
      buttons = avr_get_buttons();
      joystick = avr_get_joystick();
      touch_pad = avr_get_touch_pad();

      msg.data[0] = (uint8_t)joystick.x;
      msg.data[1] = (uint8_t)joystick.y;
      msg.data[2] = (uint8_t)joystick.btn;
      msg.data[3] = buttons.right;
      msg.data[4] = touch_pad.x;
      msg.data[5] = touch_pad.y;
      msg.data[6] = touch_pad.size;

      LOG_INF("joystick data: x:%d y:%d\n", joystick.x, joystick.y);

      if (can_transmit(&msg) != CAN_ERROR_NONE) {
        LOG_ERR("CAN transmit failed!\n");
      }
      input_ctrl_flag = 0;
    }

    // if (can_ctrl_flag) {
    //   can_ctrl_flag = 0;
    //   can_message_t received = can_receive();
    //   LOG_INF("ID received: %d\n", received.id);
    //   LOG_INF("Data received: %s \n", received.data);
    // }
    _delay_ms(50);
  }
  return 0;
}

uint8_t init_sys() {
  uart_init(MY_UBRR);
  ext_ram_init();
  init_gpio();
  oled_init();
  menu_init();
  avr_init();
  if (mcp2515_init() != 0) {
    // LOG_ERR("MCP2515 initialization failed!\n");
    return 1;
  }
  LOG_INF("System initialized - V3\n");
  return 0;
}

void init_gpio() {
  DDRB |= (1 << AVR_SS_PIN) | (1 << OLED_CS) | (1 << MCP_SS_PIN);
  PORTB |= (1 << AVR_SS_PIN) | (1 << OLED_CS) | (1 << MCP_SS_PIN);
}

ISR(TIMER0_COMP_vect) { oled_ctrl_flag = 1; }

ISR(TIMER3_COMPA_vect) { input_ctrl_flag = 1; }

ISR(INT0_vect) { can_ctrl_flag = 1; }
