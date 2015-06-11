/*
 * FSM.cpp
 *
 *  Created on: 18 Nov 2014
 *      Author: kreczko
 */

#include "swatch/core/FSM.hpp"
#include "toolbox/fsm/exception/Exception.h"

namespace swatch {
namespace core {
FSM::FSM() :
        initialState_("\0"),
        currentState_("\0"),
        states_(),
        state_map_(),
        stateTransitionTable_(),
        fsm_(),
        xdaq_state_index_(0),
        highestState_(0),
        stateMethods_() {
}

FSM::~FSM() {
}

void FSM::setInitialState(const std::string s) {
  if (hasState(s)) {
    toolbox::fsm::State state = state_map_[s];
    fsm_.setInitialState(state);
    initialState_ = s;
  } else
    throw exception("FSM::setInitialState: State " + s + " is undeclared");
}

const std::string& FSM::getInitialState() const {
  // wish I could use fsm_.getInitialState(), but for some reason that is not const
  return initialState_;
}

void FSM::addState(const std::string s) {
  if (hasState(s))
    throw exception("State " + s + " already declared");

  try {
    toolbox::fsm::State xdaq_state = getNextXDAQState();
    fsm_.addState(xdaq_state, s);
    state_map_[s] = xdaq_state;
    states_.push_back(s);
  } catch (toolbox::fsm::exception::Exception& e) {
    std::string msg = "swatch::core::FSM::setInitialState(): The state " + s;
    msg += " could not be added to FSM object.";
    msg += "Received exception: " + std::string(e.what());
    throw exception(msg);
  }
}

bool FSM::hasState(const std::string& s) const {
  return state_map_.find(s) != state_map_.end();
}

const std::vector<std::string>& FSM::getStates() const {
  return states_;
}

const toolbox::fsm::State FSM::getNextXDAQState() {
  if ((unsigned) highestState_ >= 0xffffffff)
    throw exception(
        "FSM::getNextXDAQState: It is not possible to create more than 255 states");

  // skip state 'F' as it is the failure state
  if (highestState_ == ('F' - 1))
    highestState_ += 2;
  else
    highestState_ += 1;

  return highestState_;
}

const toolbox::fsm::State FSM::getXDAQState(const std::string& s) const {
  if (!hasState(s))
    throw exception("State " + s + " is undeclared");

  return state_map_.find(s)->second;
}

void FSM::addStateTransition(const std::string& from, const std::string& to,
    const std::string& input) {
  if (!hasState(from))
    throw exception("'from' State " + from + " is undeclared");
  if (!hasState(to))
    throw exception("'to' State " + to + " is undeclared");
  const toolbox::fsm::State xdaq_from = getXDAQState(from);
  const toolbox::fsm::State xdaq_to = getXDAQState(to);
  fsm_.addStateTransition(xdaq_from, xdaq_to, input);
  stateTransitionTable_[from][input] = to;
}

const std::map<std::string, std::string> FSM::getStateTransitions( const std::string& s) const {
  if (!hasState(s))
    throw exception("State " + s + " is undeclared");
  std::map<std::string, std::string> transitions;
  // check if any transition for from this state
  TransitionTable::const_iterator transition_map_it = stateTransitionTable_.find(s);
  if (transition_map_it != stateTransitionTable_.end())
    transitions = transition_map_it->second;

  return transitions;
}

void FSM::fireEvent(toolbox::Event::Reference event) {
  try {
    fsm_.fireEvent(event);
    const toolbox::fsm::State xdaq_new = fsm_.getCurrentState();
    if (xdaq_new == 'F')
      currentState_ = "failure";
    else
      currentState_ = getState(xdaq_new);
  } catch (toolbox::fsm::exception::Exception& e) {
    throw exception( "An error occured during an event:" + std::string(e.what()));
  } catch (...) {
    throw exception("An unknown error occured during an event");
  }
}

const std::string& FSM::getCurrentState() const {
  return currentState_;
}

void FSM::reset() {
  if (initialState_ == "\0")
    throw exception("initial state is not set");
  else {
    fsm_.reset();
    currentState_ = initialState_;
  }
}

const std::string FSM::getState(const toolbox::fsm::State& s) const {
  std::string result = "\0";
  for (StateMap::const_iterator it = state_map_.begin(); it != state_map_.end(); ++it) {
    if (it->second == s) {
      result = it->first;
      break;
    }
  }
  if (result == "\0")
    throw exception(
        "XDAQState " + std::string((const char*) s) + " is undeclared");

  return result;
}

void FSM::doNothing(toolbox::Event::Reference e) {
  // do nothing
}

void FSM::moveForward(toolbox::fsm::FiniteStateMachine& fsm) {
  try {
    std::string state = fsm_.getStateName(fsm_.getCurrentState());
    if (stateMethods_[state]->condition()) {
      toolbox::Event::Reference e(new toolbox::Event("move_forward", NULL));
      stateMethods_[state]->method();
      fsm.fireEvent(e);
      currentState_ = fsm_.getStateName(fsm_.getCurrentState());
    } else {
      toolbox::Event::Reference e(new toolbox::Event("move_back", NULL));
      fsm.fireEvent(e);
    }

  } catch (toolbox::fsm::exception::Exception& except) {
    toolbox::Event::Reference e(new toolbox::Event("move_back", NULL));
    fsm.fireEvent(e);
    throw toolbox::fsm::exception::Exception("FSM::moveForward", except.what(), __FILE__, __LINE__, __FUNCTION__);
  } catch (xcept::Exception& except) {
    toolbox::Event::Reference e(new toolbox::Event("move_back", NULL));
    fsm.fireEvent(e);
    throw toolbox::fsm::exception::Exception("FSM::moveForward", except.what(), __FILE__, __LINE__, __FUNCTION__);
  } catch (exception& except) {
    toolbox::Event::Reference e(new toolbox::Event("move_back", NULL));
    fsm.fireEvent(e);
    throw toolbox::fsm::exception::Exception("FSM::moveForward", except.what(), __FILE__, __LINE__, __FUNCTION__);
  } catch (...) {
    toolbox::Event::Reference e ( new toolbox::Event ( "move_back", NULL ) );
    fsm.fireEvent ( e );
    throw toolbox::fsm::exception::Exception("FSM::moveForward", "Unkown exception", __FILE__, __LINE__, __FUNCTION__);
  }
}

std::string FSM::getRollbackTransition(const std::string& from, const std::string& transition) {
  std::string rollback;
  rollback = "move_back_";
  rollback += from + "_";
  rollback += transition;
  return rollback;
}

void FSM::executeTransition(const std::string& transition) {
  toolbox::Event::Reference e(new toolbox::Event(transition, NULL));
  fireEvent(e);
}

void FSM::addTransition(const std::string& from, const std::string& to, const std::string& event) {

  if (!hasState(from)) {
    throw exception("FSM::addTransition: State name: " + from + " does not exist");
  }
  if (!hasState(to)) {
    throw exception("FSM::addTransition: State name: " + to + " does not exist");
  }

  toolbox::fsm::State fromState = state_map_[from];
  toolbox::fsm::State toState = state_map_[to];

  // add intermediate state
  toolbox::fsm::State intermediateState = getNextXDAQState();
  std::string intermediateStateName = from + "_" + event + "_" + to;
  fsm_.addState(intermediateState, intermediateStateName, this, &FSM::moveForward);
  StateMethodSignature* stateMethod = new StateMethod<FSM>(this, NULL,NULL);

  stateMethods_[intermediateStateName] = stateMethod;
  stateTransitionTable_[from][event] = intermediateStateName;

  //add event transition
  fsm_.addStateTransition(fromState, intermediateState, event, this, &FSM::doNothing);

  //add move_forward transition
  fsm_.addStateTransition(intermediateState, toState, "move_forward", this, &FSM::doNothing);

  //add move_back transition
  fsm_.addStateTransition(intermediateState, fromState, "move_back", this,  &FSM::doNothing);

  //add rollback transition
  std::string rollback = getRollbackTransition(from, event);
  fsm_.addStateTransition(toState, fromState, rollback, this, &FSM::doNothing);
}

void FSM::clear() {
  reset();
  currentState_ = "\0";
  initialState_ = "\0";
  states_.clear();
  state_map_.clear();
  stateTransitionTable_.clear();
  xdaq_state_index_ = 0;
  highestState_ = 0;
  stateMethods_.clear();
  fsm_ = toolbox::fsm::FiniteStateMachine();
}

} /* namespace core */
} /* namespace swatch */
