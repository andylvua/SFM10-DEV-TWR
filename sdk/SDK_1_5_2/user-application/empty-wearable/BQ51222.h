
#ifndef BQ51222_h
#define BQ51222_h

// Wireless Power
// BQ51222YFPT
// 0x6C

#include <stdint.h>
#include <stdbool.h>

uint16_t BQ51_getVoltage();

uint16_t BQ51_getPower();

bool BQ51_ready();

#endif // BQ51222_h