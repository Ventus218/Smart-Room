/* I HAD TO PUT ALL THE IMPLEMENTATION INSIDE StateSystem.h AS I WAS GETTING LINKER ISSUES WHICH I WASN'T ABLE TO SOLVE*/

// #include <StateSystem.h>
// #include <Logger.h>

// template <class State>
// StateSystem<State>::StateSystem(State initialState) {
//     this->state = initialState;
// }

// template <class State>
// void StateSystem<State>::enterState(State s) {
//     this->state = s;
// }

// template <class State>
// void StateSystem<State>::exitState(State s) { }

// template <class State>
// void StateSystem<State>::transitionTo(State s) {
//     Logger::log(getSystemName() + " -> Transition:\t" + stateToString(getState()) + " ---> " + stateToString(s), LogLevel::INFO);
//     exitState(this->state);
//     enterState(s);
// }

// template <class State>
// State StateSystem<State>::getState() {
//     return state;
// }
