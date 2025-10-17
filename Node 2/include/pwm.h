#include "sam3xa.h"  // Header for SAM3X register definitions
#include "sam.h"
#define PB13 (1u << 13)
#define PWM_CH1 1

void pwm_init();
void pwm_update_dutyCycle(int channel, int CDTY);