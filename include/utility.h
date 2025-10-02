#pragma once
#define F_CPU 4915200UL
#include <stdio.h>
#include <util/delay.h>
#define BASE_LOG(fmt, info, ...) printf(info fmt "\n\r", ##__VA_ARGS__);
#if DEBUG
#define LOG_DBG(fmt, ...) BASE_LOG(fmt, "[Debug]: ", ##__VA_ARGS__);
#else
#define LOG_DBG(fmt, ...) \
  do {                    \
  } while (0);
#endif
#define LOG_INF(fmt, ...) BASE_LOG(fmt, "[Info]: ", ##__VA_ARGS__);
#define LOG_ERR(fmt, ...) BASE_LOG(fmt, "[Error]: ", ##__VA_ARGS__);
#define UNUSED(x) (void)(x);
#define HZ_TO_TIMER(HZ) (F_CPU / (1024UL * HZ) - 1)
#define PIN_WRITE(PORT, PIN, MODE) \
  do {                             \
    if (MODE) {                    \
      (PORT) |= (1 << (PIN));      \
    } else {                       \
      (PORT) &= ~(1 << (PIN));     \
    }                              \
  } while (0)

#define PIN_MODE(DDR, PIN, MODE) \
  do {                           \
    if (MODE) {                  \
      (DDR) |= (1 << (PIN));     \
    } else {                     \
      (DDR) &= ~(1 << (PIN));    \
    }                            \
  } while (0)

#define CLAMP(value, min, max) ((value) < (min) ? (min) : ((value) > (max) ? (max) : (value)))
#define ABS(value) ((value) < 0 ? -(value) : (value))

#define ARR_LEN(arr) sizeof(arr) / sizeof(arr[0])
#define LOW 0
#define HIGH 1
#define INPUT 0
#define OUTPUT 1
