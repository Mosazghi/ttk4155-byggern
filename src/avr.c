#include <avr/io.h>
#include "avr.h"
#include "spi.h"
#define F_CPU 4915200UL
#include <util/delay.h>
#include "utility.h"
static long map(long x, long in_min, long in_max, long out_min, long out_max)
{

    long ret = (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    return ret;
}
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
    long x = map((long)joystick.x, 54, 201, -100, 100);
    long y = map((long)joystick.y, 54, 201, -100, 100);
    LOG_INF("After mapping: x = %d, y = %d", x, y);
    joystick.x = (int8_t)x;
    joystick.y = (int8_t)y;
    return joystick;
}