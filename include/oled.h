#pragma once
#include "spi.h"
#include "utility.h"
#define OLED_CS PB3
#define OLED_CMD PB2

int oled_init(void);
void oled_clear(void);
void oled_print();
void oled_printf(const char* str);
void oled_go_to_column(int column);
void oled_update();