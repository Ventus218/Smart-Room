#include <RollerSubsystem.h>

RollerSubsystem::RollerSubsystem(Lever* roller, InputSubsystem* inputSys): StateSystem(RollerSubsystemState::INIT) {
  this->roller = roller;
  this->inputSys = inputSys;
}

void RollerSubsystem::init(int period) {
  Task::init(period);
  transitionTo(RollerSubsystemState::ROLLER_CONTROL);
}

void RollerSubsystem::tick() {
  switch (getState()) {
    case RollerSubsystemState::ROLLER_CONTROL:
      setRoller(inputSys->getRollerPercentage());
      break;
    case RollerSubsystemState::INIT:
      log(F("should not tick in INIT state"), LogLevel::WARNING);
      break;
  }
}

void RollerSubsystem::setRoller(int rollerPercentage) {
  int angle = map(rollerPercentage, 0, 100, 0, 180);
  this->roller->setAngle(angle);
}

String RollerSubsystem::getSystemName() {
  return F("RollerSubsystem");
}

String RollerSubsystem::stateToString(RollerSubsystemState s) {
  String str;

  switch(s) {
    case RollerSubsystemState::INIT: 
      str = F("INIT");
      break;
    case RollerSubsystemState::ROLLER_CONTROL: 
      str = F("ROLLER_CONTROL");
      break;
  }

  return str;
}
