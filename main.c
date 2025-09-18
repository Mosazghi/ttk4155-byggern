#define F_CPU 4915200UL
#define BAUD 9600
#define MY_UBRR F_CPU / 16 / BAUD - 1
#ifndef __AVR_ATmega162__
#define __AVR_ATmega162__
#endif
#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include <util/delay.h>
#include "uart.h"
#include "sram.h"
#include "utility.h"
#include "adc.h"
#include "input.h"

void init_sys();

int main()
{
  DDRB |= (1 << PB0);
  init_sys();

  
while (1)
  {
    PORTB ^= (1 << PB0);
    ADC input_values = adc_read_all();
    LOG_INF("Joystick X: %d, Y: %d\n", input_values.joystick.x, input_values.joystick.y);
    //LOG_INF("Touch Pad X: %d, Y: %d\n", input_values.touch_pad.x, input_values.touch_pad.y);
    //uint8_t adc_ch1 = adc_read(ADC_CH0);
    //uint8_t adc_ch2 = adc_read(ADC_CH1);
    //LOG_INF("ADC Value: %d (CHANNEL 0) %d (CHANNEL 1)\n", adc_ch1, adc_ch2);
    _delay_ms(200);
  }
  return 0;
}

void init_sys()
{
  uart_init(MY_UBRR);
  ext_ram_init();
  adc_timer_init();
  LOG_INF("System initialized.\n");
}

