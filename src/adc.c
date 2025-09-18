
#include "adc.h"
#include "utility.h"
#define N_CHANNELS 2
#define F_CPU 4915200UL
#include <util/delay.h>
#define T_CONV (9 * N_CHANNELS * 2) / F_CPU

void adc_init() {

  // Set PD5 (OC1A) as output
  DDRD |= (1 << PD5);

  TCCR1B = (1 << WGM12) | (1 << CS10); // CTC mode, no prescaler
  TCCR1A |= (1 << COM1A0);

  OCR1A =
      0; // f_out = F_CPU / (2 * (OCR1A + 1)); 0 here gives half CPU frequency
}

uint8_t adc_read(ADC_CHANNEL channel) {
  LOG_DBG("Reading from ADC: channel = %02X", (1 << channel));
  volatile uint8_t *ext_ram = (uint8_t *)ADC_START; // ADC start address
  for (int i = 0; i < (int)channel; i++) { // Cycle to the correct channel
    ext_ram[0] = (1 << channel);           // Read data from ADC
    _delay_us(T_CONV);                     // Wait for ADC to stabilize
  }
  uint8_t value =
      ext_ram[channel]; // Read the ADC value from the specified channel
  return value;
}
