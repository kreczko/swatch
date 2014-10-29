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




} // namespace test
} // namespace processor
} // namespace swatch
