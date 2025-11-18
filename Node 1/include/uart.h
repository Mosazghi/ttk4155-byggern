#pragma once
#include <stdio.h>

/**
 * @brief  Initializes the UART with the specified baud rate.
 * @param ubrr The baud rate setting for the UART.
 */
void uart_init(unsigned int ubrr);
/**
 * @brief Transmits a single character over UART.
 * @param msgdata The character to transmit.
 * @param file   Standard FILE pointer for compatibility with stdio.
 * @return int   Status of the transmission.
 */
int uart_tx(char msgdata, FILE* file);
/**
 * @brief Receives a single character over UART.
 * @param file Standard FILE pointer for compatibility with stdio.
 * @return int The received character.
 */
int uart_rx(FILE* file);
