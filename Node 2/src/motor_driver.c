#include "motor_driver.h"

void motor_init(){
    pwm_init(PWM_CH0, PB12, 1e4);

    // Enable clock
    PMC->PMC_PCER0 = (ID_PIOC);

    // Enable PIO control of PC23 (GPIO, not peripheral)
    PIOC->PIO_PER = PC23;

    // Disable pull-up resistor (optional)
    PIOC->PIO_PUDR = PC23;

    // Configure PC23 as output
    PIOC->PIO_OER = PC23;

    // Optionally set initial state (low)
    PIOC->PIO_CODR = PC23;
}

void motor_set_dir(enum motor_direction dir) {
    if (dir == LEFT) {
        PIOC->PIO_CODR = PC23;
    }
    if (dir == RIGHT) {
        PIOC->PIO_SODR = PC23;
    }
}

uint8_t motor_get_dir(touch_pad_t *touchpad) {
    if (touchpad->x < 128) {
        return 0;
    }
    if (touchpad->x >= 128) {
        return 1;
    }
    return 0;
}

// TODO: Motor encoder setup

// PC25, PC26 = encoder channel A & B
// XC0 selected as clock for TC2 channel 0
// Externally driven 
// overflow vs physical endpoints?
// - TC_CV: Current count value
// - TC_BMR: Channel mode register
// - TC_CCR: Channel control register

void motor_encoder_init(){};
