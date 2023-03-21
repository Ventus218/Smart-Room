#include <InputSubsystem.h>
#include <MsgService.h>
#include <utils.h>

InputSubsystem::InputSubsystem(): StateSystem(InputSubsystemState::INIT) { }

void InputSubsystem::checkForStateTransitions() {
  // TODO
}

void InputSubsystem::receiveStateFromService() {
  if (MsgService.isMsgAvailable()) {
    Msg* msg = MsgService.receiveMsg();
    String myContent = msg->getContent();

    // Getting only the last message received.
    int lastSeparatorIndex = myContent.lastIndexOf(";");
    
    // Message encoding: "lightOn;RollerPercentage" as "int;int"
    String lightOnStr = myContent.substring(lastSeparatorIndex - 1, lastSeparatorIndex);
    String rollerPercentageStr = myContent.substring(lastSeparatorIndex + 1, myContent.length());

    this->lightOn = String(lightOnStr).toInt();
    this->rollerPercentage = String(rollerPercentageStr).toInt();
    log(String("light: ") + (this->lightOn ? String("ON") : String("OFF")) + String("\troller: ") + this->rollerPercentage + String(" (from service)"), LogLevel::INFO);  
    
    delete msg;
  }
}

void InputSubsystem::receiveStateFromBluetooth() {
  // **** MOCKUP ****
  this->lightOn = (rand() % 2);
  this->rollerPercentage = (rand() % 101);
  log(String("light: ") + (this->lightOn ? String("ON") : String("OFF")) + String("\troller: ") + this->rollerPercentage + String(" (from bluetooth)"), LogLevel::INFO);
  // **** MOCKUP ****
}

void InputSubsystem::updateServiceState() {
  MsgService.sendMsg(String("BLUETOOTH;" + (lightOn ? String("true") : String("false")) + ";" + String(rollerPercentage)));
}

void InputSubsystem::init(int period) {
  Task::init(period);
  MsgService.init();
  srand(millis()); // NEEDED ONLY FOR MOCKUP
  this->lightOn = false;
  this->rollerPercentage = 0;
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

int InputSubsystem::getRollerPercentage() {
    return this->rollerPercentage;
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
