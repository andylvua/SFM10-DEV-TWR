
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

/* Example output:
sht30_connected = 1
sht30_connected = 0
Measured: 24.6 deg C, 61.2 %RH
From 0xF05BB7F6: 24.6 deg C, 61.2 %RH
Measured: 24.6 deg C, 60.9 %RH
From 0xF05BB7F6: 24.6 deg C, 60.9 %RH
Measured: 24.6 deg C, 60.5 %RH
From 0xF05BB7F6: 24.6 deg C, 60.5 %RH
Measured: 24.6 deg C, 60.1 %RH
From 0xF05BB7F6: 24.6 deg C, 60.1 %RH
Measured: 24.6 deg C, 59.8 %RH
From 0xF05BB7F6: 24.6 deg C, 59.8 %RH
Measured: 24.5 deg C, 59.5 %RH
From 0xF05BB7F6: 24.5 deg C, 59.5 %RH
*/

#define ADDRESS_SHT30 (0x45)


// ======================[ DATA STRUCTURES ]====================== //

typedef struct sht30_payload_t {
    uint32_t serial_nr;
    int16_t temp_x10;
    int16_t humidity_x10;
} sht30_payload_t;


// ======================[ DATA CONTAINERS ]====================== //

static uint8_t uwb_buffer[3+sizeof(sht30_payload_t)];
bool sht30_connected = false;
uint32_t my_serial;

// ======================[ SETUP and LOOP ]====================== //

void setup() {
    // Start UART and I2C and detect SHT30:
    Serial.begin(115200);
    attachInterrupt(ERROR_INTERRUPT, Serial.printError, 0);
    I2C.begin(D2, D1);
    sht30_connected = I2C.write(ADDRESS_SHT30, 0, 0, true) \
        == WRITE_RESULT_ACK;
    Serial.print("sht30_connected = %u\n", sht30_connected);

    my_serial = getSerialNumber();
    ASSERT_0(UWB.begin());
}

void loop() {
    sht30_payload_t* plData;

    if (sht30_connected) {
        // Do measurement (see sht30 example)
        uint8_t data[6] = {0x2C, 0x06}; 
        I2C.write(ADDRESS_SHT30, data, 2, true);
        delay(500);
        I2C.read(ADDRESS_SHT30, data, 6, true);
        // Construct Payload (see sht30_payload_t above)
        SetPayloadCmd* payload = (SetPayloadCmd*)uwb_buffer;
        payload->payloadLength = sizeof(sht30_payload_t);
        plData = (sht30_payload_t*)payload->payloadData;
        plData->serial_nr = my_serial;
        plData->temp_x10 = (((((uint32_t)data[0] << 8) + \
            (uint32_t)data[1]) * 1750) / 65535) - 450;
        plData->humidity_x10 = (((((uint32_t)data[3] << 8) + \
            (uint32_t)data[4]) * 1000) / 65535);
        // Pass payload and transmit UWB packet
        ASSERT_0(UWB.setPayload(payload));
        UWB.onTxUpdate(TX_DONE, 0);
        ASSERT_0(UWB.startTx(0));
        // Wait until TX was success:
        uint32_t eventID = waitForEvent(0);
        if (eventID != (uint32_t)TX_DONE) {
            return;
        }
    } else {
        // Target payload buffer
        PayloadNtf* payload = (PayloadNtf*)uwb_buffer;
        UWB.onRxUpdate(RX_DONE, 0, payload, \
            sizeof(sht30_payload_t), 0, 0);
        
        // Reset and start RX
        UWB.stopAction();
        ASSERT_0(UWB.startRx(0));
        uint32_t eventID = waitForEvent(0);
        if (eventID != (uint32_t)RX_DONE || \
            payload->payloadLength != sizeof(sht30_payload_t)) {
            // Got something else, restart RX
            return;
        }
        // If we get here, RX was success
        plData = (sht30_payload_t*)payload->payloadData;
    }
    // Print reading/received payload:
    if (!sht30_connected) {
        Serial.print("From 0x%08X: ",  plData->serial_nr);
    } else {
        Serial.print("Measured: ");
    }
    Serial.print("%i.%u deg C, ", \
        plData->temp_x10/10, plData->temp_x10%10);
    Serial.print("%i.%u %%RH\n", \
        plData->humidity_x10/10, plData->humidity_x10%10);
}

