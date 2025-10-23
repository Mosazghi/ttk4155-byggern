#include "adc_internal.h"

void adc_init(){
    // Disable write protection for ADC_MR
    ADC->ADC_WPMR &= ~(1 << 0);
    
    // Enable ADC Peripheral clock
    PMC->PMC_PCER1 = (1 << ID_ADC -32);

    // Enable single ended freerun mode
    ADC->ADC_MR = ADC_MR_FREERUN_ON;

    // Alternative ADC_MR setup if minimalist doesn't work
    // ADC->ADC_MR =
    // ADC_MR_FREERUN_ON    |   // Free-running mode
    // ADC_MR_TRGEN_DIS     |   // No external trigger
    // ADC_MR_PRESCAL(41)   |   // ADCClock = MCK / (2*(41+1)) = 1 MHz
    // ADC_MR_STARTUP_SUT64 |
    // ADC_MR_TRACKTIM(15);

    // Enable ADC channel 0
    ADC->ADC_CHER = ADC_CHER_CH0;

    // Start conversion once
    ADC->ADC_CR = ADC_CR_START;
}

uint16_t adc_read(){
    return ADC->ADC_CDR[0];
}

uint16_t adc_clean_read() {
    uint16_t adc_clean;
    while (!(ADC->ADC_ISR & (1 << 0)));          // Wait for conversion complete flag
    uint16_t adc_value = ADC->ADC_CDR[0];
    if (adc_value > ADC_MIN && adc_value < ADC_MAX) {
        adc_clean = adc_value;
    } return adc_clean;
    return 1;
}