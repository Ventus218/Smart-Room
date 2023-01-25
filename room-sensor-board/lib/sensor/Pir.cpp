#include <Pir.h>
#include <Arduino.h>

Pir::Pir(int pin) {
    this->pin = pin;
    pinMode(this->pin, INPUT);
}

bool Pir::isDetectingMotion() {
    return digitalRead(this->pin);
}