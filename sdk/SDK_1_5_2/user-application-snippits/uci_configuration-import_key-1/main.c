
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

*/

#include "user-application.h"

void setup() {
    Serial.begin(115200);    
    UWB.begin();
    ImportKeyCmd exampleKey = {
        .keyIdx = 0,
        .keyUsage = UWB_DEVICE_IMPORT_KEY_CMD_PAYLOAD_STS_GENERATION,
        .rawKey = {
            0x0d, 0xfc, 0x5c, 0x31, 0x98, 0xa1, 0xbe, 0x1a, 
            0xe6, 0xee, 0xcf, 0x1b, 0xa3, 0x13, 0xac, 0x02 
        },
    };
    UciStatus retCode = UWB.importKey(&exampleKey);

    if (retCode != UCI_STATUS_SUCCESS) {
      Serial.print("importKey failed: %X\n", retCode);
    } else {
      Serial.print("importKey success\n");
    }
}

