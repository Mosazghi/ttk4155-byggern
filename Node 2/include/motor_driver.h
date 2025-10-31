#pragma once
#include "pwm.h"
#include "input.h"
#define PC23 (1u << 23)
#define PC25 (1u << 25)
#define PC26 (1u << 26)
#define ENCODER_MIN 0
#define ENCODER_MAX 5596

enum motor_direction {
    LEFT,
    RIGHT
};

/**
 * @brief Initiates motor driver to output PWM channel 0 on pin PB13 with 1000Hz.
 * 
 * DIR initiated to pin PC23.
 * 
 */
void motor_init();

/**
 * @brief Sets PC23 high or low to change motor direction.
 * 
 * LEFT = low, RIGHT = high.
 * 
 * @param dir 
 */
void motor_set_dir(enum motor_direction dir);

/**
 * @brief Reads uint8_t value and returns direction.
 * 
 * Value < 128 returns 0, value >= 128 returns 1.
 * 
 * @param value 
 * @return uint8_t 
 */
uint8_t motor_get_dir(uint8_t value);

/**
 * @brief Reads joystick.x value and returns direction.
 * 
 * joy_x < 0 returns 0 (LEFT).
 * 
 * joy_x >= 0 returns 1 (RIGHT).
 * 
 * @param joy_x 
 * @return int 
 */
int joy_get_dir(int joy_x);

/**
 * @brief Sets the duty cycle of the motor equal to the absolute value of the joystick value.
 * 
 * Includes deadzone hardcoded to 5.
 * 
 * @param joy_x 
 */
void motor_set_speed(int joy_x);

/**
 * @brief Initializes the motor encoder using TC2, channel 6(0). 
 * 
 * Externally driven clock: XC0 (TCLK0).
 * 
 * Channel A: PC25 (TIOA6)
 * 
 * Channel B: PC26 (TIOB6)
 * 
 */
void motor_encoder_init();

/**
 * @brief Reads encoder value from TC_CV (channel 0).
 * 
 * @return int32_t 
 */
int32_t encoder_get_position(void);