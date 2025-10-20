#include "sam3xa.h"  // Header for SAM3X register definitions
#include "sam.h"
#include "servo.h"

#define CLKA (CHIP_FREQ_CPU_MAX / 64)
#define F_PWM(hz)(CLKA / hz)        // Corresponds to ticks, not actual frequecy
#define PWM_PERIOD_20_US 20000

enum PWM_CHANNEL{
    PWM_CH0 = 0,
    PWM_CH1,
    PWM_CH2,
    PWM_CH3,
    PWM_CH4,
    PWM_CH5,
    PWM_CH6,
    PWM_CH7
};

enum portb_pin{
    PB12 = (1u << 12),  // High - D12
    PB13 = (1u << 13),  // High - D21
    PB14 = (1u << 14),  // High - D22
    PB15 = (1u << 15),  // High - D23
    PB16 = (1u << 16),  // Low  - D24
    PB17 = (1u << 17),  // Low  - D25
    PB18 = (1u << 18),  // Low  - D26
    PB19 = (1u << 19)   // Low  - D27
};
void pwm_init(enum PWM_CHANNEL channel, enum portb_pin pin, int Hz);

/**
 * @brief WARNING: Do NOT use with servos.\n
 * @brief Sets duty cycle as % of current period.
 * 
 * @param channel PWM channel
 * @param CDTY Duty cycle in % of period
 */
void pwm_set_dutyCycle(enum PWM_CHANNEL channel, int CDTY);

/**
 * @brief Sets duty cycle as a function of pulse width. Safe to use with servos.
 * 
 * @param channel PWM channel
 * @param pulsewidth [1000, 2000]us
 * @param Hz PWM frequency
 */
void pwm_set_pulseWidth(enum PWM_CHANNEL channel, int pulsewidth, int Hz);