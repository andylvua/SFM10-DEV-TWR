
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

// We always start with including the user-application header file:
#include "user-application.h"

void setup() {
    // We initialize the LED pins and the UART:
    pinMode(PIN_LED_R | PIN_LED_G, OUTPUT_HIGH);
    Serial.begin(115200);
    // In case an error occurs, errno will change its value. 
    // Using the line below we can catch this event:
    attachInterrupt(ERROR_INTERRUPT, Serial.printError, 0);

    // We start the hardware timer class:
    Timer.begin();

    // We start the UWB circuitry which activates the XTAL, providing a 
    // precise clock. This can be omited to reduce power consumption:
    UWB.begin();
}

// Instead of defining MACROs like we did with blinky-led, we 
// create explicit functions for the LED states:
void redOn() {  
    digitalWrite(PIN_LED_R, LOW); 
}

void redOff() {
    digitalWrite(PIN_LED_R, HIGH); 
}

void greenOn() {
    digitalWrite(PIN_LED_G, LOW); 
}

void greenOff() {
    digitalWrite(PIN_LED_G, HIGH); 
}

// The function defined below will be our last state of the 
// pattern, it will let our scheduler know that we are done 
// (see waitForEvent):
void proceed() {
    triggerEvent(0);
}


// Our loop, that runs continuously:
void loop() {
    static uint32_t iteration = 0;
    // We keep count of how many times we come here:
    iteration++;

    // Based on the iteration count, we set the (absolute) 
    // start time in milliseconds:
    uint32_t startTime = 1000 * iteration;

    // Using the Timer class, we schedule the LED blinking
    // pattern: 
    Timer.at(startTime +   0, redOn);
    Timer.at(startTime + 500, redOff);
    Timer.at(startTime + 700, greenOn);
    Timer.at(startTime + 800, greenOff);
    Timer.at(startTime + 900, proceed);

    waitForEvent(0);
    // We end up here only after triggerEvent() is called 
    // somewhere, making sure the next blinking pattern is 
    // scheduled only when the previous is finished.
}

