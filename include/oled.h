#pragma once
#include <string.h>

#include "spi.h"
#include "sram.h"
#include "utility.h"
#define OLED_CS PB3
#define OLED_CMD PB2
#define SEG_WIDTH 127
#define PAGE_HEIGHT 7
#define ADDR_START 0x1400  // start of sram (1kb)
#define ADDR_END 0x1800    // end of sram (2kb)

typedef enum { CMD, DATA } oled_write_mode_t;
typedef enum { SMALL = 4, MEDIUM = 5, LARGE = 8 } oled_font_size_t;

typedef struct {
  oled_font_size_t font_size;
} oled_ctx;

int oled_init(void);

/* commands */
void oled_go_to_column(int column);
void oled_set_font(oled_font_size_t font_size);
void oled_clear(void);

/* Graphics */
void oled_printf(const char* str, int x, int y);
void oled_display();