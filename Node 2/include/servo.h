#include "sam3xa.h"  // Header for SAM3X register definitions
#include "sam.h"
#define SERVO_MAX 2000
#define SERVO_MIN 1000
#define SERVO_CENTERED 1500
#define joystick_min -100
#define joystick_max 100
#define CLAMP(value, min, max) ((value) < (min) ? (min) : ((value) > (max) ? (max) : (value)))

// typedef struct {
//   int x;
//   int y;
//   bool btn;
// } joystick_xy_t;

long map(long x, long in_min, long in_max, long out_min, long out_max);
void piob_output_init(int pin);
void piob_set_pin_low(int pin);
void piob_set_pin_high(int pin);

/**
 * @brief Converts joystick value to pulsewidth in us.
 * 
 * @param joystick_value [-100,100]
 * @return int [1000, 2000]us
 */
int pos_to_us(int joystick_value);
//void joystick_move_servo(joystick_xy_t *joystick);

int remap(uint8_t value);
