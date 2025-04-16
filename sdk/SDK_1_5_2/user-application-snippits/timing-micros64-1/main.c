
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

void setup() {
    Serial.begin(115200);
}

void loop() {
    uint64_t t = *micros64(); // use * notation to get the value
    Serial.print("Microseconds since boot = ");
    Serial.printInt64(t, 0);
    Serial.print(" (0x%08X%08X)\n", (uint32_t)(t >>32), (uint32_t)(t));
    delay(1000);
}

/* Example output:
Microseconds since boot = 2474 (0x00000000000009AA)
Microseconds since boot = 1009028 (0x00000000000F6584)
Microseconds since boot = 2015582 (0x00000000001EC15E)
Microseconds since boot = 3022137 (0x00000000002E1D39)
*/

