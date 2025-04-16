

#include "user-application.h"

// we can disable specific notifications like this:
#define ENABLE_CIR_NTF 0

#include "phy-uci-interpreter.h"
// please see: documentation\uci-for-host.html

uint8_t buffer_to_uci[UCI_PACKET_LENGTH_MAX];
uint8_t buffer_from_uci[UCI_PACKET_LENGTH_MAX];

void UCI_ntfReady(void) ;


ADD_NOTIFICATION_CONTAINERS
ADD_NOTIFICATION_ISR

void UCI_ntfReady() {
    Serial.write(buffer_from_uci, 4 + GET_UCI_LEN(buffer_from_uci));
}

void uart_to_uci() {
    uint16_t len = Serial.readBytes(buffer_to_uci, 4); // read header first
    if (len < 4) {
        return;
    }
    len = GET_UCI_LEN(buffer_to_uci);
    if (len > 0) {
        Serial.readBytes(&buffer_to_uci[4],  len);    
    }

    UCI_execute(buffer_to_uci, buffer_from_uci);
    // return response:
    Serial.write(buffer_from_uci, 4 + GET_UCI_LEN(buffer_from_uci));
}

void setup() {
    Serial.begin(115200);
    SET_NOTIFICATION_HANDLERS;
}

void loop() {
    if (Serial.available()) {
        uart_to_uci();
    }
}