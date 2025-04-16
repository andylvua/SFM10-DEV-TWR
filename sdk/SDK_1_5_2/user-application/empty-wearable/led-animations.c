

#include "led-animations.h"
#include "user-application.h"



void led_showBreathing(uint16_t duration, uint16_t gpio) {

  uint16_t h = duration / 2;

  for (int i = 0; i < h; i++) {
    uint32_t tEnd = millis() + 1;
    while (millis() < tEnd) {
      int k = (h-1) - i;
      uint16_t dur = (k*k)/duration;
      digitalWrite(gpio, LOW);
      delayMicroseconds(dur);
      digitalWrite(gpio, HIGH);
      delayMicroseconds(duration-dur);
    }
  }
 for (int i = 0; i < h; i++) {
    uint32_t tEnd = millis() + 1;
    while (millis() < tEnd) {
      int k = i;
      uint16_t dur = (k*k)/duration;
      digitalWrite(gpio, LOW);
      delayMicroseconds(dur);
      digitalWrite(gpio, HIGH);
      delayMicroseconds(duration-dur);
    }
  }
}



void led_showStrobo(uint16_t total_duration, uint16_t flash_duration, uint16_t gpio) {
    uint32_t tEnd = millis() + total_duration;
    uint16_t h = flash_duration / 2;
    while (millis() < tEnd) {
        digitalWrite(gpio, LOW);
        delay(h);
        digitalWrite(gpio, HIGH);
        delay(h);
    }
}


void led_showStatic(uint16_t total_duration, uint16_t gpio) {
    uint32_t tEnd = millis() + total_duration;
    digitalWrite(gpio, LOW);
    while (millis() < tEnd) {
        delay(1);
    }
    digitalWrite(gpio, HIGH);
}


void led_showNotification( uint16_t gpio) {
    digitalWrite(gpio, LOW);
    delay(300);
    digitalWrite(gpio, HIGH);
    delay(300);
    digitalWrite(gpio, LOW);
    delay(300);
    digitalWrite(gpio, HIGH);
    delay(1000);
}
