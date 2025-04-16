
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


void setup() {
    attachInterrupt(ERROR_INTERRUPT, Serial.printError, 0);
    Serial.begin(115200);
    
    Serial.print("Reset reason = ");

    switch (getResetReason())
    {
        case POWER_ON:              Serial.print("POWER_ON"); break;
        case RESET_PIN:             Serial.print("RESET_PIN"); break;
        case DPD_RESET_PIN:         Serial.print("DPD_RESET_PIN"); break;
        case HPD_RESET_PIN:         Serial.print("HPD_RESET_PIN"); break;
        case HPD_WAKE_UP_PIN:       Serial.print("HPD_WAKE_UP_PIN"); break;
        case SOFTWARE_RESET:        Serial.print("SOFTWARE_RESET"); break;
        case FAIL_BIAS_BANDGAP_VDD: Serial.print("FAIL_BIAS_BANDGAP_VDD"); break;
        case FAIL_CPU_VDD:          Serial.print("FAIL_CPU_VDD"); break;
        case BROWNOUT_CPU:          Serial.print("BROWNOUT_CPU"); break;
        case FAIL_PFLASH_VDD:       Serial.print("FAIL_PFLASH_VDD"); break;
        case FAIL_BUF_LOW_VDD:      Serial.print("FAIL_BUF_LOW_VDD"); break;
        case WATCHDOG:              Serial.print("WATCHDOG"); break;
        case FAIL_STATE_CHANGE:     Serial.print("FAIL_STATE_CHANGE"); break;
        case WUP_TIMER:             Serial.print("WUP_TIMER"); break;
        case WUP_SNAPSHOT:          Serial.print("WUP_SNAPSHOT"); break;
        case DPD_GPIO:        Serial.print("DPD_GPIO"); break;
        default: break;
    }
    Serial.print("\n");

    switch (getResetReason()) {
        case RESET_PIN: {
            delay(1000); 
            reset(); // trigger SOFTWARE_RESET
        } break; 
        case SOFTWARE_RESET: {
            Timer.setWatchdog(1000); // trigger WATCHDOG
        } break; 
        case WATCHDOG: {
            delay(10); // allow Serial.print() to complete
            sleep(1000, 0, 0); // trigger WUP_TIMER
        } break; 
        case WUP_TIMER: {
            Serial.print("Press the User Button to continue...\n");
            delay(10); 
            sleep(0, D3, FALLING); // trigger DPD_GPIO
        } break; 
        case DPD_GPIO: {
            Serial.print("Send me some serial character(s)\n");
            delay(10); 
            deepSleep(PIN_RX); // trigger HPD_WAKE_UP_PIN
        } break;
        case HPD_WAKE_UP_PIN: {
            Serial.print("Done\n");
        }
    }

    Serial.print("You reached this line\n");
}

/* example output:

Reset reason = RESET_PIN
Reset reason = SOFTWARE_RESET
You reached this line
Reset reason = WATCHDOG
Reset reason = WUP_TIMER
Press the User Button to continue...
Reset reason = DPD_GPIO
Send me some serial character(s)
TO COM6: test
Reset reason = HPD_WAKE_UP_PIN
Done
You reached this line

*/

