#pragma once
#include <avr/io.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/iom162.h>

#define ADC_START 0x1000 

void adc_init();
uint8_t adc_read(uint8_t channel);
