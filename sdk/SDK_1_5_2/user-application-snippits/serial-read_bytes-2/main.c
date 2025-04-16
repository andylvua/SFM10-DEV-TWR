
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
#include <string.h>

#define RECEIVED_SERIAL_EVENT (0x100)

void setup() {
    Serial.begin(115200);
    attachInterrupt(SERIAL_INTERRUPT, (void*)RECEIVED_SERIAL_EVENT, 0);

    while (1) {
        uint32_t events = readEvents();
        if (events == 0) {
            events == waitForEvent(1000);
        }
        if (events == 0) {
            Serial.print("nothing\n");
        }
        if (events & RECEIVED_SERIAL_EVENT) {
            delay(1);
            uint8_t buf[256];
            uint16_t len = Serial.available();
            if (len > 0) {
                memset(buf, 0, sizeof(buf));
                Serial.readBytes(buf, len);
                Serial.print("Received (%u): %s\n", len, buf);
                Serial.print("In HEX: [ "); 
                Serial.hexDump(buf, len, 32);
                Serial.print(" ]\n"); 
            }
        }
    }
}

