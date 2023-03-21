#ifndef __INPUTSUBSYSTEM__
#define __INPUTSUBSYSTEM__

#include <Task.h>
#include <Led.h>
#include <StateSystem.h>

enum class InputSubsystemState { INIT, SERVICE, BLUETOOTH };

class InputSubsystem: public Task, public StateSystem<InputSubsystemState> {

private:
  bool lightOn;
  int rollerPercentage;

  void checkForStateTransitions();

  void receiveStateFromService();
  void receiveStateFromBluetooth();
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
