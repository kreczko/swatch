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
    const State& getInitialState() const { return initialState_; }
    const State& getCurrentState() const { return currentState_; }
    void reset();
    
    void addState( State s );
    const std::set<State>& getStates() const { return states_; }

    const std::set<Input>& getInputs() const { return inputs_; }

    void addStateTransition (const State& from, const State& to, const Input &input);

    void execute( const Input& input ); 

private:

    //
    State initialState_;

    //! Current state machine state
    State currentState_;

    std::set<State> states_;
    
    std::set<State> inputs_;

    std::map< State, std::map<Input, State> > stateTransitionTable_;


};

} // namespace test
} // namespace processor
} // namespace swatch

#include "swatch/processor/test/SillyFSM.hxx"

#endif /* SWATCH_PROCESSOR_TEST_SILLYFSM_HPP */