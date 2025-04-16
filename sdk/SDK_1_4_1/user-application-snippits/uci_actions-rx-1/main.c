
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

#include "user-application.h"

RxTimestampNtf rxTimestamp;

uint8_t myRxPayloadBuffer[20];
PayloadNtf* rxPayload = (PayloadNtf*)&myRxPayloadBuffer[0];
const uint16_t maxRxSize = sizeof(myRxPayloadBuffer) - 3;

FirstPathInfoNtf fpi;


void myISR() {
    // Should be as quick as possible
    triggerEvent(1234);
}

void setup() {
    Serial.begin(115200);
    attachInterrupt(ERROR_INTERRUPT, Serial.printError, 0);
    ASSERT_0(UWB.begin());
    UWB.onRxUpdate(myISR, &rxTimestamp, rxPayload, maxRxSize, &fpi, 0);
    // Could also use instead:
    // UWB.onRxUpdate(1234, &rxTimestamp, rxPayload, maxRxSize, &fpi, 0);
    UWB.startContinuousRx(ASAP);
}

void loop() {
    uint32_t eventId = waitForEvent(1000);
    if (eventId == EVENT_TIMEOUT) {
        Serial.print("Nothing received\n");
    } else if (eventId == 1234) {
        Serial.print("Received something!\n");
        Serial.print("Payload size = %u\n", rxPayload->payloadLength);
        Serial.print("Signal strength = %i\n", GET_RSSI_x10(fpi.overallRxPower));
    }
}

