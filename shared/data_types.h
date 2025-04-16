//
// Created by Andrew Yaroshevych on 30.10.2024.
//

#ifndef DATA_TYPES_H
#define DATA_TYPES_H

#include <sys/types.h>

typedef struct __attribute__((packed)) {
    uint16_t anchor;       // 16-bit anchor address
    uint32_t timestamp;    // Timestamp (us)
    int16_t distance;      // Distance (cm)
    uint8_t rangeNumber;   // Range number (modulo 256)
    uint8_t status;        // Transaction status (see twr_result_t)
    uint8_t losStatus;     // Line-of-sight status
    /* For diagnostics. Currently unused */
    // int32_t lastSlotCorrection;
    // int32_t transactionTime;
    // int32_t guardTime;
} uwbLogMessageData_t;

#endif //DATA_TYPES_H
