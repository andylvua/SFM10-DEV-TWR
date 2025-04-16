
/* (c) SynchronicIT B.V. 2021. All rights reserved.                                   05/07/2024
             _____                  _               ______         _             
            / ____|                | |             |  ____|       (_)             TM
           | (___  _   _ _ __   ___| |__  _ __ ___ | |__ _   _ ___ _  ___  _ __  
            \___ \| | | | '_ \ / __| '_ \| '__/ _ \|  __| | | / __| |/ _ \| '_ \ 
            ____) | |_| | | | | (__| | | | | | (_) | |  | |_| \__ \ | (_) | | | |
           |_____/ \__, |_| |_|\___|_| |_|_|  \___/|_|   \__,_|___/_|\___/|_| |_|
                    __/ |                                                        
                   |___/                                 http://www.synchronicit.nl/ 

  This software is confidential and proprietary of SynchronicIT and is subject to the terms and 
  conditions defined in file 'LICENSE.txt', which is part of this source code package. You shall 
  not disclose such Confidential Information and shall use it only in accordance with the terms 
  of the license agreement.

*/

#include "user-application.h"

/* Example output:
measured 167 lux
measured 167 lux
measured 161 lux
measured 167 lux
measured 166 lux
*/

#define BH1750_ADDRESS 0x23
#define BH1750_POWER_ON 0x01

enum Mode {
  // same as Power Down
  UNCONFIGURED = 0,
  // Measurement at 1 lux resolution. Measurement time is approx 120ms.
  CONTINUOUS_HIGH_RES_MODE = 0x10,
  // Measurement at 0.5 lux resolution. Measurement time is approx 120ms.
  CONTINUOUS_HIGH_RES_MODE_2 = 0x11,
  // Measurement at 4 lux resolution. Measurement time is approx 16ms.
  CONTINUOUS_LOW_RES_MODE = 0x13,
  // Measurement at 1 lux resolution. Measurement time is approx 120ms.
  ONE_TIME_HIGH_RES_MODE = 0x20,
  // Measurement at 0.5 lux resolution. Measurement time is approx 120ms.
  ONE_TIME_HIGH_RES_MODE_2 = 0x21,
  // Measurement at 4 lux resolution. Measurement time is approx 16ms.
  ONE_TIME_LOW_RES_MODE = 0x23
};

void setup() {
  // Start UART and I2C
  Serial.begin(115200);
  I2C.begin(D2, D1);

  // Instruct BH1750 to wake up
  uint8_t data[] = { BH1750_POWER_ON };
  I2C.write(BH1750_ADDRESS, data, 1, true);
}

void loop() {
  // Instruct BH1750 to perform a measurement
  uint8_t data[] = { ONE_TIME_HIGH_RES_MODE, 0 };
  I2C.write(BH1750_ADDRESS, data, 1, true);

  // Wait until the measurement has completed
  delay(130);

  // Transfer the reading
  I2C.read(BH1750_ADDRESS, data, 2, true); 

  // Convert and print the reading
  uint16_t lux = (((uint16_t)data[0])<<8) + data[1];
  Serial.print("measured %u lux\n", lux);

  delay(1000);
}

