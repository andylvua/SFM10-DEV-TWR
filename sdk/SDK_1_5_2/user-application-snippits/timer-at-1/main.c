
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

void foo_1() {
    uint32_t t = millis();
    Serial.print("1: Arrived here at t = %u ms.\n", t); 
}

void foo_2() {
    uint32_t t = millis();
    Serial.print("2: Arrived here at t = %u ms.\n", t); 
}

void setup() {
    Serial.begin(115200);
    Timer.begin();
    Timer.at(1500, foo_2); // schedule foo_2() at 1500ms
    Timer.at(200, foo_1);  // schedule foo_1() at 200ms
}

/* Example output:
1: Arrived here at t = 200 ms.
2: Arrived here at t = 1502 ms.
*/

