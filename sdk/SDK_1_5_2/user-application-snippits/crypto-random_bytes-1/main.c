
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
    Serial.print("\nStarted\n");
    Crypto.begin();
    uint32_t values[16];
    Crypto.randomBytes((uint8_t*)values, sizeof(values));
    Serial.hexDump((uint8_t*)values, sizeof(values), 16);
}

/* Example output:

Started

  6c cf a6 1b 0b c1 bc f0 13 85 f2 0d 7a 8b 4a 8c
  c0 46 4c b3 3b 79 d3 b6 64 43 d9 98 45 4e f1 c4
  7f 8b e9 98 85 59 fc 6b 30 fc 78 53 cf 55 30 52
  05 d2 b2 87 1c bb bc 52 f1 97 2c 1f 1d 39 5b 41

Started

  ec c8 ef 7e 7e aa 67 14 d2 e3 44 0f dd 25 76 6f
  7c 5d 98 17 e4 71 84 b6 c5 64 e3 a8 3a 2a 95 81
  f9 e9 82 d3 2b e3 a3 3e ac 08 dd ae 0f d1 4a b9
  43 05 76 f7 2c 8b 7d c5 f0 71 9c 28 e7 c5 c4 55

Started

  d7 6b 85 35 fe 58 30 7f 38 c5 3a 06 f9 9b cb 79
  26 46 fe 00 d5 fd 14 e2 26 d6 0c 9c f0 91 a8 2f
  87 39 cf 91 5e 1f e5 f6 d0 d4 08 bf 83 b1 87 27
  0c 60 a4 47 41 06 2e 7a 02 54 87 df 72 fa 83 8e
*/

