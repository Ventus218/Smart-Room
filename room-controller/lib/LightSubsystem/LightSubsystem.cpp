#include <LightSubsystem.h>

LightSubsystem::LightSubsystem(Light* light, InputSubsystem* inputSys): StateSystem(LightSubsystemState::INIT) {
  this->light = light;
  this->inputSys = inputSys;
}

void LightSubsystem::checkForStateTransitions() {
  bool lightOn = this->inputSys->getLightOn();
  LightSubsystemState curState = this->getState();
  
  if (lightOn && curState != LightSubsystemState::ON) {
    transitionTo(LightSubsystemState::ON);
  } else if (!lightOn && curState != LightSubsystemState::OFF) {
    transitionTo(LightSubsystemState::OFF);
  }
}

void LightSubsystem::enterState(LightSubsystemState state) {
  StateSystem::enterState(state);
  switch (StateSystem::getState()) {
    case LightSubsystemState::OFF:
      this->light->switchOff();
      break;
    case LightSubsystemState::ON:
      this->light->switchOn();
      break;
    case LightSubsystemState::INIT:
      log(F("should not enter in INIT state"), LogLevel::WARNING);
      break;
  }
}

void LightSubsystem::init(int period) {
  Task::init(period);
  checkForStateTransitions();
}

void LightSubsystem::tick() {
  checkForStateTransitions();
}

String LightSubsystem::getSystemName() {
  return F("LightSubsystem");
}

String LightSubsystem::stateToString(LightSubsystemState s) {
  String str;

  switch(s) {
    case LightSubsystemState::INIT: 
      str = F("INIT");
      break;
    case LightSubsystemState::OFF: 
      str = F("OFF");
      break;
    case LightSubsystemState::ON: 
      str = F("ON");
      break;
  }

  return str;
}
