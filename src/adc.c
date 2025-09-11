
#include "adc.h"
#include "utility.h"

void adc_init() {
    DDRB |= (1 << PB1);   // Set PB1 as output (OC1A clock output)
    // CTC mode, clear on compare match, no prescaling
    // TCCR0 |= (1 << WGM01) | (1 << COM01) | (1 << CS00);
    // OCR0 = 0;              // Set compare value for
    TCCR1A |= (1 << COM1A0); // Toggle OC1A on compare match
    TCCR1B |= (1 << WGM12) | (1 << CS10); 
    OCR1A = 7;               // Set compare value for 50% duty cycle
}

uint8_t adc_read(uint8_t channel){
  LOG_DBG("Reading from ADC: channel = %02X", channel);
  volatile uint8_t *ext_ram = (uint8_t *) ADC_START; // ADC start address
  return ext_ram[channel];               // Read data from ADC
}