
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

                                                                  USER APPLICATION version 1.4.1
*/

#include "user-application.h"

void monitorRam() {
  Serial.print("\nGLOBALS:\n");
  Serial.print("  capacity: %8u bytes\n",  globalsCapacity());
  Serial.print("      used: %8u bytes\n",  globalsUsage());  
  Serial.print("\nUSER STACK:\n");
  Serial.print("  capacity: %8u bytes\n",  stackCapacity(USER_STACK));
  Serial.print("      used: %8u bytes\n",  stackUsageMax(USER_STACK));
  Serial.print("   current: %8u bytes\n",  stackUsageCurrent(USER_STACK));
  Serial.print("\nIDLE STACK:\n");
  Serial.print("  capacity: %8u bytes\n",  stackCapacity(IDLE_STACK));
  Serial.print("      used: %8u bytes\n",  stackUsageMax(IDLE_STACK));
  Serial.print("   current: %8u bytes\n",  stackUsageCurrent(IDLE_STACK));
  Serial.print("\nINTERRUPT STACK:\n");
  Serial.print("  capacity: %8u bytes\n",  stackCapacity(INTERRUPT_STACK));
}

void setup() {
    Serial.begin(115200);
}

void loop() {
    monitorRam();
    delay(1000);
}

// output:
//
// GLOBALS:
//   capacity:    16384 bytes
//       used:       28 bytes

// USER STACK:
//   capacity:     3072 bytes
//       used:      520 bytes
//    current:      408 bytes

// IDLE STACK:
//   capacity:     1024 bytes
//       used:       88 bytes
//    current:       88 bytes

// INTERRUPT STACK:
//   capacity:     1536 bytes              

