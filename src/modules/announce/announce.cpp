#include <DBASEV/announce.h>

void initLed(){
    printf("LED init\n");
    #ifndef DEBUG
        cout << "announce.cpp" << endl;
    #endif
}

void initBuzzer(){
    printf("buzzer init\n");
    #ifndef DEBUG
        cout << "announce.cpp" << endl;
    #endif
}

void ledOn(){
    printf("LED ON\n");
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

void ledOff(){
    printf("LED OFF\n");
    #ifndef DEBUG
        cout << "announce.cpp" << endl;
    #endif
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
    ledOn();
    buzzerOn();
}

void announceOff(){
    ledOff();
    buzzerOff();
}