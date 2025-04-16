
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

TxTimestampNtf txResult;

uint32_t tCommand;
uint32_t tSchedule;
uint32_t tEvent;

void txEventHandler() {
    tEvent = micros();
}

void setup() {
    Serial.begin(115200);    
    
    UWB.begin();

    UWB.onTxUpdate(txEventHandler, &txResult);
}

void loop() {
    UciStatus retCode;
    tCommand = micros();
    retCode = UWB.startTx(ASAP);
    tSchedule = micros();
    if (retCode != UCI_STATUS_SUCCESS) {
        Serial.print("startTx failed: 0x%X\n", retCode);
    }
    // We get here before txEventHandler() is called (asynchronous programming):
    Serial.print("Command/schedule/event = %u/%u/%u\n", tCommand, tSchedule, tEvent);
    delay(1000);
}

/* Example output:
Command/schedule/event = 5008/5034/0
Command/schedule/event = 1009008/1009034/5162
Command/schedule/event = 2013008/2013034/1009162
Command/schedule/event = 3018008/3018034/2013162
Command/schedule/event = 4023008/4023034/3018162
*/

