#include "dsp.h"

// Box filter
#define n_filter 1  // box filter coefficients 
int box_filter(int value)
{
    static int buffer[n_filter];
    static int sum = 0;
    static int index = 0;

    sum -= buffer[index];      // remove oldest
    buffer[index] = value; // store newest
    sum += value;

    index = (index + 1) % n_filter;

    return (sum / n_filter);
}

// Spike filter
// TODO: (ring-buffer implementation)
int spike_filter(int array[2], int threshold) {
    int prev_value = array[0];
    int value = array[1];
    int diff = value - prev_value;
    
    if (abs(diff) > threshold) {
        return prev_value;   
    }
    return value;
    
}

// Low-pass filter
#define LPF_ALPHA 0.8f
static float filtered_joy_x = 0.0f; 

float low_pass_filter(int new_value) {
    filtered_joy_x = LPF_ALPHA * new_value + (1.0f - LPF_ALPHA) * filtered_joy_x;
    return filtered_joy_x;
}

// Combined median & low-pass filter
#define MEDIAN_SIZE 3
#define ALPHA 0.92f

static int median_buffer[MEDIAN_SIZE] = {0};
static int median_index = 0;
static float filtered_value = 0.0f;

int combined_filter(int new_value) {
    // Stage 1: Median filter (spike removal)
    median_buffer[median_index] = new_value;
    median_index = (median_index + 1) % MEDIAN_SIZE;
    
    int sorted[MEDIAN_SIZE];
    for (int i = 0; i < MEDIAN_SIZE; i++) sorted[i] = median_buffer[i];
    
    // Sort (for size 3, just find median directly)
    if (sorted[0] > sorted[1]) { int t = sorted[0]; sorted[0] = sorted[1]; sorted[1] = t; }
    if (sorted[1] > sorted[2]) { int t = sorted[1]; sorted[1] = sorted[2]; sorted[2] = t; }
    if (sorted[0] > sorted[1]) { int t = sorted[0]; sorted[0] = sorted[1]; sorted[1] = t; }
    
    int median = sorted[1];
    
    // Stage 2: Low-pass filter (smoothing)
    filtered_value = ALPHA * median + (1.0f - ALPHA) * filtered_value;
    
    return (int)filtered_value;
}