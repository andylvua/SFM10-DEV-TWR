
/* (c) SynchronicIT B.V. 2021. All rights reserved.                                   24/02/2025
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

                                                                  USER APPLICATION version 1.5.2
*/

#include "user-application.h"

#define AMG88_ADDR 0x69

void setup() {
    Serial.begin(115200);

    I2C.begin(D2, D1);

    uint8_t data[128];
    data[0] = 0x80; // temperature register start
    I2C.write(AMG88_ADDR, data, 1, true);

    // now request/read the values from that register:
    uint8_t result = I2C.read(AMG88_ADDR, data, 128, true);

    // check if success:
    switch (result) {
        case READ_RESULT_ACK: 
            Serial.print("WRITE ACK\n"); break;
        case READ_RESULT_NACK: 
            Serial.print("WRITE NACK\n"); break;
        case READ_RESULT_TIMEOUT: 
            Serial.print("WRITE TIMEOUT\n"); break;
    }
}

