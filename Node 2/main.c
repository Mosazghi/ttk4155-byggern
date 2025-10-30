#include <stdarg.h>
#include <stdio.h>
#include <sys/types.h>

#include "can.h"
#include "sam.h"
#include "servo.h"
#include "time.h"
#include "uart.h"
#include "motor_driver.h"
#include "dsp.h"
#include "adc_internal.h"

#define BAUDRATE 115200
#define F_CPU 84000000

can_init_t can_config = {
    .brp = 84,  // Baud Rate Prescaling (83)
    .phase1 = 6,
    .phase2 = 6,
    .propag = 3,
    .sjw = 1,
    .smp = 1,
    .interrupt = false,
};

int main() {
  SystemInit();

  WDT->WDT_MR = WDT_MR_WDDIS;  // Disable Watchdog Timer
  uart_init(F_CPU, BAUDRATE);
  can_init(can_config);
  can_msg_t msg = {0};
  input_t input = {0};
  printf("version 5\n\r");

  piob_output_init(13);
  pwm_init(PWM_CH1, PB13, 50);
  motor_init();
  motor_encoder_init();
  adc_init();
  
  int arr[2];
  int idx = 0; 
  while (1) {
    if (can_rx(&msg)) {
      can_parse_input_msg(&msg, &input);
      int x_ys = pos_to_us(input.joystick.x);
      int x_tp = remap(input.touch_pad.x);
      arr[idx] = x_tp;
      idx += 1; 
      if(idx == 2) {

      
      int x_tp_spike = spike_filter(arr, 20);  // TODO: make && test this @ torsdag.
      
      int x_servo = pos_to_us(x_tp);
      u_int8_t t_dir = motor_get_dir(input.touch_pad.x);
      printf("dir: %d -> tp.x = %d -> remapped to: %d -> PWM = %d[us]\n", t_dir, input.touch_pad.x, x_tp, x_servo);
      printf("x_tp: %d -> Spike_filter: %d", x_tp, x_tp_spike);
      motor_set_dir(t_dir);





      //printf("X: %d -> %d us\n\r", input.joystick.x, x_ys);
      pwm_set_pulseWidth(PWM_CH1, x_servo, 50);
      idx =0;
    }
    }
    time_spinFor(msecs(10));
  }
}
