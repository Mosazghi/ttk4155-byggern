#pragma once
#include "motor_driver.h"

typedef struct {
    double Kp, Ki;                   // Gains
    double limMin;
    double limMax;                   // Output limits

    double intlimMin;
    double intlimMax;
    double T;                        // Sample time[s]
    double setpoint;

    // Memory
    double integrator;
    double prevError;

    double output;
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
 * @return double 
 */
double PI_update(PI_controller_t *PI, double measurement);

/**
 * @brief Updates the internal setpoint value of the PI-controller.
 * 
 * @param PI 
 * @param setpoint 
 */
void PI_update_setpoint(PI_controller_t *PI, double setpoint);

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