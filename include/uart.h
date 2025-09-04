#include <stdio.h>

void uart_init(unsigned int ubrr);
int uart_tx(char msgdata, FILE * file);
int uart_rx(FILE * file);
