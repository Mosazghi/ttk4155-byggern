#pragma once
#include <stdio.h>

/**
 * @brief  Initializes the UART with the specified baud rate.
 * @param ubrr The baud rate setting for the UART.
 */
void uart_init(unsigned int ubrr);
int uart_tx(char msgdata, FILE* file);
int uart_rx(FILE* file);
