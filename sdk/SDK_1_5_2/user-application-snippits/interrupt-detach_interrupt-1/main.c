
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
    Serial.print("1: button pressed!\n");
}

void foo_2() {
    Serial.print("2: button pressed!\n");
}

void setup() {
    Serial.begin(115200);
    pinMode(PIN_USER_BUTTON, INPUT_PULLUP);
    attachInterrupt(PIN_USER_BUTTON, foo_1, FALLING);
    delay(4000); // after 4 seconds, we change button functionality:
    detachInterrupt(PIN_USER_BUTTON); // detach first
    attachInterrupt(PIN_USER_BUTTON, foo_2, FALLING);
}

