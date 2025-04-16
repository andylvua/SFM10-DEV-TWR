
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
    uint32_t t1 = micros();
    delayMicroseconds(1200); // halt process for 1200 us
    uint32_t t2 = micros();
    uint32_t t3 = micros(); // t3-t2 = execution time of micros()
    Serial.print("Halted for %u us.\n", (t2 - t1) - (t3 - t2)); 
    delay(1000);
}

/* Example output:
Halted for 1200 us.
Halted for 1205 us.
Halted for 1202 us.
Halted for 1202 us.
*/

