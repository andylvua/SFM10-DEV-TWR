
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


#define PRINT_HEX(name) \
    Serial.print(#name "\t= 0x%X\n", info.name);
    
#define PRINT_UINT(name) \
    Serial.print(#name "\t= %u\n", info.name);

#define PRINT_VERSION(name) \
    Serial.print(#name "Version\t= %u.%u\n", \
    info.name ## MajorVersion, info.name ## MinorVersion);
    

void setup() {
    Serial.begin(115200);    
    GetDeviceInfoRsp info;
    UWB.getDeviceInfo(&info);  
    PRINT_UINT(uciStatus)            
    PRINT_HEX(serialNumber)
    PRINT_VERSION(app)
    PRINT_VERSION(phy)
    PRINT_VERSION(chip)
    PRINT_VERSION(rom)
    PRINT_VERSION(baseband)
    PRINT_VERSION(dsp)
    Serial.print("\n");
}

/* example output:
uciStatus       = 0
uciVersion      = 1.16
macVersion      = 1.48
phyVersion      = 1.48
testVersion     = 0.0
vsiLength       = 10
serialNumber    = 0xF05BB7FD
appVersion      = 0.3
chipVersion     = 22.15
romVersion      = 4.0
basebandVersion = 2.23
dspVersion      = 2.20
*/

