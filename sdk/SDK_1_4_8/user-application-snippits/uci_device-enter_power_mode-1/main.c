
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

void setup() {
    Serial.begin(115200);    

    Serial.print("Boot!\n");

    delay(10); // wait for Serial.print to finish
    EnterPowerModeCmd cmd = {
        .powerMode = DEEP_POWER_DOWN,
        .wakeUpPin = WUP_RX_FALLING | WUP_D3_FALLING,
        .wakeUpTime = millis() + 5000, // wake up after 5 seconds
    };

    UciStatus retCode = UWB.enterPowerMode(&cmd);

    if (retCode != UCI_STATUS_SUCCESS) {
        Serial.print("enterPowerMode failed: %X\n", retCode);
        return;
    }

    Serial.print("This line is never reached\n");
    // Device boots after UART is received (RX)
    // OR when the User Button is pressed (D3)
    // OR when 5000 ms have passed
}

