#include <avr/iom162.h>
#include <stdio.h>
#include <avr/io.h>
#include <stdint.h>
#include "sram.h"


void xmem_init(){
    MCUCR |= (1 << SRE);        // Enable external memory
    SFIOR |= (1 << XMM2);       // Mask PC4-PC7, used for JTAG
}

void sram_write(uint16_t addr, uint8_t data){
    volatile uint8_t *xmem = (uint8_t *) 0x1800; // SRAM start address
    xmem[addr] = data;                           // Write data to SRAM
}

uint8_t sram_read(uint16_t addr){
    volatile uint8_t *xmem = (uint8_t *) 0x1800; // SRAM start address
    return xmem[addr];                           // Read data from SRAM
}
