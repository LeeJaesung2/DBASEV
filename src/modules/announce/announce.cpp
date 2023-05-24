#include <DBASEV/announce.h>





void initBuzzer(){
    printf("buzzer init\n");
    #ifndef DEBUG
        cout << "announce.cpp" << endl;
    #endif
}



void buzzerOn(){
    printf("buzzer ON\n");
    #ifndef DEBUG
        cout << "announce.cpp" << endl;
    #endif
}

void ledOff(ws2811_t ledstring){
    printf("LED OFF\n");
    #ifndef DEBUG
        cout << "announce.cpp" << endl;
    #endif
    matrix_clear(ledstring);
    ws2811_render(&ledstring);
     ws2811_fini(&ledstring);
}

void buzzerOff(){
    printf("buzzer OFF\n");
    #ifndef DEBUG
        cout << "announce.cpp" << endl;
    #endif
}

//control led and piezo buzzer
int announceOn(){
    ws2811_t ledstring = initNeopixel();
    ws2811_return_t ret;

    sprintf(VERSION, "%d.%d.%d", VERSION_MAJOR, VERSION_MINOR, VERSION_MICRO);

    if ((ret = ws2811_init(&ledstring)) != WS2811_SUCCESS) {
        fprintf(stderr, "ws2811_init failed: %s\n", ws2811_get_return_t_str(ret));
        return ret;
    }

    initBuzzer();
    while(true){
        //matrix_mode1();
        ledstring = matrix_render_red(ledstring);

        if ((ret = ws2811_render(&ledstring)) != WS2811_SUCCESS) {
            fprintf(stderr, "ws2811_render failed: %s\n", ws2811_get_return_t_str(ret));
            break;
        }

        usleep(1000000);

        //matrix_mode2();
        ledstring = matrix_render_blue(ledstring);

        if ((ret = ws2811_render(&ledstring)) != WS2811_SUCCESS) {
            fprintf(stderr, "ws2811_render failed: %s\n", ws2811_get_return_t_str(ret));
            break;
        }

        usleep(1000000);
    }
    return 0;
}

void announceOff(ws2811_t ledstring){
    ledOff(ledstring);
    buzzerOff();
}