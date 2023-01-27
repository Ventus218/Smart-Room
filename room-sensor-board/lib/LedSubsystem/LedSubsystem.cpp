#include <LedSubsystem.h>

LedSubsystem::LedSubsystem(Led* led, SamplingSubsystem* samplingSys): StateSystem(LedSubsystemState::INIT) {
  this->led = led;
  this->samplingSys = samplingSys;
}

void LedSubsystem::checkForStateTransitions() {
  bool motion = this->samplingSys->getMotion();
  LedSubsystemState curState = this->getState();
  
  if (motion && curState != LedSubsystemState::ROOM_NOT_EMPTY) {
    transitionTo(LedSubsystemState::ROOM_NOT_EMPTY);
  } else if (!motion && curState != LedSubsystemState::ROOM_EMPTY) {
    transitionTo(LedSubsystemState::ROOM_EMPTY);
  }
}

void LedSubsystem::enterState(LedSubsystemState state) {
  StateSystem::enterState(state);
  switch (StateSystem::getState()) {
    case LedSubsystemState::ROOM_EMPTY:
      this->led->switchOff();
      break;
    case LedSubsystemState::ROOM_NOT_EMPTY:
      this->led->switchOn();
      break;
    case LedSubsystemState::INIT:
      log(F("should not enter in INIT state"), LogLevel::WARNING);
      break;
  }
}

void LedSubsystem::init(int period) {
  Task::init(period);
  checkForStateTransitions();
}

void LedSubsystem::tick() {
  checkForStateTransitions();
}

String LedSubsystem::getSystemName() {
  return F("LedSubsystem");
}

String LedSubsystem::stateToString(LedSubsystemState s) {
  String str;

  switch(s) {
    case LedSubsystemState::INIT: 
      str = F("INIT");
      break;
    case LedSubsystemState::ROOM_EMPTY: 
      str = F("ROOM_EMPTY");
      break;
    case LedSubsystemState::ROOM_NOT_EMPTY: 
      str = F("ROOM_NOT_EMPTY");
      break;
  }

  return str;
}
