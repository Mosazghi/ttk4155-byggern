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

/**
 * @brief Initiates PI-controller with most default values set to 0. 
 * 
 * @param PI 
 */
void PI_init(PI_controller_t *PI);

/**
 * @brief Integrates the error and calculates output based on current setpoint and encoder value.
 * 
 * To update the setpoint value, use PI_update_setpoint instead.
 * 
 * @param PI 
 * @param measurement 
 * @return float 
 */
float PI_update(PI_controller_t *PI, float measurement);

/**
 * @brief Updates the internal setpoint value of the PI-controller.
 * 
 * @param PI 
 * @param setpoint 
 */
void PI_update_setpoint(PI_controller_t *PI, float setpoint);

/**
 * @brief Updates global setpoint 'target_position' based by adding joystick.x value to the current value. 
 * 
 * Includes deadzone filtration (currently hardcoded to: 5).
 * 
 * Does not actuate the motor by itself.
 * 
 * @param joy_x 
 */
void update_target_pos(int joy_x);

/**
 * @brief Initiates 100hz TC1, CH0 timer that triggers an ISR every 10ms.
 * 
 */

void TC1_init_10ms();
/**
 * @brief ISR for TC1, channel 0. Stores encoder value as int32_t in encoder_position.
 * 
 */
void TC3_Handler();

/**
 * @brief Groups all PI-functionality into one meta-function for ease of use. 
 * 
 * Includes motor actuation. Sets motor direction based on the sign of the error term.
 * 
 * @param PI 
 * @param joy_x 
 */
void PI_control(PI_controller_t *PI, int joy_x);
extern volatile int32_t encoder_position;
extern int32_t target_position;
extern volatile bool new_sample_ready;