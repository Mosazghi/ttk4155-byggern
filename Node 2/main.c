#include "sam.h"
#define F_CPU 84000000
#include <stdarg.h>
#include <stdio.h>

/*
 * Remember to update the Makefile with the (relative) path to the uart.c file.
 * This starter code will not compile until the UART file has been included in
 the Makefile.
 * If you get somewhat cryptic errors referencing functions such as _sbrk,
 * _close_r, _write_r, _fstat etc, you have most likely not done that correctly.

 * If you get errors such as "arm-none-eabi-gcc: no such file", you may need to
 reinstall the arm gcc packages using
 * apt or your favorite package manager.
 */
// #include "../path_to/uart.h"
#include "can.h"
#include "time.h"
#include "uart.h"
#define BAUDRATE 115200
#define F_CPU 84000000

// CanInit can_setup = {
// .brp = 5,
// .phase1 = 2,
// .phase2 = 2,
// .propag = 1,
// .sjw = 2,
// };
CanInit can_config = {.brp = 84,  // Baud Rate Prescaling (83)
                      .phase1 = 6,
                      .phase2 = 6,
                      .propag = 3,
                      .sjw = 1,
                      .smp = 1};
int main() {
  SystemInit();

  int x = 0;
  WDT->WDT_MR = WDT_MR_WDDIS;  // Disable Watchdog Timer
  uart_init(F_CPU, BAUDRATE);
  can_init(can_config, 0);

  CanMsg msg = {
      .id = 200,
      .byte = "hello",
      .length = 8,
  };

  printf("version 2\n\r");
  // int x = 0;

  while (1) {
    // if (can_rx(&msg)) {
    // printf("msg: %s", msg.byte);

    // msg.byte[0] = 0x10;
    // msg.byte[1] = 0x11;

    can_tx(msg);
    printf("sendt message id: %d\n\r", msg.id);

    printf("LFG x = %d\n\r", x);
    x++;
    for (volatile int i = 0; i < 1000000; i++);
    time_spinFor(msecs(100));
  }
}
