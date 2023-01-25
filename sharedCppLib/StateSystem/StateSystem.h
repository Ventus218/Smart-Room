#ifndef __STATESYSTEM__
#define __STATESYSTEM__

#include <Logger.h>

template <class State>
class StateSystem {
private:
  State state;

protected:

  StateSystem(State initialState) {
    this->state = initialState;
  }

  virtual void enterState(State s) {
    this->state = s;
  }
  virtual void exitState(State s) { }

  void transitionTo(State s) {
    Logger::log(getSystemName() + " -> Transition:\t" + stateToString(getState()) + " ---> " + stateToString(s), LogLevel::NOTICE);
    exitState(this->state);
    enterState(s);
  }

  void log(String message) {
    log(message, LogLevel::INFO);
  }

  void log(String message, LogLevel logLevel) {
    Logger::log(getSystemName() + " -> " + message, logLevel);
  }

  virtual String getSystemName() = 0;

  virtual String stateToString(State s) = 0;

public:

  State getState() {
    return state;
  }

};

#endif