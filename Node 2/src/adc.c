#include "adc.h"

#define IR_ADC_CHANNEL 7
void adc_init() {
  // Disable write protection for ADC_MR
  //   ADC->ADC_WPMR &= ~(1 << 0);

  //   // Enable ADC Peripheral clock
  PMC->PMC_PCER1 |= (1 << ID_ADC - 32);

  // Enable single ended freerun mode
  ADC->ADC_MR |= ADC_MR_FREERUN_ON;

  // Start conversion once
  // Enable ADC channel 0
  ADC->ADC_CHER |= ADC_CHER_CH7;
  ADC->ADC_CR |= ADC_CR_START;

  // adc pin PA16
  PIOA->PIO_PER |= (1u << 16);  // Enable pin control by PIOA
  PIOA->PIO_ODR |= (1u << 16);  // Set pin as input
  //   PIOA->PIO_PDR = PIO_PDR_P16;    // Disable PIO control to let ADC use the pin
  //   PIOA->PIO_PUDR |= PIO_PDR_P16;  // Disable pull-up resistor
}

uint16_t adc_read() {
  ADC->ADC_CR |= ADC_CR_START;             // Start conversion
  while (!(ADC->ADC_ISR & ADC_ISR_EOC7));  // Wait for conversion complete flag
  return ADC->ADC_CDR[IR_ADC_CHANNEL];
}

uint16_t adc_clean_read() {
  uint16_t adc_clean;
  while (!(ADC->ADC_ISR & (1 << 0)));  // Wait for conversion complete flag
  uint16_t adc_value = ADC->ADC_CDR[IR_ADC_CHANNEL];
  if (adc_value > ADC_MIN && adc_value < ADC_MAX) {
    adc_clean = adc_value;
  }
  return adc_clean;
  return 1;
}