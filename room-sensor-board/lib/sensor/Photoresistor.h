#ifndef __PHOTORESISTOR__
#define __PHOTORESISTOR__

#include <LightLevelDetector.h>

class Photoresistor: public LightLevelDetector {
private:
    int pin;
    
public:
    Photoresistor(int pin);
    int getLightLevel();
};

#endif