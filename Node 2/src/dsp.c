#include "dsp.h"

int filter_joystick(int stick_value)
{
    static int buffer[n_filter];
    static int sum = 0;
    static int index = 0;

    sum -= buffer[index];      // remove oldest
    buffer[index] = stick_value; // store newest
    sum += stick_value;

    index = (index + 1) % n_filter;

    return (sum / n_filter);
}
