/**
 * @file    IPBusTxChannel.cpp
 * @author  Alessandro Thea
 * @brief   Brief description
 * @date    
 */

#ifndef SWATCH_PROCESSOR_TEST_SILLYFSM_HPP
#define SWATCH_PROCESSOR_TEST_SILLYFSM_HPP

#include <string>
#include <map>
#include <set>

namespace swatch {
namespace processor {
namespace test {

typedef std::string State;
typedef std::string Input;

class SillyFSM {
public:
    SillyFSM();
    ~SillyFSM();

    void setInitialState( State s) { initialState_ = s; }
    State getInitialState() { return initialState_; }
    State getCurrentState() { return currentState_; }
    void reset();
    
    void addState( State s );
private:

    //
    State initialState_;

    //! Current state machine state
    State currentState_;

    std::set<State> states_;
    
    std::map< State, std::map<std::string, State> > stateTransitionTable_;


};

} // namespace test
} // namespace processor
} // namespace swatch

#endif /* SWATCH_PROCESSOR_TEST_SILLYFSM_HPP */