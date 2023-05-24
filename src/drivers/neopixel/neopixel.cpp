#include <DBASEV/neopixel.h>




ws2811_t initNeopixel(){
    ws2811_t ledstring =
    {
        .freq = TARGET_FREQ,
        .dmanum = DMA,
        .channel =
        {
            [0] =
            {
                .gpionum = GPIO_PIN_18,
                .invert = 0,
                .count = LED_COUNT,
                .strip_type = STRIP_TYPE,
                .brightness = 255,
            },
            [1] =
            {
                .gpionum = GPIO_PIN_13,
                .invert = 0,
                .count = LED_COUNT,
                .strip_type = STRIP_TYPE,
                .brightness = 255,
            },
        },
    };
    return ledstring;
}
ws2811_t matrix_render_red(ws2811_t ledstring){
    for (int x = 0; x < LED_COUNT; x++) {
        
        ledstring.channel[0].leds[x] = RED; // Pin 18
        ledstring.channel[1].leds[x] = BLUE; // Pin 15 
    }
    return ledstring;
}
ws2811_t matrix_render_blue(ws2811_t ledstring){
    for (int x = 0; x < LED_COUNT; x++) {
        
        ledstring.channel[0].leds[x] = BLUE; // Pin 18
        ledstring.channel[1].leds[x] = RED; // Pin 15 
    }
    return ledstring;
}
ws2811_t matrix_clear(ws2811_t ledstring){
    for (int x = 0; x < LED_COUNT; x++) {
        
        ledstring.channel[0].leds[x] = 0; // Pin 18
        ledstring.channel[1].leds[x] = 0; // Pin 15 
    }
    return ledstring;
}