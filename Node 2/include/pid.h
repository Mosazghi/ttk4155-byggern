#pragma once
#include "motor_driver.h"

typedef struct {
    float Kp, Ki;                   // Gains
    float limMin;
    float limMax;                   // Output limits

    float intlimMin;
    float intlimMax;
    float T;                        // Sample time[s]
    float setpoint;

    // Memory
    float integrator;
    float prevError;

    float output;
} PI_controller_t;

void PI_init(PI_controller_t *PI);
float PI_update(PI_controller_t *PI, float measurement);
void PI_update_setpoint(PI_controller_t *PI, float setpoint);
void update_setpoint_from_joyx(int joy_x);
void TC1_init_10ms();
void TC1_Handler();

extern volatile int32_t encoder_position;
extern int32_t target_position;
extern volatile bool new_sample_ready;