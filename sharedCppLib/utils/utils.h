#ifndef __UTILS__
#define __UTILS__

#include <Arduino.h>

void crashWithError(String error);
float mapFloat(float x, float in_min, float in_max, float out_min, float out_max);

#endif