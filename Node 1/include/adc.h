#pragma once
#include <avr/io.h>
#include <avr/iom162.h>
#include <stdint.h>
#include <stdio.h>

#define ADC_START 0x1000  //< Start address for ADC data storage

/**
 * @brief ADC channels
 */
typedef enum {
  ADC_CH0,
  ADC_CH1,
  ADC_CH2,
  ADC_CH3,
} adc_channel_t;

/**
 * @brief Initialize the ADC timer
 */
void adc_timer_init();
/**
 * @brief Read value from the specified ADC channel
 *
 * @param channel The ADC channel to read from
 * @return uint8_t The read ADC value
 */
uint8_t adc_read(adc_channel_t channel);
