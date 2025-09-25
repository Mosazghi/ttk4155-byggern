#pragma once
#include <avr/io.h>
#include <avr/iom162.h>
#include <stdint.h>
#include <stdio.h>
#define SRAM_START 0x1400
#define SRAM_SIZE 0x0C00
void ext_ram_init();
void sram_write(uint16_t addr, uint8_t data);
uint8_t sram_read(uint16_t addr);
void sram_write_string(uint16_t addr, const char *data);
void sram_read_string(uint16_t addr, char *buffer, size_t max_length);
void sram_test(void);
