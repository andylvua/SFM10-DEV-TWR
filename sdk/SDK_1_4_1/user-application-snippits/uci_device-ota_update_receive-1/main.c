
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

// 1) Upload this application to a SFM10 device, as normally done.

// 2) Disconnect it from the PC and power it with for example a power bank.

// 3) Make a change in the application, e.g. change ``PIN_LED_G`` into ``PIN_LED_R``.

// 4) Take a second SFM10 device for OTAU distribution and connect it to the PC.

// 5) in file .vscode/tasks.json near the end change the line:
//     "../${config:sourceFiles}/upload.py",
// into:
//     "../${config:sourceFiles}/otau.py",

// 6) Press F5 (or Ctrl+Shift+B). Within 2 seconds the initial SFM10 device will adopt the changes.


#include "user-application.h"

void setup() {
  pinMode(PIN_LED_G, OUTPUT_HIGH);
}

PayloadNtf rxPacket;
uint32_t tLastChecked = 0;


void loop() {
  digitalWrite(PIN_LED_G, LOW);
  delay(200);
  digitalWrite(PIN_LED_G, HIGH);
  delay(100);

  uint32_t tNow = millis();
  if (tNow - tLastChecked >1000) {
    UWB.begin();
    UWB.onRxUpdate( RX_DONE, NULL, &rxPacket, sizeof(OtaPacket_t), NULL, NULL );
    UWB.startRx(ASAP);
    if ((waitForEvent(25) & RX_DONE) && IS_OTAU(&rxPacket)) {
      GET_OTAU();
    }
    UWB.end();
    tLastChecked = tNow;
  }
}

