#ifndef __ROLLERSUBSYSTEM__
#define __ROLLERUBSYSTEM__

#include <Task.h>
#include <Lever.h>
#include <StateSystem.h>
#include <InputSubsystem.h>

enum class RollerSubsystemState { INIT, ROLLER_CONTROL };

class RollerSubsystem: public Task, public StateSystem<RollerSubsystemState> {

private:
  Lever* roller;
  InputSubsystem* inputSys;

  void setRoller(int rollerPercentage);

public:
  RollerSubsystem(Lever* roller, InputSubsystem* inputSys);  
  void init(int period);  
  void tick();
  String getSystemName();
  String stateToString(RollerSubsystemState s);
};

#endif
