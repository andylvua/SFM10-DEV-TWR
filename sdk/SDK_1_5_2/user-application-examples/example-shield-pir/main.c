
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
0
0
0
0
motion_detected
1
1
1
1
no_motion_anymore
0
0
0
0
0
*/

// Some macro's to turn on/off the red onboard LED:
#define ON LOW
#define OFF HIGH
#define TURN_RED_LED(value)   { digitalWrite(PIN_LED_R, value); }


// Forward declare the interrupt handlers:
void motion_detected(void);
void no_motion_anymore(void);

// Procedure on motion detect:
void motion_detected(void) {
  // Print function name:
  Serial.print("%s\n", __FUNCTION__);
  // Start listening for when D3 goes LOW:
  detachInterrupt(D3);
  attachInterrupt(D3, no_motion_anymore, FALLING);
  // Turn on LED to indicate motion:
  TURN_RED_LED(ON);
}

// Procedure when no motion anymore:
void no_motion_anymore(void) {
  Serial.print("%s\n", __FUNCTION__);
  // Start listening for when D3 goes HIGH:
  detachInterrupt(D3);
  attachInterrupt(D3, motion_detected, RISING);
  // Turn off LED:
  TURN_RED_LED(OFF);
}


void setup() {
  Serial.begin(115200);
  // Setup LED output and PIR input pins:
  pinMode(PIN_LED_R, OUTPUT_HIGH);
  pinMode(D3, INPUT); // D3 = PIR pin
  attachInterrupt(D3, motion_detected, RISING);
}

// Print status of PIR pin twice per second:
void loop() {
  Serial.print("%u\n", digitalRead(D3));
  delay(500);
}

