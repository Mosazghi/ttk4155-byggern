#pragma once

#include "adc.h"

struct joystick
{
    uint8_t x;
    uint8_t y;
    uint8_t btn;
};
struct touch_pad
{
    uint8_t x;
    uint8_t y;
};
typedef struct ADC
{
    struct joystick;
    struct touch_pad;
};

ADC adc_read_all();