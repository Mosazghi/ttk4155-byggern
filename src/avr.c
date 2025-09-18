#include <avr/io.h>
#include "avr.h"
#include "spi.h"
#define F_CPU 4915200UL
#include <util/delay.h>

static void avr_write(uint8_t data)
{
    PORTB ^= (1 << PB4); // select AVR
    spi_transmit(data);  // Read AVR info
    // PORTB ^= (1 << PB4); // select AVR
}

joystick_xy_t avr_get_joystick()
{
    joystick_xy_t joystick;
    avr_write(0x03); // Command to read joystick data
    _delay_us(40);
    joystick.x = spi_recveive();
    _delay_us(2);
    joystick.y = spi_recveive();
    _delay_us(2);
    joystick.btn = spi_recveive();
    PORTB ^= (1 << PB4); // Deselect AVR
    return joystick;
}