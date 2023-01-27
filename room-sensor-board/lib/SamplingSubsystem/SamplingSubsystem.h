#ifndef __SAMPLINGSUBSYSTEM__
#define __SAMPLINGSUBSYSTEM__

#include <Task.h>
#include <StateSystem.h>
#include <MotionDetector.h>
#include <LightLevelDetector.h>

enum class SamplingSubsystemState { INIT, SAMPLING };

class SamplingSubsystem: public Task, public StateSystem<SamplingSubsystemState> {
private:
  MotionDetector* motionDetector;
  LightLevelDetector* lightLevelDetector;
  int lightLevel;
  bool motion;

  void sample();

public:
  SamplingSubsystem(MotionDetector* motionDetector, LightLevelDetector* lightLevelDetector);
  void init(int period);  
  void tick();

  int getLightLevel();
  bool getMotion();

  String getSystemName();
  String stateToString(SamplingSubsystemState s);
};

#endif