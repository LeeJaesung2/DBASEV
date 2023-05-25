#ifndef __ANNOUNCE_H__
#define __ANNOUNCE_H__
#include "visibility.h"
#include "neopixel.h"

#define RELAY_PIN 17 // using GPIO pin

//control led and piezo buzzer
int announceOn();
void announceOff(ws2811_t ledstring);
#endif
