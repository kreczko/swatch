/**
 * @file    FSM.hxx
 * @author  kreczko
 * @brief   Finite State Machine (FSM) for swatch.
 * @date    November 2014
 *
 */

#ifndef __SWATCH_CORE_FSM_HXX__
#define	__SWATCH_CORE_FSM_HXX__
#include "swatch/core/exception.hpp"

namespace swatch {
namespace core {

template<class OBJECT> void FSM::addStateTransition(const std::string& from, const std::string& to, const std::string& input, OBJECT * obj,
            void (OBJECT::*func)(toolbox::Event::Reference)) {
      if (!hasState(from))
            throw exception("'from' State " + from + " is undeclared");
        if (!hasState(to))
            throw exception("'to' State " + to + " is undeclared");
        const toolbox::fsm::State xdaq_from = getXDAQState(from);
        const toolbox::fsm::State xdaq_to = getXDAQState(to);
        fsm_.addStateTransition<OBJECT>(xdaq_from, xdaq_to, input, obj, func);
    }

//template <class OBJECT>
//        void FSM::setFailedStateTransitionAction(OBJECT * obj,
//                    void (OBJECT::*func)(toolbox::Event::Reference) ){
//  fsm_.setFailedStateTransitionAction(obj, func);
//}

template<class OBJECT> FSM::StateMethod<OBJECT>::StateMethod(OBJECT* object,
    bool (OBJECT::*condition)(), void (OBJECT::*method)()) :
        obj_(object),
        condition_(condition),
        method_(method) {

}

template <class OBJECT> void FSM::StateMethod<OBJECT>::method() {
  if (method_ != 0) {
    return (obj_->*method_)();
  }
}

template <class OBJECT> bool FSM::StateMethod<OBJECT>::condition() {
  if (condition_ != 0) {
    return (obj_->*condition_)();
  }

  return true;
}

template <class OBJECT>
  void FSM::addTransition(const std::string& from, const std::string& to,
      const std::string& event, OBJECT* object, bool (OBJECT::*condition)(),
      void (OBJECT::*function)()){
  if( ! hasState(from)){
    throw exception("FSM::addTransition: State name: " + from + " does not exist");
  }
  if ( ! hasState(to)) {
    throw exception("FSM::addTransition: State name: " + to + " does not exist");
  }

  toolbox::fsm::State fromState = state_map_[from];
  toolbox::fsm::State toState = state_map_[to];

  // add intermediate state
  toolbox::fsm::State intermediateState = getNextXDAQState();
  std::string intermediateStateName = from + "_" + event + "_" + to;
  fsm_.addState ( intermediateState, intermediateStateName, this, &FSM::moveForward );
  StateMethodSignature* stateMethod = new StateMethod<OBJECT> ( object, condition, function );

  stateMethods_[intermediateStateName] = stateMethod;
  stateTransitionTable_[from][event] = intermediateStateName;

  //add event transition
  fsm_.addStateTransition(fromState, intermediateState, event, this,
      &FSM::doNothing);

  //add move_forward transition
  fsm_.addStateTransition(intermediateState, toState, "move_forward", this,
      &FSM::doNothing);

  //add move_back transition
  fsm_.addStateTransition(intermediateState, fromState, "move_back", this,
      &FSM::doNothing);

  //add rollback transition
  std::string rollback = getRollbackTransition(from, event);
  fsm_.addStateTransition(toState, fromState, rollback, this,
      &FSM::doNothing);
}

} // namespace core
} // namespace swatch


#endif	/* __SWATCH_CORE_FSM_HXX__ */
