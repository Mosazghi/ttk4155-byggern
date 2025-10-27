#include "sam3xa.h"  // Header for SAM3X register definitions
#include "sam.h"
#include <math.h>
#include <stdint.h>
#define n_filter 3  // box filter coefficients 

/**
 * @brief Returns the average of the last 'n_filter' samples.
 * 
 * @param value 
 * @return int 
 */
int box_filter(int value);

/**
 * @brief Checks if input value is within a threshold range of previous value. 
 * 
 * Returns previous value if within threshold.
 * 
 * @param value 
 * @return int 
 */
int spike_filter(int value, int threshold);