#include "pid.h"
#define CONTROL_HZ   100             // 100 Hz = 10 ms
#define CONTROL_T    (1.0f / CONTROL_HZ)

volatile int32_t encoder_position = 0;
int32_t target_position = 0;
volatile bool new_sample_ready = false;

void TC1_init_10ms(void)
{
    PMC->PMC_PCER0 = (1 << ID_TC3);  // Enable clock for TC1 Channel 0 (TC3? 1st channel of TC1)

    // MCK = 84 MHz → TIMER_CLOCK4 = MCK/128 = 656 250 Hz
    TC1->TC_CHANNEL[0].TC_CMR =
        TC_CMR_TCCLKS_TIMER_CLOCK4 | // MCK/128
        TC_CMR_WAVE |                // Waveform mode
        TC_CMR_WAVSEL_UP_RC;         // Count up to RC then reset

    TC1->TC_CHANNEL[0].TC_RC = 6562; // 656 250 / 6562 ≈ 100 Hz

    TC1->TC_CHANNEL[0].TC_IER = TC_IER_CPCS;  // interrupt on RC compare
    NVIC_EnableIRQ(TC3_IRQn);

    TC1->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG; // enable + reset
}

void PI_init(PI_controller_t *PI){
    PI->Kp = 2.5f;
    PI->Ki = 2.0f;
    PI->integrator = 0.0f;
    PI->prevError = 0.0f;
    PI->output = 0.0f;
    PI->limMax = ENCODER_MAX;
    PI->limMin = ENCODER_MIN;
    PI->intlimMax = ENCODER_MAX/10;
    PI->intlimMin = ENCODER_MIN/10;
    PI->T = CONTROL_T;                     
    PI->setpoint = 0.0f;
}


float PI_update(PI_controller_t *PI, float measurement){
    float error = PI->setpoint - measurement;
    float proportional = PI->Kp*error;
    PI->integrator = PI->integrator + (0.5f * PI->T * PI->Ki
                                    * (error + PI->prevError));
    
    // Anti-windup
    if (PI->integrator > PI->intlimMax) PI->integrator = PI->intlimMax;
    if (PI->integrator < PI->intlimMin) PI->integrator = PI->intlimMin;

    PI->output = proportional + PI->integrator;
    if (PI->output > PI->limMax) {
        PI->output = PI->limMax;
    }
    if (PI->output < PI->limMin) {
        PI->output = PI->limMin;
    }

    PI->prevError = error;

    return PI->output;
}

void TC1_Handler(void)
{
    TC1->TC_CHANNEL[0].TC_SR;                   // clear interrupt flag

    encoder_position = encoder_get_position();
    new_sample_ready = true;                   // new sample is ready
}

void update_setpoint_from_joyx(int joy_x) {
    const int DEADZONE = 5;
    if (abs(joy_x) < DEADZONE) joy_x = 0;

    int delta_p = joy_x / 10;       // Scales position value to speed of change in position
    
    target_position += delta_p;
}

void PI_update_setpoint(PI_controller_t *PI, float setpoint){
    PI->setpoint = setpoint;
}