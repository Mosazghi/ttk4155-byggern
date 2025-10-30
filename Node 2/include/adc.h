#pragma once
#include "sam.h"
#include "sam3xa.h"  // Header for SAM3X register definitions
#define ADC_MIN 1e2
#define ADC_MAX 1e6

/**
 * @brief ADC initiation for IR sensor.
 *
 * Pin A0 / Port D54
 *
 * Channel 0
 */
void adc_init();

/**
 * @brief Returns adc data from channel 0.
 *
 * @return uint16_t
 */
uint16_t adc_read();

/**
 * @brief Returns adc_read value if within expected range. Returns 1 otherwise.
 *
 * Waits for conversion complete flag to be set.
 *
 * Range given by ADC_MIN, ADC_MAX.
 *
 * Intended to eliminate conversions due to noise.
 *
 * @param adc_value
 * @return uint16_t
 */
uint16_t adc_clean_read();