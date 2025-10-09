#include "main.h"
volatile uint8_t oled_ctrl_flag = 0;
volatile uint8_t input_ctrl_flag = 0;
buttons_t buttons;
void init_sys();
void init_gpio();
#include "avr.h"
#include "game_logic.h"

int main() {
  spi_config_t spi = {
      .mosi_pin_num = PB5,
      .sck_pin_num = PB7,
      .miso_pin_num = PB6,
      .clock_div = F_DIV_16,
  };
  spi_init(&spi);
  init_sys();
  // int i = 0;
  while (1) {
    if (input_ctrl_flag) {
      input_ctrl_flag = 0;
      buttons = avr_get_buttons();
      joystick_xy_t joystick = avr_get_joystick();
      // LOG_INF("Joystick X: %d, Y: %d, Btn: %d\n", joystick.x, joystick.y, joystick.btn);
      menu_loop(&buttons);
    }
    if (oled_ctrl_flag) {
      oled_ctrl_flag = 0;
      // if (g_game_state.is_in_game) {
      //   game_loop(&buttons);
      // } else {
      update_display();
      // }
    }
    _delay_ms(50);
  }
}

void init_sys() {
  uart_init(MY_UBRR);
  ext_ram_init();
  init_gpio();
  oled_init();
  menu_init();
  avr_init();
  LOG_INF("System initialized.\n");
}

void init_gpio() {
  DDRB |= (1 << AVR_SS_PIN) | (1 << OLED_CS);   // avr_cs, display_cs as output
  PORTB |= (1 << AVR_SS_PIN) | (1 << OLED_CS);  // Set CS high
}

ISR(TIMER0_COMP_vect) { oled_ctrl_flag = 1; }

ISR(TIMER3_COMPA_vect) { input_ctrl_flag = 1; }