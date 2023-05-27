#include <wiringPi.h>

#define RELAY_PIN 0 // using GPIO pin
int main() {
    if (wiringPiSetup() == -1) {
        return 1;
    }

    pinMode(RELAY_PIN, OUTPUT);

    while(1){
        // 릴레이 켜기
        digitalWrite(RELAY_PIN, HIGH);

        // 5초간 대기
        delay(1000);

        // 릴레이 끄기
        digitalWrite(RELAY_PIN, LOW);
    }

    return 0;
}