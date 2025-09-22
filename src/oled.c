#include <oled.h>
#include "fonts.h"


const uint8_t a[] = {0b01111100,0b01111110,0b00010011,0b00010011,0b01111110,0b01111100,0b00000000,0b00000000};
const uint8_t a_normal[] = {0b01111110,0b00010001,0b00010001,0b00010001,0b01111110};
const uint8_t a_small[] = {0b01111000,0b00010100,0b01111000,0b00000000};
const uint8_t s[] = {0b00100110,0b01100111,0b01001101,0b01011001,0b01111011,0b00110010,0b00000000,0b00000000};

static void oled_write(uint8_t data) {
    PIN_WRITE(PORTB, OLED_CS, LOW); 
    LOG_DBG("Transmitting from oled_write: %#x", data); 
    spi_transmit(data); 
}

static void oled_write_data_packet(const uint8_t *data, int size) {
    PIN_WRITE(PORTB, OLED_CS, LOW); 
    PIN_WRITE(PORTB, OLED_CMD, HIGH); 
    spi_transmit_packet(data, size); 
}

static void oled_write_command(uint8_t data) {
    PIN_WRITE(PORTB, OLED_CMD, LOW);
    oled_write(data); 
}

static void oled_write_data(uint8_t data) {
    PIN_WRITE(PORTB, OLED_CMD, HIGH);
    oled_write(data); 
}

int oled_init(void) {
    DDRB |= (1 << PB2); 
    PORTB &= ~(1 << PB3); // Display CS 
    PORTB &= ~(1 << PB2); // OLED cmd display 
    uint8_t oled_init_array[] = {
            0xAE,   // display off
            0xA1,   // segment remap
            0xDA,   // common pads hardware: alternative
            0x12,   //
            0xC8,   // common output scan direction: com63-com0
            0xA8,   // multiplex ratio mode: 63
            0x3F,   //
            0xD5,   // dispay division ratio / osc. freq. mode
            0x80,
            0x81,// contrast control
            0xFF,   //
            0xD9,   // set pre-charge preiod
            0x21,   // 
            0x20,   // set addressing mode
            0x02,   // enable page mode
            0xDB,   // VCOM deselect level mode
            0x30, //
            0xAD,   // master configuration
            0x00,   // 
            0xA4,   // out follows RAM content
            0xA6,   // set normal display
            0xAF    // display on
        };

    spi_transmit_packet(oled_init_array, ARRAY_LENGTH(oled_init_array));
    PORTB  |= (1 << PB2); // Display cmd 
    PORTB  |= (1 << PB3); // Display CS
                // 000100 (1 << PB4)
                //
                //  000100 (1 << PB4)
                //| 001000 (PORTB)
                //  001000
    oled_clear(); 

    return 0; // Return 0 on success
}

void oled_clear(void) {
    for (int i = 0; i < 8; i++) {
        oled_write_command(0xB0 + i); // Set page adress to page0 
        oled_write_command(0x00); // set oled lower column line
        oled_write_command(0x10); // Set oled higher column line 
        for(int j = 0; j < 128; j++) {
            oled_write_data(0x00); // writing blank to each page
        }
       
    }
}

void oled_print() {
    oled_write_command(0xB0); // Set page adress to page0 
    oled_go_to_column(0);
    oled_write_data_packet(a, ARRAY_LENGTH(a)); 
    oled_write_command(0xB3); // Set page adress to page0 
    oled_go_to_column(26);
    oled_write_data_packet(a_normal, ARRAY_LENGTH(a_normal)); 
    oled_write_command(0xB6); // Set page adress to page0 
    oled_go_to_column(64);
    oled_write_data_packet(a_small, ARRAY_LENGTH(a_small)); 
    // oled_write_command(0xB1); // Set page adress to page0 
    // oled_go_to_column(0);
    // oled_write_data_packet(s, ARRAY_LENGTH(s)); 
}

void oled_go_to_column(int column)
{
    oled_write_command(0x00 + (column % 16));
    oled_write_command(0x10 + (column / 16));
}
