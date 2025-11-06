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
#include "pid.h"

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
  printf("version 7\n\r");

  piob_output_init(13);
  pwm_init(PWM_CH1, PB13, 50);
  motor_init();
  motor_encoder_init();
  adc_init();
  TC1_init_10ms();
  PI_controller_t PI;
  encoder_zero();

  PI_init(&PI);
  int joy_x_raw = 0;
  int joy_x = 0;

  piob_output_init(17);
  int prev_joy_x = 0;
  // int arr[2];
  // int idx = 0; 
  while (1) {
    if (can_rx(&msg)) {
      can_parse_input_msg(&msg, &input);
      bool button_state = input.buttons.R1;
      joy_x_raw = input.joystick.x;
      joy_x =(int) low_pass_filter(joy_x_raw);


      int x_tp = remap(input.touch_pad.x);
      //printf("x_tp: %d", x_tp);
      // arr[idx] = x_tp;
      // idx += 1; 
      // if(idx == 2) {
      // int x_tp_spike = spike_filter(arr, 20);  // TODO: make && test this @ torsdag.
      
      int x_servo = pos_to_us(x_tp);
      //u_int8_t t_dir = motor_get_dir(input.joystick.x);
      //int32_t encoder_value = encoder_get_position();
      //printf("Encoder value: %d\n", encoder_value);
      //printf("dir: %d -> tp.x = %d -> remapped to: %d -> PWM = %d[us]\n", t_dir, input.touch_pad.x, x_tp, x_servo);
      //printf("x_tp: %d -> Spike_filter: %d", x_tp, x_tp_spike);
      //motor_set_dir(t_dir);
      //printf("X: %d -> %d us\n\r", input.joystick.x, x_us);

      if (button_state == 0) {
        piob_set_pin_high(17);
      }
      else {
        piob_set_pin_low(17);
      }


      pwm_set_pulseWidth(PWM_CH1, x_servo, 50);
      //idx =0;
    }

    PI_control(&PI, joy_x);
    // After testing the if statement below, the following function should handle all PI-control.
    //PI_control(&PI, joy_x);
    
    // if (new_sample_ready) {
    //   new_sample_ready = false;

    //   int32_t position = encoder_position;

    //   update_target_pos(joy_x);
    //   PI_update_setpoint(&PI, target_position);

    //   PI_update(&PI, (double)position);

    }
    //printf("L");
    //time_spinFor(msecs(10));
  }

