
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

void setup() { // wake up at ~38.4Mhz 
  Serial.begin(115200);
  Serial.print("Time = %u\n", micros());

  UWB.begin(); // use XTAL ~55.2Mhz for CPU
  // and start ~64MHz timestamp unit
  uint32_t t1 = micros(); // capture CPU time 
  // and capture timestamp unit time:
  uint32_t t2 = TICKS_TO_MICROS(*ticks());

  Serial.print("%i vs %i us\n", t1, t2);
  delay(10); // Allow UART to print
  UWB.end();

  Serial.print("Now sleep...\n");
  delay(2); // Allow UART to print

  EnterPowerModeCmd cmd = {
    .powerMode = DEEP_POWER_DOWN,
    .wakeUpPin = PIN_CS1,
    .wakeUpTime = millis() + 1000,
  };
  UWB.enterPowerMode(&cmd); // start 32Khz oscillator 
}

