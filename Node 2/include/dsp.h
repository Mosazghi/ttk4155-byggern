#include "sam3xa.h"  // Header for SAM3X register definitions
#include "sam.h"
#include <math.h>
#include <stdint.h>

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
 * @param array 
 * @return int 
 */
int spike_filter(int array[2], int threshold);

/**
 * @brief Combines median & low-pass filtering
 * 
 * @param new_value 
 * @return int 
 */
int combined_filter(int new_value);

/**
 * @brief Low-pass filter
 * 
 * @param new_value 
 * @return float 
 */
float low_pass_filter(int new_value);