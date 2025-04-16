
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

void setup() {
    Serial.begin(115200);
}

void loop() {
    uint32_t voltage = analogRead(VDD_IO);
    uint32_t temp = analogRead(INT_TEMPERATURE);
    Serial.print("GPIO VDD measures %u millivolts ", voltage); 
    Serial.print("and the chip temperature reads ");
    Serial.print("%u.%03u degrees Celcius\n", temp/1000, temp%1000); 
    delay(1000);
}        

/* Example output:
GPIO VDD measures 3083 millivolts and the chip temperature reads 23.112 degrees Celcius
*/            

