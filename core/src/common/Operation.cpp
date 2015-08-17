/*
 * Operation.cpp
 *
 *  Created on: 16 Feb 2015
 *      Author: kreczko
 */
#include <iostream>
#include "swatch/core/Operation.hpp"
#include "swatch/core/ActionableObject.hpp"

#include "toolbox/fsm/exception/Exception.h"
#include "swatch/core/CommandSequence.hpp"

//using namespace swatch::logger;
namespace swatch {
namespace core {
Operation::Operation( const std::string& aId ) :
        Functionoid( aId ),
        fsm_(new FSM()) {
}

Operation::~Operation() {
  if( fsm_ )
  {
    delete fsm_;
    fsm_ = NULL;
  }
}

const std::vector<std::string>& Operation::getStates() const{
  return fsm_->getStates();
}

const std::map<std::string, std::string> Operation::getStateTransitions(const std::string& s) const {
  return fsm_->getStateTransitions(s);
}

void Operation::executeTransition(const std::string& name){
    fsm_->executeTransition(name);
}

std::string Operation::getCurrentState() const {
  return fsm_->getCurrentState();
}


/*
void Operation::addTransition(const std::string& from, const std::string& to, const std::string& event, CommandSequence& aCommandSequence )
{
  void (CommandSequence::*lExec)() =  &CommandSequence::threadless_exec;
  bool (CommandSequence::*lPrecondition)() = &CommandSequence::precondition;
  fsm_->addTransition( from , to , event , &aCommandSequence , lPrecondition , lExec );
}

void Operation::addTransition(const std::string& from, const std::string& to, const std::string& event, const std::string& aCommandSequenceId )
{
  void (CommandSequence::*lExec)() =  &CommandSequence::threadless_exec;
  bool (CommandSequence::*lPrecondition)() = &CommandSequence::precondition;
  ActionableObject* lParent( getParent<ActionableObject>()  );
  fsm_->addTransition( from , to , event , & lParent->getCommandSequence( aCommandSequenceId ) , lPrecondition , lExec );
}
*/

} /* namespace core */
} /* namespace swatch */
