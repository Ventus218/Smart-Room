#ifndef __LIGHTSUBSYSTEM__
#define __LIGHTSUBSYSTEM__

#include <Task.h>
#include <Light.h>
#include <StateSystem.h>
#include <InputSubsystem.h>

enum class LightSubsystemState { INIT, OFF, ON };

class LightSubsystem: public Task, public StateSystem<LightSubsystemState> {

private:
  Light* light;
  InputSubsystem* inputSys;

  void checkForStateTransitions();
  void enterState(LightSubsystemState state);

public:
  LightSubsystem(Light* light, InputSubsystem* inputSys);  
  void init(int period);  
  void tick();
  String getSystemName();
  String stateToString(LightSubsystemState s);
};

#endif
