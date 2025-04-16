
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

/* Example output:
It took 46 us to transmit the SFD
It then took 45 us to trigger UWB_TX_DONE
It then took 49 us to return
Current CPU vs UWB time: 30012695 vs 30012694 = -1 us
*/

#include "user-application.h"

// Some arbitrary 8-bit event identifiers:
enum {
    TX_EVENT = 0x12,
    BUTTON_EVENT = 0x23,
};

// Data containers that we'll use to calculate latencies:

TxTimestampNtf txResult;
uint32_t txDoneMarker;
uint32_t returnMarker;
uint32_t scheduleMarker;

// These are Interrupt Service Routines or interrupt handlers:

void scheduleTx() {
  scheduleMarker = micros();
  UWB.startTx(ASAP);
  // Previously we set:
  //  - attachInterrupt(UWB_TX_DONE, txDoneHandler, 0):
  //    Interrupt UWB_TX_DONE is triggered before data is read!
  //    This can be used to early schedule events or change GPIO.
  // 
  //  - UWB.onTxUpdate((void*)TX_EVENT, &txResult):
  //    When the TX action is fully completed, 
  //    triggerEvent(TX_EVENT) is called. TX timestamp will be 
  //    stored in container object txResult.
}

void txDoneHandler() {
  txDoneMarker = micros();
}

void buttonHandler(void) {
  triggerEvent(BUTTON_EVENT);
}


void setup() {
  // Setup user (D3 / GPIO4) button and UART:

  pinMode(PIN_USER_BUTTON, INPUT_PULLUP);
  attachInterrupt(PIN_USER_BUTTON, buttonHandler, FALLING);
  Serial.begin(115200);

  UWB.begin();

  // Configure UWB interrupts:
  attachInterrupt(UWB_TX_DONE, txDoneHandler, 0);
  UWB.onTxUpdate((void*)TX_EVENT, &txResult);
  Timer.every(1000, scheduleTx);
}

void loop() {
  uint32_t eventId = waitForEvent(0); // Wait for a triggerEvent()

  switch (eventId) { 

    case TX_EVENT: {
      returnMarker = micros();
      uint32_t txTimestamp_us = TICKS_TO_MICROS(txResult.txTimestamp);
      int32_t dt1 = txTimestamp_us - scheduleMarker;
      int32_t dt2 = txDoneMarker - txTimestamp_us;
      int32_t dt3 = returnMarker - txDoneMarker;

      // How many microseconds (us) passed between scheduling and 
      // transmitting the Start of Frame Delimiter (SFD)?
      Serial.print("It took %i us to transmit the SFD\n", dt1);

      // How many microseconds passed between transmitting the SFD 
      // and receiving the UWB_TX_DONE interrupt?
      Serial.print("It then took %i us to trigger UWB_TX_DONE\n", dt2);

      // How many microseconds passed between the UWB_TX_DONE interrupt
      // and returing to the application?
      Serial.print("It then took %i us to return\n", dt3);

      // How well are the CPU clock and Radio Circuitry clock in sync?
      uint32_t t_1 = micros();
      uint32_t t_2 = TICKS_TO_MICROS(*ticks());
      Serial.print("Current CPU vs UWB time: %i vs %i = %i us\n", \
      t_1, t_2, t_2 - t_1);

    } break;

    case BUTTON_EVENT: {
      Serial.print("Button Pressed\n");
    }
  }
}

