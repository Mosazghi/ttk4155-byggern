#include "servo.h"

// Set pin n on port p as output
void piob_output_init(int pin) {
    
    // Enable PIOB timer
    PMC->PMC_PCER0 = (1 << ID_PIOB);

    // Enable pin control by PIO
    PIOB->PIO_PER = (1 << pin);

    // Set pin as output
    PIOB->PIO_OER = (1 << pin);

}

void piob_set_pin_low(int pin){
    PIOB->PIO_CODR = (1 << pin);
}

void piob_set_pin_high(int pin){
    PIOB->PIO_SODR = (1 << pin);
}