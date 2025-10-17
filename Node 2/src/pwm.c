#include "pwm.h"

// PWM_init for PWM_channel, PortB_pin
void pwm_init(enum PWM_CHANNEL channel, enum portb_pin pin){

    // 1. Enable peripheral clocks for PIOB & PWM
    PMC->PMC_PCER0 = (1 << ID_PIOB);
    PMC->PMC_PCER1 = (1 << (ID_PWM - 32));

    // 2. Enable peripheral mode B (disable PIO control)
    PIOB->PIO_PDR = pin;

    // 3. Select perhipheral mode B (PWM)
    PIOB->PIO_ABSR |= pin;

    // 4. Disable pull-up
    PIOB->PIO_PUDR = pin;

    // 5. Configure PWM clock: f_pwm = CLKA / period = 50Hz 
    // CLKA = MCK / (DIVA * 2^PREA), MCK = 84MHz, Duty Cycle = [0.9, 2.1]ms
    PWM->PWM_CLK = PWM_CLK_PREA(3) | PWM_CLK_DIVA(8);   // PREA(3) => divide by 8
                                                        // DIVA(8) => divide by 8

    // 6. Configure PWM channel
    PWM->PWM_CH_NUM[channel].PWM_CMR = PWM_CMR_CPRE_CLKA;
    PWM->PWM_CH_NUM[channel].PWM_CPRD = 26250;           // Period: 50Hz = 20ms = 26250
    PWM->PWM_CH_NUM[channel].PWM_CDTY = 2625;            // Duty cycle: 2ms = 2625

    // 7. Enable PWM channel 
    PWM->PWM_ENA = (1 << channel);
}

// Update duty cycle as a ratio of: (current_period / CDTY) for PWM channel
void pwm_update_dutyCycle(enum PWM_CHANNEL channel, int CDTY) {

    int period = PWM->PWM_CH_NUM[channel].PWM_CPRD;
    PWM->PWM_CH_NUM[channel].PWM_CDTYUPD = (period / CDTY);
}

// OLD: Currently only defined for PB13 
// void pwm_init(){

//     // 1. Enable peripheral clocks for PIOB & PWM
//     PMC->PMC_PCER0 = (1 << ID_PIOB);
//     PMC->PMC_PCER1 = (1 << (ID_PWM - 32));

//     // 2. Enable peripheral mode B (disable PIO control)
//     PIOB->PIO_PDR = PB13;

//     // 3. Select perhipheral mode B (PWM)
//     PIOB->PIO_ABSR |= PB13;

//     // 4. Disable pull-up
//     PIOB->PIO_PUDR = PB13;

//     // 5. Configure PWM clock: f_pwm = CLKA / period = 50Hz 
//     // CLKA = MCK / (DIVA * 2^PREA), MCK = 84MHz, Duty Cycle = [0.9, 2.1]ms
//     PWM->PWM_CLK = PWM_CLK_PREA(3) | PWM_CLK_DIVA(8);   // PREA(3) => divide by 8
//                                                         // DIVA(8) => divide by 8

//     // 6. Configure PWM channel
//     PWM->PWM_CH_NUM[PWM_CH1].PWM_CMR = PWM_CMR_CPRE_CLKA;
//     PWM->PWM_CH_NUM[PWM_CH1].PWM_CPRD = 26250;           // Period: 50Hz = 20ms
//     PWM->PWM_CH_NUM[PWM_CH1].PWM_CDTY = 2625;            // Duty cycle: 2ms

//     // 7. Enable PWM channel 
//     PWM->PWM_ENA = (1 << PWM_CH1);
// }
