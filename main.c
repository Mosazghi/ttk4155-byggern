#include "main.h"

#include "avr.h"
#include "avr/io.h"
#include "can.h"

// Interrupts variables.
volatile uint8_t oled_ctrl_flag = 0;
volatile uint8_t input_ctrl_flag = 0;
volatile uint8_t can_ctrl_flag = 0;

static uint8_t init_sys();
static void init_gpio();

int main(void) {
  system_state_t sys_state = STATE_MENU;

  // Inputs
  buttons_t buttons;
  joystick_xy_t joystick_xy;

  // SPI
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

  while (1) {
    if (input_ctrl_flag) {
      input_ctrl_flag = 0;
      buttons = avr_get_buttons();
      joystick_xy = avr_get_joystick();
    }

    if (oled_ctrl_flag) {
      oled_ctrl_flag = 0;
      if (sys_state == STATE_MENU) {
        render_menu();
      } else {
        render_game(&g_game_state);
      }
    }

    if (can_ctrl_flag) {
      can_ctrl_flag = 0;
      can_message_t msg = can_receive();
    }

    // State machine.
    if (sys_state == STATE_MENU) {
      menu_loop(&buttons);
    } else {
      game_loop(&g_game_state, &joystick_xy);
    }

    if (g_game_state.is_in_game && sys_state == STATE_MENU) {
      sys_state = STATE_GAME;
    } else if (!g_game_state.is_in_game && sys_state == STATE_GAME) {
      sys_state = STATE_MENU;
    }

    _delay_ms(10);
  }
}

static uint8_t init_sys() {
  uart_init(MY_UBRR);
  ext_ram_init();
  init_gpio();
  oled_init();
  menu_init();
  avr_init();
  if (mcp2515_init() != 0) {
    LOG_ERR("MCP2515 initialization failed!\n");
    return 1;
  }
  LOG_INF("System initialized.\n");
  return 0;
}

static void init_gpio() {
  DDRB |= (1 << AVR_SS_PIN) | (1 << OLED_CS) | (1 << MCP_SS_PIN);
  PORTB |= (1 << AVR_SS_PIN) | (1 << OLED_CS) | (1 << MCP_SS_PIN);
}

ISR(TIMER0_COMP_vect) { oled_ctrl_flag = 1; }

ISR(TIMER3_COMPA_vect) { input_ctrl_flag = 1; }

ISR(INT0_vect) { can_ctrl_flag = 1; }
