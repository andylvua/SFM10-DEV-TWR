
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

static uint8_t tx_buf1[2] = { 0x01, 0x02 };
static uint8_t tx_buf2[2] = { 0x03, 0x04 };

void setup() { // demonstrating MASTER mode
    SPI.begin();

    // send 0x01 0x02 and 0x03, 0x04 as SPI master,
    // while keeping slave select pin asserted inbetween
    // the transfers
    SPI.beginTransaction ( 
      SPISettings(2000000, MSBFIRST, SPI_MODE0 | SPI_CS1 | SS_HOLD_ACTIVE)
    );             
    // we are not interested in rx, hence NULL:
    SPI.transfer(tx_buf1, NULL, 2); 
    SPI.transfer(tx_buf2, NULL, 2);

    SPI.endTransaction(); // will deassert slave select pin
        // which happens not automatically with SPI.transfer 
        // here because SS_HOLD_ACTIVE was set
}

