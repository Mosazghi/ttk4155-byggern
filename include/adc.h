#pragma once
#include <avr/io.h>
#include <avr/iom162.h>
#include <stdint.h>
#include <stdio.h>

#define ADC_START 0x1000
typedef enum {
  ADC_CH0,
  ADC_CH1,
  ADC_CH2,
  ADC_CH3,
} adc_channel_t;

void adc_timer_init();
uint8_t adc_read(adc_channel_t channel);
