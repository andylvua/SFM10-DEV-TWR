#include "BQ51222.h"

#include "user-application.h"
#include "I2C_helpers.h"


#define ADDRESS_BQ51222 (0x6C)
#define BQ51_MAILBOX (0xE0)
#define BQ51_VRECT (0xE3)
#define BQ51_VOUT (0xE4)
#define BQ51_REC_PWR (0xE8)


I2C_FUN(BQ51, ADDRESS_BQ51222);


bool BQ51_ready() {
    return (I2C.write(ADDRESS_BQ51222, 0, 0, true) == WRITE_RESULT_ACK);
}


uint16_t BQ51_getVoltage() {
    return (uint16_t)BQ51_read(BQ51_VOUT) * 46;
}


uint16_t BQ51_getPower() {
    return (uint16_t)BQ51_read(BQ51_REC_PWR) * 39;
}



