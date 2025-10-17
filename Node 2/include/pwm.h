#include "sam3xa.h"  // Header for SAM3X register definitions
#include "sam.h"
//#define PB13 (1u << 13)
//#define PWM_CH1 1

#define F_PWM(hz) = (CHIP_FREQ_CPU_MAX / 64 * hz)
//TODO: use F_PWM as pwm_init argument to set a given frequency??
//TODO: #define dutyCycle() = ??

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
void pwm_init(enum PWM_CHANNEL channel, enum portb_pin pin);
void pwm_update_dutyCycle(enum PWM_CHANNEL channel, int CDTY);