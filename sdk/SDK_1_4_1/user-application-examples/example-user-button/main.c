
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

// We always start with including the user-application header file:
#include "user-application.h"

// The LEDs on the SFM10-DEV board are active low, meaning that we 
// have to set the GPIO pin to LOW when the LED must be lit and 
// HIGH when it must be off.
#define ON LOW
#define OFF HIGH
#define TURN_RED_LED(value)   { digitalWrite(PIN_LED_R, value); }


// Forward declaration of function, so we can 'use' them before they 
// are defined:
void btn_down_isr(void);
void btn_up_isr(void);


void setup() {
    // We initialize the LED pins and the UART:
    pinMode(PIN_LED_R | PIN_LED_G, OUTPUT_HIGH);
    Serial.begin(115200);
    // In case an error occurs, errno will change its value. 
    // Using the line below we can catch this event:
    attachInterrupt(ERROR_INTERRUPT, Serial.printError, 0);

    pinMode(PIN_USER_BUTTON, INPUT_PULLUP); // setup button pin

    attachInterrupt(PIN_USER_BUTTON, btn_down_isr, FALLING);
}


void btn_down_isr(void) {

    detachInterrupt(PIN_USER_BUTTON);
    attachInterrupt(PIN_USER_BUTTON, btn_up_isr, RISING);

    TURN_RED_LED(ON);

}

void btn_up_isr(void) {

    detachInterrupt(PIN_USER_BUTTON);
    attachInterrupt(PIN_USER_BUTTON, btn_down_isr, FALLING);
    
    TURN_RED_LED(OFF);
}

