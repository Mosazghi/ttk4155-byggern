#include "pwm.h"
#include "dsp.h"
// PWM_init for PWM_channel, PortB_pin, for given frequency, 10% DC
void pwm_init(enum PWM_CHANNEL channel, enum portb_pin pin, int Hz) {
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
  PWM->PWM_CLK = PWM_CLK_PREA(3) | PWM_CLK_DIVA(8);  // PREA(3) => divide by 8
                                                     // DIVA(8) => divide by 8

  // 6. Configure PWM channel
  PWM->PWM_CH_NUM[channel].PWM_CMR = PWM_CMR_CPOL | PWM_CMR_CPRE_CLKA;
  PWM->PWM_CH_NUM[channel].PWM_CPRD = F_PWM(Hz);         // Period(50hz) = 26250
  PWM->PWM_CH_NUM[channel].PWM_CDTY = (F_PWM(Hz) / 10);  // Default duty cycle = 10%
  // PWM->PWM_CH_NUM[channel].PWM_CMR
  // PWM->PWM_CH_NUM[channel].PWM_CPRD = 26250;
  // PWM->PWM_CH_NUM[channel].PWM_CDTY = 2625;

  // 7. Enable PWM channel
  PWM->PWM_ENA = (1 << channel);
}

// Sets PWM duty cycle as % of current period
void pwm_set_dutyCycle(enum PWM_CHANNEL channel, int CDTY) {
  int period = PWM->PWM_CH_NUM[channel].PWM_CPRD;
  PWM->PWM_CH_NUM[channel].PWM_CDTYUPD = ((period * CDTY) / 100);
}

// Sets PWM duty cycle as function of desired pulse width[us] & Hz
void pwm_set_pulseWidth(enum PWM_CHANNEL channel, int pulsewidth, int Hz) {
  pulsewidth = CLAMP(pulsewidth, SERVO_MIN, SERVO_MAX);  // pw clamped to [1000, 2000]us
  uint32_t period = PWM->PWM_CH_NUM[channel].PWM_CPRD;   // Reads current period in ticks
  PWM->PWM_CH_NUM[channel].PWM_CDTYUPD = ((uint32_t)(pulsewidth * period * Hz) / 1e6);
}
