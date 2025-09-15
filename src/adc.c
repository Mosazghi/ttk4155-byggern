
#include "adc.h"
#include "utility.h"

void adc_init() {

    // Set PD5 (OC1A) as output
    DDRD |= (1 << PD5);

    TCCR1B = (1 << WGM12) | (1 << CS10); // CTC mode, no prescaler
    TCCR1A |= (1 << COM1A0);

    OCR1A = 0; // f_out = F_CPU / (2 * (OCR1A + 1)); 0 here gives half CPU frequency

}

uint8_t adc_read(uint8_t channel){
  LOG_DBG("Reading from ADC: channel = %02X", channel);
  volatile uint8_t *ext_ram = (uint8_t *) ADC_START; // ADC start address
  return ext_ram[channel];               // Read data from ADC
}