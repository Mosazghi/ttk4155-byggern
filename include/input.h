#pragma once

#include "adc.h"

typedef struct
{
    uint8_t x;
    uint8_t y;
    uint8_t btn;
} joystick_xy_t;

typedef enum
{
    LEFT,
    RIGHT,
    UP,
    DOWN,
    NEUTRAL
} joystick_dir_t;

typedef struct
{
    uint8_t x;
    uint8_t y;
} touch_pad_xy_t;

typedef struct ADC
{
    struct joystick;
    struct touch_pad;
};

ADC adc_read_all();