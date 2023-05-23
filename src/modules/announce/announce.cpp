#include <DBASEV/announce.h>

int initLed(){
    printf("LED init\n");
    #ifndef DEBUG
        cout << "announce.cpp" << endl;
    #endif
    ledStrip.freq = WS2811_TARGET_FREQ;
    ledStrip.dmanum = 5;
    ledStrip.channel[0].gpionum = LED_PIN1;
    ledStrip.channel[0].count = SIDE_LED_COUNT;
    ledStrip.channel[0].brightness = 255;
    ledStrip.channel[0].strip_type = WS2811_STRIP_GRB;
    ledStrip.channel[1].gpionum = LED_PIN1;
    ledStrip.channel[1].count = SIDE_LED_COUNT;
    ledStrip.channel[1].brightness = 255;
    ledStrip.channel[1].strip_type = WS2811_STRIP_GRB;
    ledStrip.channel[2].gpionum = LED_PIN1;
    ledStrip.channel[2].count = UPSIDE_LED_COUNT;
    ledStrip.channel[2].brightness = 255;
    ledStrip.channel[2].strip_type = WS2811_STRIP_GRB;
    ledStrip.channel[3].gpionum = LED_PIN1;
    ledStrip.channel[3].count = UPSIDE_LED_COUNT;
    ledStrip.channel[3].brightness = 255;
    ledStrip.channel[3].strip_type = WS2811_STRIP_GRB;

    if(ws2811_init(&ledStrip) != WS2811_SUCCESS){
        std::cerr << "Failed to initialize WS2812 LED strip!" << std::endl;
         #ifndef DEBUG
            cout << "announce.cpp" << endl;
        #endif
        return -1;
    }
    return 0;
}




void initBuzzer(){
    printf("buzzer init\n");
    #ifndef DEBUG
        cout << "announce.cpp" << endl;
    #endif
}

void ledOn(int flag){
    printf("LED ON\n");
    #ifndef DEBUG
        cout << "announce.cpp" << endl;
    #endif
    if(flag == mode1){
        ledStrip.channel[0].leds[0] = red;
        ledStrip.channel[1].leds[0] = blue;
        ledStrip.channel[2].leds[0] = red;
        ledStrip.channel[3].leds[0] = blue;
    }
    else{
        ledStrip.channel[0].leds[0] = blue;
        ledStrip.channel[1].leds[0] = red;
        ledStrip.channel[2].leds[0] = blue;
        ledStrip.channel[3].leds[0] = red;
    }
    ws2811_render(&ledStrip);
    usleep(1000);
}

void buzzerOn(){
    printf("buzzer ON\n");
    #ifndef DEBUG
        cout << "announce.cpp" << endl;
    #endif
}

void ledOff(){
    printf("LED OFF\n");
    #ifndef DEBUG
        cout << "announce.cpp" << endl;
    #endif
    ws2811_fini(&ledStrip);
}

void buzzerOff(){
    printf("buzzer OFF\n");
    #ifndef DEBUG
        cout << "announce.cpp" << endl;
    #endif
}

//control led and piezo buzzer
void announceOn(){
    initLed();
    initBuzzer();
    while(true){
        ledOn(mode1);
        buzzerOn();
        ledOn(mode2);
    }
}

void announceOff(){
    ledOff();
    buzzerOff();
}