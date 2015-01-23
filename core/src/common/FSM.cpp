/*
 * FSM.cpp
 *
 *  Created on: 18 Nov 2014
 *      Author: kreczko
 */

#include "swatch/core/FSM.hpp"

namespace swatch {
namespace core {
/// 'F' is not included as it is reserved for the 'failure' state in XDAQ
const std::string FSM::XDAQ_STATES = "abcdefghijklmnopqrstuvwxyzABCDEGHIJKLMNOPQRSTUVWXYZ";

FSM::FSM() :
				initialState_("\0"),
				currentState_("\0"),
				states_(),
				state_map_(),
				stateTransitionTable_(),
				fsm_(),
				xdaq_state_index_(0) {
}

FSM::~FSM() {
}

void FSM::setInitialState(State s) {
	if (hasState(s)) {
		fsm_.setInitialState(getXDAQState(s));
		initialState_ = s;
	} else
		throw exception("State " + s + " is undeclared");
}

const State& FSM::getInitialState() const {
	// wish I could use fsm_.getInitialState(), but for some reason that is not const
	return initialState_;
}

void FSM::addState(State s) {
	if (hasState(s))
		throw exception("State " + s + " already declared");
	else {
		toolbox::fsm::State xdaq_state = getNextXDAQState();
		fsm_.addState(xdaq_state, s);
		state_map_[s] = xdaq_state;
		states_.push_back(s);
	}
}

bool FSM::hasState(State s) const {
	return state_map_.find(s) != state_map_.end();
}

const std::vector<State>& FSM::getStates() const {
	return states_;
}

const toolbox::fsm::State FSM::getNextXDAQState() {
	if (xdaq_state_index_ >= FSM::XDAQ_STATES.size())
		throw exception("Too many states booked!");
	toolbox::fsm::State state(FSM::XDAQ_STATES[xdaq_state_index_]);
	++xdaq_state_index_;
	return state;
}

const XDAQState FSM::getXDAQState(const State& s) const {
	if (!hasState(s))
		throw exception("State " + s + " is undeclared");

	return state_map_.find(s)->second;
}

void FSM::addStateTransition(const State& from, const State&to, const Input& input) {
	if (!hasState(from))
		throw exception("'from' State " + from + " is undeclared");
	if (!hasState(to))
		throw exception("'to' State " + to + " is undeclared");
	const XDAQState xdaq_from = getXDAQState(from);
	const XDAQState xdaq_to = getXDAQState(to);
	fsm_.addStateTransition(xdaq_from, xdaq_to, input);
	stateTransitionTable_[from][input] = to;
}

const std::map<Input, State> FSM::getStateTransitions(const State& s) const {
	if (!hasState(s))
		throw exception("State " + s + " is undeclared");
	std::map<Input, State> transitions;
	// check if any transition for from this state
	auto transition_map_it = stateTransitionTable_.find(s);
	if (transition_map_it != stateTransitionTable_.end())
		transitions = transition_map_it->second;

	return transitions;
}

void FSM::fireEvent(toolbox::Event::Reference event) {
  try{
    fsm_.fireEvent(event);
    const XDAQState xdaq_new = fsm_.getCurrentState();
    if(xdaq_new == 'F')
      currentState_ = "failure";
    else
      currentState_ = getState(xdaq_new);
  }catch (toolbox::fsm::exception::Exception& e){
    throw exception("An error occured during an event:" + std::string(e.what()));
  }
}

State FSM::getCurrentState() const {
  return currentState_;
}

void FSM::reset() {
  if(initialState_ == "\0")
    throw exception("initial state is not set");
  else {
    fsm_.reset();
    currentState_ = initialState_;
  }
}

const State FSM::getState(const XDAQState& s) const {
  State result = "\0";
  for(auto it = state_map_.begin(); it != state_map_.end(); ++it){
    if(it->second == s){
      result = it->first;
      break;
    }
  }
  if(result == "\0")
    throw exception("XDAQState " + std::string((const char*)s) + " is undeclared");

  return result;
}

} /* namespace core */
} /* namespace swatch */
