#pragma once
#include <avr/io.h>
#include <avr/iom162.h>
#include <stdint.h>

// TODO: Define correct pins
#define AVR_SS_PIN PB3
#define DISPLAY_SS_PIN PB4

void spi_init();
void spi_transmit(uint8_t data);
uint8_t spi_recveive(void);
void spi_transmit_packet(const uint8_t *data, int length);
