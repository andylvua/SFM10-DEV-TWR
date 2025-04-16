
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

/* Example output:
9437: 356 mm
9448: 272 mm
9459: 323 mm
9470: 342 mm
9481: 234 mm
9492: 276 mm
9503: 337 mm
9514: 370 mm
9525: 211 mm
9536: 361 mm
*/

// Variable to indicate the current role:
uint8_t isInitiator = 0; // 0 = responder

// Forward declarations (so we can 'use' these 
// before they are defined):
void initiator_task(void);
void responder_task(void);
void btn_down_isr(void);

// We setup UART, UWB, and USER_BUTTON
void setup() {
    Serial.begin(115200);
    UWB.begin();

    pinMode(PIN_USER_BUTTON, INPUT_PULLUP); // setup button pin
    attachInterrupt(PIN_USER_BUTTON, btn_down_isr, FALLING);
    attachInterrupt(ERROR_INTERRUPT, Serial.printError, 0);
}

// In loop, we perform the initiator or responder task:
void loop() {
  if (isInitiator) {
    delay(10);
    initiator_task();
    isInitiator--;
  } else {
    responder_task();
  }
}

// Button press handler:
void btn_down_isr(void) {
  // After a button press, we'll perform 10 distance measurements:
  isInitiator = 10;
  // trigger an event to make the responder_task stop
  // Any event number will do except for RX_DONE and TX_DONE
  triggerEvent(GPIO_INTERRUPT);
}

void initiator_task() {
    // We transmit an empty packet as soon as possible (UWB.startTx(ASAP))
    TxTimestampNtf txResult;
    UWB.onTxUpdate(TX_DONE, &txResult);
    ASSERT_GOTO(UWB.startTx(ASAP), SUCCESS, end);
    ASSERT_GOTO(waitForEvent(1), TX_DONE, end);

    // We listen for the response:
    RxTimestampNtf rxResult;
    PayloadNtf rxPayload; 
    UWB.onRxUpdate(RX_DONE, &rxResult, &rxPayload, 100, 0, 0);
    ASSERT_GOTO(UWB.startRx(ASAP), SUCCESS, end);
    ASSERT_GOTO(waitForEvent(2), RX_DONE, end);

    // We print the result:
    #define replyTime_ (*(uint32_t*)rxPayload.payloadData)   
    #define roundTripTime (rxResult.rxTimestamp - txResult.txTimestamp)
    #define antennaDelay (13914)

    int64_t ToF = (roundTripTime - replyTime_) / 2 - antennaDelay; 
    int32_t distance = TICKS_TO_MILLIMETERS(ToF);
    Serial.print("%5u: %i mm\n", millis(), distance);
    end: 
    UWB.stopAction();
}

void responder_task() {
    // We listen for a packet and store the time of reception:
    RxTimestampNtf received;
    UWB.onRxUpdate(RX_DONE, &received, 0, 0, 0, 0);
    ASSERT_GOTO(UWB.startRx(ASAP), SUCCESS, end);
    ASSERT_GOTO(waitForEvent(1000), RX_DONE, end);

    ASSERT_0(received.uciStatus);

    // After reception, we schedule the response:
    SetPayloadCmd reply = { .payloadLength = sizeof(uint32_t) };
    #define replyTime (*(uint32_t*)reply.payloadData)
    StartTxCmd send = { .time = *ticks() + MICROS_TO_TICKS(150) };
    replyTime = send.time - received.rxTimestamp;  
    
    // We set the reply time as payload, and transmit:
    UWB.setPayload(&reply); 
    UWB.stopAction(); // just to make sure
    UWB.onTxUpdate(TX_DONE, 0);

    ASSERT_GOTO(UWB.startTx(&send), SUCCESS, end);
    ASSERT_GOTO(waitForEvent(2), TX_DONE, end);  
    end:
    UWB.stopAction();
}

