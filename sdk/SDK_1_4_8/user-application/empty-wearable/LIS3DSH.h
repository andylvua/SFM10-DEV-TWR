
#ifndef LIS3DSH_h
#define LIS3DSH_h

// Accelerometer
// LIS3DSHTR
// 0x1D

#include <stdint.h>
#include <stdbool.h>

uint32_t LIS3_getJerk(void (*deadTimeInstrutions)(void));

#endif // LIS3DSH_h