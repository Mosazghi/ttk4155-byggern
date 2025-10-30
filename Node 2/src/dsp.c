#include "dsp.h"

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

int spike_filter(int array[2], int threshold) {
    int prev_value = array[0];
    int value = array[1];
    int diff = value - prev_value;
    
    if (abs(diff) > threshold) {
        return prev_value;   
    }
    return value;
    
}