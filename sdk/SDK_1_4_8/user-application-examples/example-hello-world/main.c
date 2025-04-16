
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
}

// Our loop, that runs continuously:
void loop() {
  // We print Hello World. Don't forget to add \n at the end, which 
  // triggers a UART flush: all the bytes in the UART buffer are 
  // sent on the wire.
  Serial.print("Hello world!\n");
  delay(1000);
}

