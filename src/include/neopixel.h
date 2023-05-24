#ifndef __NEOPIXEL_H__
#define __NEOPIXEL_H__

static char VERSION[] = "XX.YY.ZZ";

#include<DBASEV/visibility.h>

#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <signal.h>
#include <stdarg.h>
#include <getopt.h>
#include <iostream>

#include <ws2811/clk.h>
#include <ws2811/gpio.h>
#include <ws2811/dma.h>
#include <ws2811/pwm.h>
#include <ws2811/version.h>
#include <ws2811/ws2811.h>

#define TARGET_FREQ             WS2811_TARGET_FREQ
#define GPIO_PIN_18             18
#define GPIO_PIN_13             13
#define DMA                     10
#define STRIP_TYPE              WS2811_STRIP_GBR 
#define LED_COUNT               9

#define RED 0x00000020
#define BLUE 0x00200000



ws2811_t initNeopixel();
ws2811_t matrix_render_red(ws2811_t ledstring);
ws2811_t matrix_render_blue(ws2811_t ledstring);
ws2811_t matrix_clear(ws2811_t ledstring);

#endif