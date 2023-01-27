#include <SamplingSubsystem.h>

SamplingSubsystem::SamplingSubsystem(MotionDetector* motionDetector, LightLevelDetector* lightLevelDetector): StateSystem(SamplingSubsystemState::INIT) {
  this->motionDetector = motionDetector;
  this->lightLevelDetector = lightLevelDetector;
}

void SamplingSubsystem::sample() {
  this->lightLevel = this->lightLevelDetector->getLightLevel();
  this->motion = this->motionDetector->isDetectingMotion();
  log("Motion: " + String(this->motion) + "\tLightLevel: " + String(this->lightLevel), LogLevel::INFO);
}

void SamplingSubsystem::init(int period) {
  Task::init(period);
  sample();
  transitionTo(SamplingSubsystemState::SAMPLING);
}

void SamplingSubsystem::tick() {
  switch (getState()) {
    case SamplingSubsystemState::SAMPLING:
      sample();
      break;
    case SamplingSubsystemState::INIT:
      log(F("should not tick in INIT state"), LogLevel::WARNING);
      break;
  }
}

int SamplingSubsystem::getLightLevel() {
  return this->lightLevel;
}

bool SamplingSubsystem::getMotion() {
  return this->motion;
}

String SamplingSubsystem::getSystemName() {
  return F("SamplingSubsystem");
}

String SamplingSubsystem::stateToString(SamplingSubsystemState s) {
  String str;

  switch(s) {
    case SamplingSubsystemState::INIT: 
      str = F("INIT");
      break;
    case SamplingSubsystemState::SAMPLING: 
      str = F("SAMPLING");
      break;
  }
  return str;
}