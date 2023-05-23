#ifndef __ANNOUNCE_H__
#define __ANNOUNCE_H__
#include "visibility.h"
#include <ws2811/ws2811.h>

#define LED_PIN1 1
#define LED_PIN2 2
#define UPSIDE_LED_COUNT 9
#define SIDE_LED_COUNT 25
#define BUZZER_RELAY_PIN 3

ws2811_t ledStrip;

uint32_t red = 0x00FF0000;
uint32_t blue = 0x000000FF;

enum{
    mode1,
    mode2
};
#endif

//control led and piezo buzzer
void announceOn();
void announceOff();
