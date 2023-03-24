#ifndef __INPUTSUBSYSTEM__
#define __INPUTSUBSYSTEM__

#include <Task.h>
#include <Led.h>
#include <StateSystem.h>

enum class InputSubsystemState { INIT, SERVICE, BLUETOOTH };
enum class InputChangeFlag { NO_CHANGE = 0, BT_CONN = 1, BT_DISC = 2 };
enum class InputSource { SERVICE = 0, BLUETOOTH = 1};

class InputSubsystemInput {

  public:
    InputChangeFlag changeFlag;
    InputSource source;
    bool lightOn;
    int rollerPercentage;

    InputSubsystemInput(InputChangeFlag changeFlag, InputSource source, bool lightOn, int rollerPercentage) {
      this->changeFlag = changeFlag;
      this->source = source;
      this->lightOn = lightOn;
      this->rollerPercentage = rollerPercentage;
    }
};

class InputSubsystem: public Task, public StateSystem<InputSubsystemState> {

private:
  bool lightOn;
  int rollerPercentage;
  int inputCount;
  InputSubsystemInput* input[10];  // this can be adjusted, a dynamic allocation would be better but considering current time slots it's okay...

  void checkForStateTransitions();
  void enterState(InputSubsystemState s);
  void sample();

  void consumeInput();
  void updateServiceState();

public:
  InputSubsystem();  
  void init(int period);  
  void tick();

  bool getLightOn();
  int getRollerPercentage();

  String getSystemName();
  String stateToString(InputSubsystemState s);
};

#endif
