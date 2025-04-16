
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

// We always start with including the user-application header file:
#include "user-application.h"

// Let's define some MACRO functions that turn on/off a LED

// The LEDs on the SFM10-DEV board are active low, meaning that we 
// have to set the GPIO pin to LOW when the LED must be lit and 
// HIGH when it must be off.
#define ON LOW
#define OFF HIGH
#define TURN_RED_LED(value)   { digitalWrite(PIN_LED_R, value); }
#define TURN_GREEN_LED(value) { digitalWrite(PIN_LED_G, value); }
// Now we can write this for example: 
//   TURN_RED_LED(ON);
// Which now is an alternative notication for:
//   digitalWrite(PIN_LED_R, LOW);


// Our setup, that runs only once:
void setup() {
  // We configure the GPIO pins that control both LEDs.
  // We can 'or' the pins together to make it a single call to 
  // pinMode(). We set them both as OUTPUT pins with the initial 
  // state HIGH, meaning the LEDs are off:
  pinMode(PIN_LED_R | PIN_LED_G, OUTPUT_HIGH);
}

// Our loop, that runs continuously:
void loop() {
  // We turn on the red LED for 500 ms:
  TURN_RED_LED(ON);
  delay(500);
  TURN_RED_LED(OFF);
  // We wait 200 ms:
  delay(200);
  // Then we turn on the green LED for 100 ms:
  TURN_GREEN_LED(ON);
  delay(100);
  TURN_GREEN_LED(OFF);
  // We wait 1 second:
  delay(200);  
}

