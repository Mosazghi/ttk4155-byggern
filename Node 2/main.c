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
  printf("version 5\n\r");

  piob_output_init(13);
  // REG_ADC_MR |= (1 << 0);     // Disable hardware trigger
  pwm_init(PWM_CH1, PB13, 50);
  // set pin PA0 as ADC input for IR sensor
  motor_init();
  motor_encoder_init();
  adc_init();

  int arr[2];
  int idx = 0;
  while (1) {
    if (totalMsecs(time_now()) - ir_check_prev_millis >= IR_CHECK_INTERVAL_MS) {
      ir_check_prev_millis = totalMsecs(time_now());
      check_ir_blocked();
    }

    if (can_rx(&msg)) {
      can_parse_input_msg(&msg, &input);
      int x_ys = pos_to_us(input.joystick.x);
      int x_tp = remap(input.touch_pad.x);
      // printf("tp.x = %d -> remapped to: %d\n", input.touch_pad.x, x_tp);
      // arr[idx] = x_tp;
      // idx += 1;
      // if (idx == 2) {
      // int x_tp_spike = spike_filter(arr, 20);  // TODO: make && test this @ torsdag.

      int x_servo = pos_to_us(x_tp);
      uint8_t t_dir = motor_get_dir(input.touch_pad.x);
      // printf("dir: %d -> tp.x = %d -> remapped to: %d -> PWM = %d[us]\n", t_dir,
      // input.touch_pad.x,
      //        x_tp, x_servo);
      // printf("x_tp: %d -> Spike_filter: %d", x_tp, x_tp_spike);
      motor_set_dir(t_dir);

      // printf("X: %d -> %d us\n\r", input.joystick.x, x_ys);
      // pwm_set_pulsewidth(PWM_CH1, x_servo, 50);
      pwm_set_pulseWidth(PWM_CH1, x_servo, 50);
      // idx = 0;
      // }
    }

    time_spinFor(msecs(10));
  }
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
