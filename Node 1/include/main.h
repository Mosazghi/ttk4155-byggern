#pragma once
#include <stdint.h>
#define F_CPU 4915200UL
#define BAUD 9600
#define MY_UBRR F_CPU / 16 / BAUD - 1
#ifndef __AVR_ATmega162__
#define __AVR_ATmega162__
#endif
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#include "game_logic.h"
#include "game_menu.h"
#include "mcp2515.h"
#include "oled.h"
#include "spi.h"
#include "sram.h"
#include "uart.h"
#include "utility.h"

#define OLED_SS_PIN PB3
typedef enum {
  STATE_MENU,
  STATE_GAME,
} system_state_t;
