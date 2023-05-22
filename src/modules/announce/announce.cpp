#include <DBASEV/announce.h>

void initLed(){

}

void initBuzzer(){

}

void ledOn(){

}

void buzzerOn(){

}

void ledOff(){

}

void buzzerOff(){

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