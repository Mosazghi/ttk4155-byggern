#pragma once

#include <avr/io.h>
#include <avr/iom162.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#define SRAM_START 0x1400
#define SRAM_SIZE 0x0C00

/**
 * @brief Initializes the external SRAM
 */
void ext_ram_init();

/**
 * @brief Writes a byte to the specified SRAM address
 * @param addr The address to write to
 * @param data The data byte to write
 */
void sram_write(uint16_t addr, uint8_t data);

/**
 * @brief Reads a byte from the specified SRAM address
 * @param addr The address to read from
 * @return The data byte read
 */
uint8_t sram_read(uint16_t addr);

/**
 * @brief Writes a null-terminated string to SRAM starting at the specified address
 * @param addr The starting address to write the string
 * @param data The null-terminated string to write
 */
void sram_write_string(uint16_t addr, const char *data);

/**
 * @brief Reads a string from SRAM starting at the specified address
 * @param addr The starting address to read the string
 * @param buffer The buffer to store the read string
 * @param max_length The maximum length of the string to read
 */
void sram_read_string(uint16_t addr, char *buffer, size_t max_length);

/**
 * @brief Reads a packet of data from SRAM starting at the specified address
 * @param addr The starting address to read the packet
 * @param buffer The buffer to store the read data
 * @param size The size of the data packet to read
 */
void sram_read_packet(uint16_t addr, uint8_t *buffer, size_t size);
/**
 * @brief Writes a packet of data to SRAM starting at the specified address
 * @param addr The starting address to write the packet
 * @param data The data packet to write
 * @param size The size of the data packet to write
 */
void sram_transmit_packet(uint16_t addr, const uint8_t *data, size_t size);
/**
 * @brief Tests the SRAM functionality
 * @return true if the test passes, false otherwise
 */
bool sram_test(void);
