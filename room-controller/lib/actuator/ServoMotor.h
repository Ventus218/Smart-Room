#ifndef __SERVOMOTOR__
#define __SERVOMOTOR__

#include <Lever.h>
#include <Arduino.h>
#include <ServoTimer2.h> // This needs #include <Arduino.h>

class ServoMotor: public Lever {
private:
    ServoTimer2 servo;

public:
    ServoMotor(int pin);
    void setAngle(int angle);
};

#endif