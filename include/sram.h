#include <avr/iom162.h>
#include <stdio.h>
#include <avr/io.h>
#include <stdint.h>

void ext_ram_init();
void sram_write(uint16_t addr, uint8_t data);
uint8_t sram_read(uint16_t addr);
void sram_test(void);