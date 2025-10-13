#include "main.h"

#include "avr/io.h"

volatile uint8_t oled_ctrl_flag = 0;
volatile uint8_t input_ctrl_flag = 0;
volatile uint8_t can_ctrl_flag = 0;

buttons_t buttons;
uint8_t init_sys();
void init_gpio();
#include "avr.h"
#include "can.h"

int main() {
  spi_config_t spi_cfg = {
      .mosi_pin_num = PB5,
      .sck_pin_num = PB7,
      .miso_pin_num = PB6,
      .clock_div = F_DIV_16,
  };

  spi_init(&spi_cfg);
  if (init_sys() != 0) {
    LOG_ERR("System initialization failed!\n");
    return 1;
  }
  // CAN TEST
  can_message_t msg = {
      .id = 0x200,
      .data_length = 1,
      .data = "H",
  };

  // LOG_INF("Pre-transmit\n");
  can_transmit(&msg);
  // _delay_ms(50);
  // LOG_INF("Post-transmit\n");

  can_message_t received = can_receive();
  LOG_INF("Data received: %d, %#x\n", received.data[0], received.id);
  // LOG_INF("Pre-while");

  _delay_ms(50);
}

uint8_t init_sys() {
  uart_init(MY_UBRR);
  ext_ram_init();
  init_gpio();
  // oled_init();
  // menu_init();
  // avr_init();
  if (mcp2515_init() != 0) {
    // LOG_ERR("MCP2515 initialization failed!\n");
    return 1;
  }
  LOG_INF("System initialized.\n");
  return 0;
}

void init_gpio() {
  DDRB |= (1 << AVR_SS_PIN) | (1 << OLED_CS) | (1 << MCP_SS_PIN);  // avr_cs, display_cs as output
  // DDRD |= (1 << MCP_SS_PIN);                    // mcp_cs as output
  PORTB |= (1 << AVR_SS_PIN) | (1 << OLED_CS) | (1 << MCP_SS_PIN);  // Set CS high
  // PORTD |= (1 << MCP_SS_PIN);                   // Set CS high
}

ISR(TIMER0_COMP_vect) { oled_ctrl_flag = 1; }

ISR(TIMER3_COMPA_vect) { input_ctrl_flag = 1; }

ISR(INT0_vect) { can_ctrl_flag = 1; }
