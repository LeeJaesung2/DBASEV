#include <DBASEV/neopixel.h>


ws2811_return_t ret;

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
    

    sprintf(VERSION, "%d.%d.%d", VERSION_MAJOR, VERSION_MINOR, VERSION_MICRO);

    if ((ret = ws2811_init(&ledstring)) != WS2811_SUCCESS) {
        fprintf(stderr, "ws2811_init failed: %s\n", ws2811_get_return_t_str(ret));
        return ret;
    }
    return ledstring;
}
ws2811_t matrix_render_red(ws2811_t ledstring){
    for (int x = 0; x < LED_COUNT; x++) {
        
        ledstring.channel[0].leds[x] = RED; // Pin 18
        ledstring.channel[1].leds[x] = BLUE; // Pin 15 
    }
    if ((ret = ws2811_render(&ledstring)) != WS2811_SUCCESS) {
        fprintf(stderr, "ws2811_render failed: %s\n", ws2811_get_return_t_str(ret));
        break;
    }
    return ledstring;
}
ws2811_t matrix_render_blue(ws2811_t ledstring){
    for (int x = 0; x < LED_COUNT; x++) {
        
        ledstring.channel[0].leds[x] = BLUE; // Pin 18
        ledstring.channel[1].leds[x] = RED; // Pin 15 
    }
    if ((ret = ws2811_render(&ledstring)) != WS2811_SUCCESS) {
        fprintf(stderr, "ws2811_render failed: %s\n", ws2811_get_return_t_str(ret));
        break;
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