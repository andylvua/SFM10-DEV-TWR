
/* (c) SynchronicIT B.V. 2021. All rights reserved.                                   18/09/2024
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
Temperature: 22.4 deg C, Relative Humidity: 61.2%
Temperature: 22.4 deg C, Relative Humidity: 61.3%
Temperature: 22.4 deg C, Relative Humidity: 61.2%
Temperature: 22.4 deg C, Relative Humidity: 61.2%
Temperature: 22.4 deg C, Relative Humidity: 61.2%
Temperature: 22.4 deg C, Relative Humidity: 61.2%
Temperature: 22.4 deg C, Relative Humidity: 61.2%
*/

#define ADDRESS_SHT30 (0x45)

void setup() {
    // Start UART and I2C:
    Serial.begin(115200);
    I2C.begin(D2, D1);
}

void loop() {
    // Send measurement command
    uint8_t data[6] = {0x2C, 0x06};
    I2C.write(ADDRESS_SHT30, data, 2, true);
    // Wait a little, then transfer the readings
    delay(500);
    I2C.read(ADDRESS_SHT30, data, 6, true);

    // Transfered bytes:
  // 0: cTemp msb, 1: cTemp lsb, 2: cTemp crc, 
    // 3: humidity msb, 4: humidity lsb, 5: humidity crc

  int16_t temp_x10 = (((((uint32_t)data[0] << 8) + \
        (uint32_t)data[1]) * 1750) / 65535) - 450;
  int16_t humidity_x10 = (((((uint32_t)data[3] << 8) + \
        (uint32_t)data[4]) * 1000) / 65535);

    Serial.print("Temperature: %i.%u deg C, ", \
        temp_x10/10, temp_x10%10);
    Serial.print("Relative Humidity: %i.%u%%\n", \
        humidity_x10/10, humidity_x10%10);
}

