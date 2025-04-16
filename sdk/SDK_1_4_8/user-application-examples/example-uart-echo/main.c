
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

// We always start with including the user-application header file:
#include "user-application.h"



// Our setup, that runs only once:
void setup() {
  // We configure the TX / RX pins as UART and initialize it with a 
  // baud rate of 115200 bits per second:
  Serial.begin(115200);
  // In case some error occurs, we can detect them by setting:
  attachInterrupt(ERROR_INTERRUPT, Serial.printError, 0);

  Serial.print("Send me some characters and I'll echo them back\n");
}

// Our loop, that runs continuously:
void loop() {
  int count = Serial.available();
  if (count >0) {
    uint8_t buffer[100];
    Serial.readBytes(buffer, count);
    Serial.print("RECEIVED: ");
    Serial.write(buffer, count);
    Serial.print("\n");
  }
}


