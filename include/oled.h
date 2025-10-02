#pragma once
#define OLED_CS PB3
#define OLED_CMD PB2
#define SEG_WIDTH 127
#define PAGE_HEIGHT 7
#define ADDR_START 0x1400  // start of sram (1kb)
#define ADDR_END 0x1800    // end of sram (2kb)
#define OLED_WIDTH 128
#define OLED_HEIGHT 64

typedef enum { CMD, DATA } oled_write_mode_t;
typedef enum { SMALL = 4, MEDIUM = 5, LARGE = 8 } oled_font_size_t;

typedef struct {
  oled_font_size_t font_size;
} oled_ctx;

/**
 * @brief Initilazes the OLED IC for the ssd1306
 */
int oled_init(void);

/****************************************************************
 *                       COMMAND FUNCTIONS                       *
 ****************************************************************/

/**
 * @brief Sets the font used by @see oled_printf(...)
 *
 * The function will update the context of the oled and update
 * which font is used when using @see oled_printf(...)
 *
 * @param font_size (SMALL, MEDIUM, LARGE) from enum
 */
void oled_set_font(oled_font_size_t font_size);

/**
 * @brief Clears the sram and oled buffers
 */
void oled_clear(void);

/****************************************************************
 *                      GRAPHICS FUNCTIONS                       *
 ****************************************************************/

/**
 *  @brief Draw's strings onto the oled
 *
 *  This function will store ASCII letters in different fonts onto
 *  the back-buffer. To transfer and display the string use
 *  @see oled_display(). The x and y values are CALMPED.
 *
 *  @param str generic string
 *  @param x  horisontal position (0-128)
 *  @param y  vertical position (0-7) page based positioning
 */
void oled_printf(const char* str, int x, int y);

/**
 * @brief Fetches data from back-buffer and displays it
 *
 * This function fetches the back-buffer from the sram and stores it
 * on the local ram causing it to display it onto the oled
 */
void oled_display();

/**
 * @brief Draws a single pixel onto the oled
 *
 *  This function draws a single pixel onto the display based on
 *  the pixel dimmension (128x64) which are set by the macros
 *  OLED_WIDTH and OLED_HEIGHT. The x and y values are CLAMPED.
 *  To transfer and display the pixel use @see oled_display().
 *
 * @param x  horisontal position (0-128)
 * @param y  vertical position (0-64)
 */
void oled_draw_pixel(int x, int y);

/**
 * @brief Draws a line based on start and end point
 *
 * This function draws a line using the Bresenham's line algorithm
 * to calculate a line using integer arithmetic. The line is built
 * from @see oled_draw_pixel(..). To transfer and display the line
 * use @see oled_display().
 */
void oled_draw_line(int x_start, int y_start, int x_end, int y_end);