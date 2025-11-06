#pragma once
#include <math.h>
#include <stdint.h>

#define CLAMP(value, min, max) ((value) < (min) ? (min) : ((value) > (max) ? (max) : (value)))


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

long map(long x, long in_min, long in_max, long out_min, long out_max);
