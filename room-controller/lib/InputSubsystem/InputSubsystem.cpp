#include <InputSubsystem.h>

InputSubsystem::InputSubsystem(): StateSystem(InputSubsystemState::INIT) { }

void InputSubsystem::checkForStateTransitions() {
  // TODO
}

void InputSubsystem::receiveStateFromService() {
  // **** MOCKUP ****
  this->lightOn = (rand() % 2);
  this->rollerValue = (rand() % 1024);
  log(String("light: ") + (this->lightOn ? String("ON") : String("OFF")) + String("\troller: ") + this->rollerValue + String(" (from service)"), LogLevel::INFO);
  // **** MOCKUP ****
}

void InputSubsystem::receiveStateFromBluetooth() {
  // **** MOCKUP ****
  this->lightOn = (rand() % 2);
  this->rollerValue = (rand() % 1024);
  log(String("light: ") + (this->lightOn ? String("ON") : String("OFF")) + String("\troller: ") + this->rollerValue + String(" (from bluetooth)"), LogLevel::INFO);
  // **** MOCKUP ****
}

void InputSubsystem::updateServiceState() {
  // TODO
}

void InputSubsystem::init(int period) {
  Task::init(period);
  srand(millis()); // NEEDED ONLY FOR MOCKUP
  this->receiveStateFromService();
  transitionTo(InputSubsystemState::SERVICE);
}

void InputSubsystem::tick() {
  checkForStateTransitions();

  switch (StateSystem::getState()) {
    case InputSubsystemState::SERVICE:
      this->receiveStateFromService();
      break;
    case InputSubsystemState::BLUETOOTH:
      this->receiveStateFromBluetooth();
      this->updateServiceState();
      break;
    case InputSubsystemState::INIT:
      log(F("should not tick in INIT state"), LogLevel::WARNING);
      break;
  }
}

bool InputSubsystem::getLightOn() {
    return this->lightOn;
}

int InputSubsystem::getRollerValue() {
    return this->rollerValue;
}

String InputSubsystem::getSystemName() {
  return F("InputSubsystem");
}

String InputSubsystem::stateToString(InputSubsystemState s) {
  String str;

  switch(s) {
    case InputSubsystemState::INIT: 
      str = F("INIT");
      break;
    case InputSubsystemState::SERVICE: 
      str = F("SERVICE");
      break;
    case InputSubsystemState::BLUETOOTH: 
      str = F("BLUETOOTH");
      break;
  }

  return str;
}
