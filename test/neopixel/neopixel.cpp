#include <iostream>
#include <wiringPi.h>
#include <ws2811/ws2811.h>

#define LED_PIN_1 18         // GPIO pin connected to the first LED
#define LED_PIN_2 15         // GPIO pin connected to the second LED
#define LED_COUNT 1          // Number of LEDs in each strip
#define LED_BRIGHTNESS 255   // Set the brightness (0-255)

int main() {
    if (wiringPiSetupGpio() == -1) {
        std::cerr << "Failed to initialize WiringPi!" << std::endl;
        return -1;
    }

    ws2811_t ledStrip;
    ledStrip.freq = WS2811_TARGET_FREQ;
    ledStrip.dmanum = 5;
    ledStrip.channel[0].gpionum = LED_PIN_1;
    ledStrip.channel[0].count = LED_COUNT;
    ledStrip.channel[0].brightness = LED_BRIGHTNESS;
    ledStrip.channel[0].strip_type = WS2811_STRIP_GRB;
    ledStrip.channel[1].gpionum = LED_PIN_2;
    ledStrip.channel[1].count = LED_COUNT;
    ledStrip.channel[1].brightness = LED_BRIGHTNESS;
    ledStrip.channel[1].strip_type = WS2811_STRIP_GRB;

    if (ws2811_init(&ledStrip) != WS2811_SUCCESS) {
        std::cerr << "Failed to initialize WS2812 LED strip!" << std::endl;
        return -1;
    }

    uint32_t redColor = 0x00FF0000;    // Red color
    uint32_t blueColor = 0x000000FF;   // Blue color

    while (true) {
        ledStrip.channel[0].leds[0] = redColor;
        ledStrip.channel[1].leds[0] = blueColor;
        ws2811_render(&ledStrip);
        delay(1000);  // Delay between toggling the pixels

        ledStrip.channel[0].leds[0] = blueColor;
        ledStrip.channel[1].leds[0] = redColor;
        ws2811_render(&ledStrip);
        delay(1000);  // Delay between toggling the pixels
    }

    ws2811_fini(&ledStrip);
    return 0;
}
