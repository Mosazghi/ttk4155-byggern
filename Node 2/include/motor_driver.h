#include "pwm.h"
#define PC23 (1u << 23)

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
 * @brief Reads value of touchpad.x and returns direction.
 * 
 * Value < 128 returns 0, value >= 128 returns 1.
 * 
 * @param touchpad 
 * @return uint8_t 
 */
uint8_t motor_get_dir(touch_pad_t *touchpad);