
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

// Forward declaration of our periodic TX function
void send_poll();

// In setup, we configure and start the UWB
void setup() {
  // Start UART and UWB class
  Serial.begin(115200);

  // We start off with the default settings
  // before we change some parameters:
  SetConfigurationCmd config;
  SetConfiguration_setDefaults(&config);

  // Let's user radio cfg 1: STS + payload:
  config.rxRadioCfg = 1; 
  config.txRadioCfg = 1;

  // We will use an AES key for the STS encryption
  // The key we'll store at index 1
  config.stsKeyIdx = 1;

  // And let's use CHAN 9, which is centered around ~8 GHz
  config.channelFrequency = UWB_CHANNEL_9;

  // Apply the configuration:
  ASSERT(UWB.setConfiguration(&config), SUCCESS);

  // Import AES key for index 1:
  ImportKeyCmd p = {
      .keyIdx = 1,
      .keyUsage = UWB_DEVICE_IMPORT_KEY_CMD_PAYLOAD_STS_GENERATION,
      .rawKey = {
      0x01, 0x02, 0x03, 0x04,
      0x01, 0x02, 0x03, 0x04,
      0x01, 0x02, 0x03, 0x04,
      0x01, 0x02, 0x03, 0x04,
      }
  };
  ASSERT(UWB.importKey(&p), SUCCESS);


  // Now the settings are imported we can start the UWB circuitry:
  
  ASSERT(UWB.begin(), SUCCESS);

  // Most settings are allowed to be changed while the session is running

  // Let's periodically transmit a 'poll' packet:
  Timer.begin();
  Timer.every(1000, send_poll);
  
  // Setup only the rx/tx event triggers:
  UWB.onRxUpdate(RX_DONE, 0, 0, 0, 0, 0);
  UWB.onTxUpdate(TX_DONE, 0);
}

void loop() {
  //  By default, we listen for poll messages
  ASSERT(UWB.startRx(0), SUCCESS);
  switch (waitForEvent(0)) // we wait
  {
    case (uint32_t)RX_DONE: { // poll received
      Serial.print("Received message!\n");
    } break;
    case (uint32_t)TX_DONE: { // or timed TX
      Serial.print("Sent message!\n");
    } break;
    
    default:
    break;
  }
}

void send_poll() {
  // Stop listening
  ASSERT(UWB.stopAction(), SUCCESS);

  // Make some (arbitary) payload
  uint8_t buffer[10];
  SetPayloadCmd* cmd = (SetPayloadCmd*)buffer;
  cmd->payloadLength = 1;
  ASSERT(UWB.setPayload(cmd), SUCCESS);

  // And Transmit
  ASSERT(UWB.startTx(0), SUCCESS);
}

