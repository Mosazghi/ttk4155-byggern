#include "motor_driver.h"

void motor_init(){
    pwm_init(PWM_CH0, PB12, 1e4);

    // Enable clock
    PMC->PMC_PCER0 = (ID_PIOC); // potentially: (1 << ID_PIOC)

    // Enable PIO control of PC23 (GPIO, not peripheral)
    PIOC->PIO_PER = PC23;

    // Disable pull-up resistor (optional?)
    PIOC->PIO_PUDR = PC23;

    // Configure PC23 as output
    PIOC->PIO_OER = PC23;

    // Set initial state (low, optional)
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

void motor_set_speed(int joy_x){
    const int deadzone = 5;
    if (abs(joy_x) < deadzone) {
        pwm_set_dutyCycle(PWM_CH0, 0);
    }
    pwm_set_dutyCycle(PWM_CH0, abs(joy_x));
}

uint8_t motor_get_dir(uint8_t value) { 
    if (value < 128) {
        return 0;
    }
    if (value >= 128) {
        return 1;
    }
    return 0;
}

int joy_get_dir(int joy_x) { 
    if (joy_x < 0) {
        return 0;
    }
    if (joy_x >= 0) {
        return 1;
    }
    return 0;
}

void motor_encoder_init(){
    // Enable clock for TC2 (ch6)
    PMC->PMC_PCER1 = (1 << (ID_TC6 - 32));

    // Configure PIO pins for TIOA6/TIOB6 (Peripheral B)
    PIOC->PIO_PDR = PC25 | PC26;    // Disable GPIO control
    PIOC->PIO_ABSR |= PC25 | PC26;  // Select Peripheral B

    // Configure Block Mode Register for Quadrature Decoder
    TC2->TC_BMR =
        TC_BMR_QDEN     |   // Enable quadrature decoding
        TC_BMR_POSEN    |   // Position enable mode
        TC_BMR_EDGPHA;      // Count on rising edge of PHA 

    // Select the quadrature mode in Channel 0
    TC2->TC_CHANNEL[0].TC_CMR = TC_CMR_TCCLKS_XC0; // External clock from quadrature logic

    // Enable the clock and trigger the counter
    TC2->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;
}

int32_t encoder_get_position(void){
    // Returns signed 32-bit position count
    return (int32_t)TC2->TC_CHANNEL[0].TC_CV;
}


