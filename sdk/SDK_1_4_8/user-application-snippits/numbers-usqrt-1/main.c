
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
    for (int i = 0; i < 10; i++) {
        Serial.print("usqrt(%u) = %u\n", i, usqrt(i));
    }
    // usqrt(0) = 0
    // usqrt(1) = 1
    // usqrt(2) = 1
    // usqrt(3) = 2
    // usqrt(4) = 2
    // usqrt(5) = 2
    // usqrt(6) = 2
    // usqrt(7) = 3
    // usqrt(8) = 3
    // usqrt(9) = 3
}

