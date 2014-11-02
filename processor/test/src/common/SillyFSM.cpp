#include <stdexcept>

#include "swatch/processor/test/SillyFSM.hpp"

namespace swatch {
namespace processor {
namespace test {

SillyFSM::SillyFSM() :
initialState_(""), currentState_("") {
    this->addState("Failed");
}

SillyFSM::~SillyFSM() {

}

void
SillyFSM::reset() {
    if (initialState_ != "") {
        currentState_ = initialState_;
    } else {
        throw std::runtime_error("Initial state not declared");
    }
}

void SillyFSM::addState(State s) {
    if ( states_.find(s) != states_.end() ) {
        throw std::runtime_error("State "+s+" already declared");
    }    
    states_.insert(s);
}

void SillyFSM::addStateTransition(const State& from, const State& to, const Input &input) {
    if ( states_.find(from) == states_.end()) {
        throw std::runtime_error("State "+from+" not found");
    }

    if ( states_.find(to) == states_.end()) {
        throw std::runtime_error("State "+to+" not found");
    }

    if ( inputs_.find(input) != inputs_.end() ) {
        throw std::runtime_error("Input "+input+" already defined");
    }

    stateTransitionTable_[from][input] = to;

}

void SillyFSM::execute(const Input& input) {
    if ( currentState_ == "" ) {
        throw std::runtime_error("State machine not initialised");
    }

    State newState = stateTransitionTable_[currentState_][input];
    if ( newState == "") {
        std::string msg = "Invalid input event: ";
        msg += input;
        msg += ", current state: ";
        msg += currentState_;
        throw std::runtime_error(msg);
    }
    
    // Execute transition-related action here
    
    currentState_ = newState;

    // State changed action here.
}



} // namespace test
} // namespace processor
} // namespace swatch
