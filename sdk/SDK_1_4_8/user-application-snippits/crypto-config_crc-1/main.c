
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
    Crypto.begin();
    
    uint8_t data[] = { 0x12, 0x34, 0x56, 0x78 };
    Serial.print("Input: ");
    Serial.hexDump(data, sizeof(data), 16);

    // standard CRC-32 (default)
    Crypto.configCrc(0x4C11DB7, 0xFFFFFFFF, true, 0xFFFFFFFF);
    Serial.print("CRC-32 = 0x%X\n", \
        Crypto.calcCrc(data, sizeof(data)));

    // CRC-32/MPEG-2
    Crypto.configCrc(0x4C11DB7, 0xFFFFFFFF, false, 0x00000000);
    Serial.print("MPEG-2 = 0x%X\n", \
        Crypto.calcCrc(data, sizeof(data)));

    // standard CRC-8 (note the bitshift in Poly and result)
    Crypto.configCrc(0x07 << 24, 0x00, false, 0x00);
    Serial.print("CRC-8 = 0x%X\n", \
        Crypto.calcCrc(data, sizeof(data)) >> 24);

    // CRC-8/MAXIM (note the bitshift in Poly but not in result)
    Crypto.configCrc(0x31 << 24, 0x00, true, 0x00);
    Serial.print("MAXIM-8 = 0x%X\n", \
        Crypto.calcCrc(data, sizeof(data)));

    // CRC-16/XMODEM (note the bitshift in Poly and result)
    Crypto.configCrc(0x1021 << 16, 0x00, false, 0x00);
    Serial.print("XMODEM-16 = 0x%X\n", \
        Crypto.calcCrc(data, sizeof(data)) >> 16);

    // CRC-16/KERMIT (note bitshift in Poly only)
    Crypto.configCrc(0x1021 << 16, 0x00000000, true, 0x00000000);
    Serial.print("KERMIT-16 = 0x%X\n", \
        Crypto.calcCrc(data, sizeof(data)));
}

