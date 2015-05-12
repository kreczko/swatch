/*
 * Operation.cpp
 *
 *  Created on: 16 Feb 2015
 *      Author: kreczko
 */
#include <iostream>
#include "swatch/core/Operation.hpp"

#include "toolbox/fsm/exception/Exception.h"

//using namespace swatch::logger;
namespace swatch {
namespace core {
Operation::Operation(ActionableObject* resource) :
        parameters_(),
        fsm_(new FSM()),
        resource_(resource) {
}

Operation::~Operation() {
  // TODO Auto-generated destructor stub
}

XParameterSet&
Operation::getParams() {
  return parameters_;
}

bool Operation::preCondition() const {
  return false;
}

const std::vector<std::string>& Operation::getStates() const{
  return fsm_->getStates();
}

const std::map<std::string, std::string> Operation::getStateTransitions(const std::string& s) const {
  return fsm_->getStateTransitions(s);
}

void Operation::executeTransition(const std::string name){
    fsm_->executeTransition(name);
}

std::string Operation::getCurrentState() const {
  return fsm_->getCurrentState();
}

} /* namespace core */
} /* namespace swatch */
