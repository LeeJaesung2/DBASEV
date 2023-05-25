#include <wiringPi.h>

#define RELAY_PIN 17 // using GPIO pin
int main() {
    if (wiringPiSetup() == -1) {
        return 1;
    }

    pinMode(RELAY_PIN, OUTPUT);

    // 릴레이 켜기
    digitalWrite(RELAY_PIN, HIGH);

    // 5초간 대기
    delay(5000);

    // 릴레이 끄기
    digitalWrite(RELAY_PIN, LOW);

    return 0;
}