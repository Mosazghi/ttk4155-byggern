#include <sys/types.h>

#include "sam.h"
#define F_CPU 84000000
#include <stdarg.h>
#include <stdio.h>

#include "can.h"
#include "time.h"
#include "uart.h"
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
  printf("version 3\n\r");

  while (1) {
    if (can_rx(&msg)) {
      can_parse_input_msg(&msg, &input);
      printf("joystick x:%d y:%d\n\r", input.joystick.x, input.joystick.y);
    }
    time_spinFor(msecs(10));
  }
}
