#include <stdio.h>
#include <avr/io.h>
#include "uart.h"

void uart_init(unsigned int baud_rate){
    // TODO Finn register for:
    // Set baud rate 
    // set antall data bits = 8?
    // set antall parity bits = 0?
    // set antall stop bits = 1? 2?
    fdevopen(uart_tx, uart_rx);
}

// Transmit data function. Including polling for TX ready
int uart_tx(char msgdata, FILE * file){
    while (!(/* TODO Finn register for: TX ready */ & (1 << /*Flag bit for empty buffer*/)));
    //     /* TODO Finn register for: TX data */ = msgdata;
    return 0;
}

// Receive data function. Including polling for RX ready
int uart_rx(FILE * file){
    // while (!(/* TODO Finn register for: RX ready */ & (1 << /*Flag bit for data in buffer*/)));
    return 0; /* TODO Finn register for: RX data */
}

