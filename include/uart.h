#include <stdio.h>
#include <avr/io.h>

void uart_init(unsigned int baud_rate);
int uart_tx(char msgdata, FILE * file);
int uart_rx(FILE * file);
