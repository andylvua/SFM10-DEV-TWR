
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

DeviceStatusNtf deviceStatusNtf;

void deviceNtf() {
    Serial.print("deviceState = 0x%X, reason = 0x%X\n", \
      deviceStatusNtf.deviceState, deviceStatusNtf.deviceStateReason);
}

void setup() {
    Serial.begin(115200);    
    UWB.onDeviceStatusUpdate(deviceNtf, &deviceStatusNtf); // deviceNtf called
    
    UWB.begin(); // device status update, deviceNtf called
    delay(10);
    UWB.end(0); // device status update, deviceNtf called
}

/* Example output:
deviceState = 0x1, reason = 0x2
deviceState = 0x2, reason = 0x11
deviceState = 0x1, reason = 0x10
*/

