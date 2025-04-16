
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

// Define the radar range:
#define MAX_DISTANCE_M 5

// CIR will have samples of roughly 30cm:
#define CIR_SAMPLE_CNT ((MAX_DISTANCE_M * 3))

// Some arbitrary (8bit) event identifier: 
#define LOOPBACK_DONE (0x08)

// Our data container:
CirNtf cirBuf;


void setup() {
    // We start UART and UWB configuration:
    Serial.begin(115200);    

    // We change the CIR settings to our region of interest:
    SetConfigurationCmd config;
    SetConfiguration_setDefaults(&config);
    
    // Our first sample will represent 0m distance:
    config.channelImpulseResponseStart = 0;
    config.channelImpulseResponseLength = CIR_SAMPLE_CNT;
    UWB.setConfiguration(&config);

    // We start the UWB ciruitry:
    UWB.begin();

    // Using onRxUpdate we configure where to store the CIR:
    UWB.onRxUpdate(0, 0, 0, 0, 0, &cirBuf);
    // Using onLoopbackUpdate we say it should trigger
    // LOOPBACK_DONE when loopback is complete:
    UWB.onLoopbackUpdate((void*)LOOPBACK_DONE, 0);

    // We print the table header: time, followed by 
    // alternating real and imaginary numbers:
    Serial.print("  time, ", millis());
    for (uint16_t k = 0; k < CIR_SAMPLE_CNT*2; k++) {
        if (k % 2 == 0) {
            Serial.print(" r%02i, ", k/2);
        } else {
            Serial.print(" i%02i, ", k/2);
        }
    } 
    Serial.print("\n");

    // Start the first loopback:
    UWB.startLoopback(ASAP);
}

void loop() {
    // Wait, then print the channel impulse response:
    if (waitForEvent(0) == LOOPBACK_DONE) {
        Serial.print("%6u, ", millis());
        for (uint16_t k = 0; k < CIR_SAMPLE_CNT*2; k++) {
            Serial.print("%4i, ", cirBuf.cirData[k]);
        } 
        Serial.print("\n");
    } 
    delay(100);
    UWB.startLoopback(ASAP);
}

/* Example output:
  time,  r00,  i00,  r01,  i01,  r02,  i02,  r03,  i03, ...
    22,  316,  148,  260,  106,   38,    2, -150,  -78, ...
   140,  314,  149,  258,  106,   37,    1, -150,  -78, ...
   257,  311,  147,  255,  107,   35,    5, -150,  -74, ...
   374,  314,  147,  258,  105,   37,    3, -150,  -75, ...
   491,  315,  149,  259,  107,   37,    2, -150,  -78, ...
   608,  316,  148,  257,  105,   36,    2, -149,  -76, ...
   725,  313,  147,  257,  105,   36,    2, -150,  -77, ...
   842,  314,  148,  257,  106,   36,    3, -151,  -75, ...
*/

