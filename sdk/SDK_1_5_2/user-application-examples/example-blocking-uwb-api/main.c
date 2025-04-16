
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

/*
This example demonstrates blocking UWB API usage. As long as the RX interrupt 
is not attached using attachInterrupt(UWB_RX_INTERRUPT, myEvent, 0) or 
UWB.onRX(myEvent, ....), the RX read functions like readPayload and readRxTime 
are blocking. That is, the function waits until the UWB subsystem is idle.
The same holds for TX events.
*/

/*
Right when this application starts, the device serial number is transmitted via UWB.
Then, it will start UWB reception and prints the received payload(s).
*/

void setup() {
    uint32_t mySerial = getSerialNumber();

    Serial.begin(115200);

    Serial.print("Starting node 0x%x\n", mySerial);
    ASSERT_0( UWB.begin() );

    // Insert "mySerial" as TX payload using setPayload(&mySerial, 4):
    ASSERT_0( setPayload((uint8_t*)&mySerial, sizeof(uint32_t)) );
    ASSERT_0( UWB.startTx(ASAP) );
}

void loop() {
    ReadRxTimeRsp rxTime;
    ReadPayloadRsp payload;

    // Start RX. In case TX is still busy, UWB.startRx() will wait:
    ASSERT_0( UWB.startRx(ASAP) );
    
    // Read payload as soon as RX has stopped:
    ASSERT_0( UWB.readPayload(&payload) );
    ASSERT_0( UWB.readRxTime(&rxTime) );
    // If RX was successful, print results:
    if (payload.payloadLength) {
        Serial.print("Received payload:\n");
        Serial.hexDump(payload.payloadData, payload.payloadLength, 16);
        Serial.print("\n  at ticks: ");
        Serial.printInt64(rxTime.rxTimestamp, NULL);
        Serial.print("\n");
    }
}

/* [Monitor output]
Starting node 0xf05bb4bf
Starting node 0xf05b95cd
Received payload:
cd 95 5b f0
at ticks: 1342859686
*/

