#include "uart.h"

#include <avr/io.h>
#include <stdio.h>

#include "utility.h"

// Initialize uart
void uart_init(unsigned int ubrr) {
  // Set baud rate
  UBRR0H = (unsigned char)(ubrr >> 8);  // bit 8-11 (MSB) USART Baud Rate Register
  UBRR0L = (unsigned char)ubrr;         // bit 0-7 USART Baud Rate Register

  // Enable receiver RXEN0  and transmitter TXEN0
  UCSR0B |= (1 << RXEN0) | (1 << TXEN0);  // USCR0B |= 0x18b;
  // URSEL: Toggles between UCSRC0 & UBRRH registers
  // Set character size: USCZ2 = 0 + USCZ1 = 1 + USCZ0 = 1 -> 8-bit
  // Set stop bits: USBS=1: 2 stop bits
  UCSR0C = (1 << URSEL0) | (1 << USBS0) | (3 << UCSZ00);

  fdevopen(uart_tx, uart_rx);
}

// Transmit data function. Including polling for empty TX buffer
int uart_tx(char msgdata, FILE *file) {
  UNUSED(file);
  while (!(UCSR0A & (1 << UDRE0)));  // UDRE0 = 1 -> Transmit buffer empty & ready
  UDR0 = msgdata;                    // UDR0 = USART0 I/O Data Register
  return 0;
}

// Receive data function. Including polling for data received.
int uart_rx(FILE *file) {
  UNUSED(file);

  while (!(UCSR0A & (1 << RXC0)));  // RXC0 = 0 -> No unread data in buffer
  return UDR0;
}
