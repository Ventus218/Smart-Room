#ifndef __PIR__
#define __PIR__

#include <MotionDetector.h>

class Pir: public MotionDetector {
private:
    int pin;

public:
    Pir(int pin);
    bool isDetectingMotion();
};

#endif