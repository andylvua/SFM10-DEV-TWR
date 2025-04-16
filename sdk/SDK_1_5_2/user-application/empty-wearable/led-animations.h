
#ifndef INDICATOR_LEDS_H
#define INDICATOR_LEDS_H



#include <stdint.h>
#include <stdbool.h>


void led_showBreathing(uint16_t duration, uint16_t gpio);

void led_showStrobo(uint16_t total_duration, uint16_t flash_duration, uint16_t gpio);

void led_showNotification(uint16_t gpio);

void led_showStatic(uint16_t total_duration, uint16_t gpio);

#endif