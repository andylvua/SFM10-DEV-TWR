
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

// Do-Re-Mi-Fa-So-La-Ti-Do frequencies (Hz):
uint16_t frequencys[] = {262, 294, 330, 349, 392, 440, 494, 523};
uint16_t fIdx = 0; // To keep track of current pitch

// Function to-be-called to create a PWM signal
void buzz() {
  static bool buzz_state = false;
  digitalWrite(D5, buzz_state);
  buzz_state = !buzz_state;
}

// Forward declare the button interrupt handlers:
void btn_down_isr(void);
void btn_up_isr(void);

// In our Setup, we only initialize the user (D3) button
void setup() {
  pinMode(PIN_USER_BUTTON, INPUT_PULLUP); // setup button pin
  attachInterrupt(PIN_USER_BUTTON, btn_down_isr, FALLING);
}

// When button down, then a PWM signal is started at pin D5
void btn_down_isr(void) {
  // Start listening for button release event:
  detachInterrupt(PIN_USER_BUTTON);
  attachInterrupt(PIN_USER_BUTTON, btn_up_isr, RISING);

  // Set D5 as OUTPUT PIN to start the pull/push of the speaker
  pinMode(D5, OUTPUT_HIGH);
  // Calculate the half-period and set the timer:
  uint16_t half_period = (1000000/2) / frequencys[fIdx];
  Timer.everyMicroseconds(half_period, buzz);

  // Setup for next tone:
  fIdx = fIdx + 1;
  if (fIdx >= sizeof(frequencys) / sizeof(uint16_t)) {
      fIdx = 0;
  }
}

// On button release, the tone is stopped
void btn_up_isr(void) {
  // Start listening for button press event again:
  detachInterrupt(PIN_USER_BUTTON);
  attachInterrupt(PIN_USER_BUTTON, btn_down_isr, FALLING);
  Timer.end();

  // Stop the pull/push to prevent current flow to/from 
  // the speaker:
  pinMode(D5, INPUT);
}

