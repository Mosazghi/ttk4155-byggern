#include <stdarg.h>
#include <stdio.h>
#include <sys/types.h>

#include "adc.h"
#include "can.h"
#include "dsp.h"
#include "motor_driver.h"
#include "pwm.h"
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
#define IR_CHECK_INTERVAL_MS 500

uint32_t ir_check_prev_millis = 0;
static void can_tx_ir_blocked_msg();
static void check_ir_blocked();



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
  // REG_ADC_MR |= (1 << 0);     // Disable hardware trigger
  pwm_init(PWM_CH1, PB13, 50);
  // set pin PA0 as ADC input for IR sensor
  motor_init();
  motor_encoder_init();
  adc_init();
  TC1_init_10ms();
  PI_controller_t PI;
  encoder_zero();

  PI_init(&PI);
  int joy_x = 0;

  piob_output_init(17);
  int prev_joy_x = 0;

  while (1) {
    if (totalMsecs(time_now()) - ir_check_prev_millis >= IR_CHECK_INTERVAL_MS) {
      ir_check_prev_millis = totalMsecs(time_now());
      check_ir_blocked();
    }

    if (can_rx(&msg)) {
      can_parse_input_msg(&msg, &input);
      bool button_state = input.buttons.R1;
      joy_x =(int) low_pass_filter(input.joystick.x);
      printf(">Joy_x: %d,Joy_y: %d\r\n", joy_x, input.joystick.y);

      int x_servo = pos_to_us(input.joystick.y);


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
      //time_spinFor(msecs(10));

}

static void check_ir_blocked() {
  static bool ir_was_blocked = false;
  uint16_t ir_adc_value = adc_read();
  bool ir_detected = (ir_adc_value < 500);

  if (ir_detected && !ir_was_blocked) {
    printf("IR blocked! ADC value: %d\n", ir_adc_value);
    can_tx_ir_blocked_msg();
    ir_was_blocked = true;
  } else if (!ir_detected && ir_was_blocked) {
    ir_was_blocked = false;
  }
}

static void can_tx_ir_blocked_msg() {
  can_msg_t tx_msg = {0};
  tx_msg.id = 0x10;  // Example ID for IR blocked message
  tx_msg.length = 2;
  tx_msg.byte[0] = 1;  // Indicate blocked
  tx_msg.byte[1] = 0;  // Reserved

  can_tx(tx_msg);
}

