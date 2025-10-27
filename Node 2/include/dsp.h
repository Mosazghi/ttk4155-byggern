#include "sam3xa.h"  // Header for SAM3X register definitions
#include "sam.h"
#include <math.h>
#include <stdint.h>
#define n_filter 3  // box filter coefficients 

/**
 * @brief Returns the average of the last 'n_filter' samples.
 * 
 * @param stick_value 
 * @return int 
 */
int filter_joystick(int stick_value);