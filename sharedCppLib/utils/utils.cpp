#include <utils.h>
#include <Logger.h>

void crashWithError(String error) {
  Logger::log(error, LogLevel::ERROR);
  Logger::log(F("Exiting program..."), LogLevel::ERROR);
  exit(1);
}

float mapFloat(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
