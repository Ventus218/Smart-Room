#include <ServoMotor.h>
#include <Arduino.h>
#include <ServoTimer2.h>
#include <Logger.h>

ServoMotor::ServoMotor(int pin) {
    this->servo.attach(pin);
}

void ServoMotor::setAngle(int angle) {
    if (angle < 0) {
        angle = 0;
        Logger::log("Tried to set " + String(angle) + "° angle but servo motors minimum angle is 0°. Using 0° instead.", LogLevel::WARNING);
    } else if (angle > 180) {
        Logger::log("Tried to set " + String(angle) + "° angle but servo motors maximum angle is 180°. Using 180° instead.", LogLevel::WARNING);
        angle = 180;
    }

    this->servo.write(map(angle, 0, 180, 500, 2400)); // https://datasheetspdf.com/pdf/791970/TowerPro/SG90/1
}