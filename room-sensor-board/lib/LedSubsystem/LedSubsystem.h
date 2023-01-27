#ifndef __LEDSUBSYSTEM__
#define __LEDSUBSYSTEM__

#include <Task.h>
#include <Led.h>
#include <StateSystem.h>
#include <SamplingSubsystem.h>

enum class LedSubsystemState { INIT, ROOM_EMPTY, ROOM_NOT_EMPTY };

class LedSubsystem: public Task, public StateSystem<LedSubsystemState> {

private:
  Led* led;
  SamplingSubsystem* samplingSys;

  void checkForStateTransitions();
  void enterState(LedSubsystemState state);

public:
  LedSubsystem(Led* led, SamplingSubsystem* samplingSys);  
  void init(int period);  
  void tick();
  String getSystemName();
  String stateToString(LedSubsystemState s);
};

#endif
