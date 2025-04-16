
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

                                                                  USER APPLICATION version 1.4.8
*/

#include "user-application.h"

void setup() {
    Serial.begin(115200);
    UWB.begin(); // Start UWB circuitry
}

void loop() {
    uint64_t us = *micros64(); // microseconds since boot
    uint64_t tcks = *ticks();
    uint64_t ticks2us = TICKS_TO_MICROS(tcks);
    Serial.printInt64(tcks, 0);
    Serial.print(" ticks since boot. This equals %uus (CPU) ", (uint32_t)us); 
    Serial.print("/ %uus (UWB).\n", (uint32_t)ticks2us); 
    delay(1000);
}

/* Example output:
319848448 ticks since boot. This equals 5005us (CPU) / 5005us (UWB).
64600838656 ticks since boot. This equals 1011005us (CPU) / 1011005us (UWB).
128945725440 ticks since boot. This equals 2018006us (CPU) / 2018005us (UWB).
193290606592 ticks since boot. This equals 3025005us (CPU) / 3025005us (UWB).
*/

