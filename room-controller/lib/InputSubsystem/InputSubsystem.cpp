#include <InputSubsystem.h>
#include <MsgService.h>
#include <utils.h>

InputSubsystem::InputSubsystem(): StateSystem(InputSubsystemState::INIT) { }

void InputSubsystem::checkForStateTransitions() {
  if (inputCount == 0) { return; }

  InputSubsystemState nextState = getState();

  for (int i = 0; i < inputCount; i++)
  {
    InputSubsystemInput* inp = input[i];
    switch (nextState) {
      case InputSubsystemState::SERVICE:
        if (inp->changeFlag == InputChangeFlag::BT_CONN) {
          nextState = InputSubsystemState::BLUETOOTH;
        }
        break;
      case InputSubsystemState::BLUETOOTH:
        if (inp->changeFlag == InputChangeFlag::BT_DISC) {
          nextState = InputSubsystemState::SERVICE;
        }
        break;
      case InputSubsystemState::INIT:
        log(F("transition from INIT state should be treated in the init method"), LogLevel::WARNING);
        break;
    }
  }
  if (nextState != getState()) {
    transitionTo(nextState);
  }
}

void InputSubsystem::sample() {
  if (MsgService.isMsgAvailable()) {
    Msg* msg = MsgService.receiveMsg();
    String myContent = msg->getContent();
    String firstMessage;
    int lastSeparatorIndex;

    int endOfFirstMessage = myContent.indexOf(':');
    while (endOfFirstMessage != -1) {
      firstMessage = myContent.substring(0, endOfFirstMessage);
      myContent = myContent.substring(endOfFirstMessage + 1, myContent.length());
      lastSeparatorIndex = firstMessage.lastIndexOf(";");

      // Message encoding: "inputChangeFlag;inputSource;lightOn;rollerPercentage" as "int;int;int;int"
      InputChangeFlag flag = (InputChangeFlag)firstMessage.substring(lastSeparatorIndex - 5, lastSeparatorIndex - 4).toInt();
      InputSource source = (InputSource)firstMessage.substring(lastSeparatorIndex - 3, lastSeparatorIndex - 2).toInt();
      String lightOnStr = firstMessage.substring(lastSeparatorIndex - 1, lastSeparatorIndex);
      String rollerPercentageStr = firstMessage.substring(lastSeparatorIndex + 1, firstMessage.length());

      input[inputCount++] = new InputSubsystemInput(flag, source, lightOnStr.toInt(), rollerPercentageStr.toInt());

      endOfFirstMessage = myContent.indexOf(':');
    }
    delete msg;
  }
}

void InputSubsystem::enterState(InputSubsystemState s) {
  StateSystem::enterState(s);

  switch (getState()) {
    case InputSubsystemState::SERVICE:
      MsgService.sendMsg("SERVICE");
      break;
    case InputSubsystemState::BLUETOOTH:
      break;
    case InputSubsystemState::INIT:
      log(F("should never enter INIT state"), LogLevel::WARNING);
      break;
  }
}

void InputSubsystem::consumeInput() {
  if (inputCount <= 0) { return; }

  for (int i = 0; i < inputCount; i++) {
    InputSubsystemInput* in = input[i];

    if ((getState() == InputSubsystemState::BLUETOOTH && in->source == InputSource::SERVICE) || (getState() == InputSubsystemState::SERVICE && in->source == InputSource::BLUETOOTH)) {
      delete in;
      break; // ignoring service input while in bt connection or viceversa
    }

    this->lightOn = in->lightOn;
    this->rollerPercentage = in->rollerPercentage;

    String source = in->source == InputSource::BLUETOOTH ? "BLUETOOTH" : "SERVICE";
    String light = this->lightOn ? String("ON") : String("OFF");

    log("Input from: " + source + String("\tlight: ") + light + String("\troller: ") + this->rollerPercentage, LogLevel::INFO);

    delete in;
  }

  inputCount = 0;
}

void InputSubsystem::updateServiceState() {
  MsgService.sendMsg(String("BLUETOOTH;" + (lightOn ? String("true") : String("false")) + ";" + String(rollerPercentage)));
}

void InputSubsystem::init(int period) {
  Task::init(period);
  MsgService.init();
  this->lightOn = false;
  this->rollerPercentage = 0;
  this->inputCount = 0;
  this->sample();
  transitionTo(InputSubsystemState::SERVICE);
}

void InputSubsystem::tick() {
  sample();
  checkForStateTransitions();

  switch (StateSystem::getState()) {
    case InputSubsystemState::SERVICE:
      this->consumeInput();
      break;
    case InputSubsystemState::BLUETOOTH:
      this->consumeInput();
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
