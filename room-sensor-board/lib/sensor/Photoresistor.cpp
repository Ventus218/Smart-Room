#include <Photoresistor.h>
#include <Arduino.h>

Photoresistor::Photoresistor(int pin) {
    this->pin = pin;
    pinMode(this->pin, INPUT);
}

int Photoresistor::getLightLevel() {
    return analogRead(this->pin);
}