#include "LIS3DSH.h"


#include "user-application.h"
#include "I2C_helpers.h"



#define ADDRESS_LIS3DSHTR (0x1D)
#define LIS3_WHO_AM_I (0x0F)
#define LIS3_STAT (0x18)
#define LIS3_PEAK1 (0x19)
#define LIS3_SETT1 (0x5B)
#define LIS3_CTRL_REG1 (0x21)
#define LIS3_CTRL_REG2 (0x22)
#define LIS3_CTRL_REG3 (0x23)
#define LIS3_CTRL_REG4 (0x20)
#define LIS3_CTRL_REG5 (0x24)
#define LIS3_THRS1_1 (0x57)
#define LIS3_ST1_1 (0x40)
#define LIS3_ST1_2 (0x41)
#define LIS3_STATUS (0x27)
#define LIS3_FIFO_CTRL (0x2E)
#define LIS3_OUT_X (0x28)
#define LIS3_OUT_Y (0x2A)
#define LIS3_OUT_Z (0x2C)



I2C_FUN(LIS3, ADDRESS_LIS3DSHTR);


// int8_t LIS3_write(uint8_t addr, uint8_t value) {
//     uint8_t buffer[2];
//     buffer[0] = addr;
//     buffer[1] = value;
//     return (I2C.write(ADDRESS_LIS3DSHTR, buffer, 2, true));
// }




uint32_t LIS3_getJerk(void (*parallelTask)(void)) {
    LIS3_write(LIS3_CTRL_REG4, 0x77); // power up, 400Hz, 3-axis
    uint32_t timeout = millis() + 8;
    if (parallelTask) {
        (*parallelTask)();
    }

    while (millis() < timeout) {
      delay(1);
    }


  // delay(6); // important!
  int16_t buffer[6];
  LIS3_readBytes(LIS3_OUT_X, (uint8_t*)&buffer[0], 6);
  delay(4);    
  LIS3_readBytes(LIS3_OUT_X, (uint8_t*)&buffer[3], 6);
  LIS3_write(LIS3_CTRL_REG4, 0x00); // power down
  
  uint32_t sum = 0;
  for (int i = 0; i < 3; i++) {
    int32_t delta = buffer[i] - buffer[i+3];
    sum += (delta * delta);
  }
  return sum;
}