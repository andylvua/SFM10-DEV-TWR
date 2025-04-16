
/* (c) SynchronicIT B.V. 2021. All rights reserved.                                   05/07/2024
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
    Serial.print("UWB Started\n");

    SetConfigurationCmd config;    
    // Alternatively, use UWB.getConfiguration(&config):
    SetConfiguration_setDefaults(&config); // fill in defaults

    config.currentLimit = 5; // enable current limiter
    
    ASSERT(UWB.setConfiguration(&config), SUCCESS); // pass

    UWB.startRx(0);
    // Changing Radio settings during an action will trigger an 
    //  Operation not allowed error:
    config.channelFrequency = UWB_CHANNEL_9;
    config.txPower = 1;

    ASSERT(UWB.setConfiguration(&config), SUCCESS);  
    delay(100);
    UWB.end();
    // When we restart UWB, the settings that failed previously become effective
    UWB.begin();
    UWB.getConfiguration(&config);
    Serial.print("Now the radio is running at %u kHz\n", config.channelFrequency);

    UWB.startRx(0);
}

/* example output:
UWB Started
Returned 0xB8 at line 41
Now the radio is running at 7987200 kHz
*/

