#pragma once
#include <avr/io.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/iom162.h>

#define ADC_START 0x1000 

typedef enum {
  ADC_CH0,
  ADC_CH1,
  ADC_CH2,
  ADC_CH3,
} ADC_CHANNEL;

void adc_init();
uint8_t adc_read(ADC_CHANNEL channel);
