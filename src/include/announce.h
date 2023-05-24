#ifndef __ANNOUNCE_H__
#define __ANNOUNCE_H__
#include "visibility.h"
#include "neopixel.h"
#include <ws2811/ws2811.h>

//control led and piezo buzzer
int announceOn();
void announceOff(ws2811_t ledstring);
#endif
