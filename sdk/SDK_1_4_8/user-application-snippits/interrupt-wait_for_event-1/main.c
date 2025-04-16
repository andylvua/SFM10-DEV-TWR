
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

#define BUTTON_PRESS_EVENT 0x1000

void foo() {
    triggerEvent(BUTTON_PRESS_EVENT);
}

void setup() {
    Serial.begin(115200);    
    pinMode(PIN_USER_BUTTON, INPUT_PULLUP);
    attachInterrupt(PIN_USER_BUTTON, foo, FALLING);
}

void loop() {
    uint32_t eventMask = waitForEvent(1000);
    if (eventMask == EVENT_TIMEOUT) {
        Serial.print("No event happened within the specified time\n");
    } else {
        if ((eventMask & BUTTON_PRESS_EVENT) > 0) {
            Serial.print("A button is called");
            Serial.print(" and we are in a derived service routine!\n");

        }
    } 
}

