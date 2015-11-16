
#include "swatch/core/ActionableStatus.hpp"


namespace swatch {
namespace core {

  
// STATIC MEMBERS INITIALIZATION
const std::string ActionableStatus::kNullStateMachineId = "";
const std::string ActionableStatus::kNullStateId = "";

//------------------------------------------------------------------------------------
bool ActionableStatus::isAlive() const {
  return mAlive;
}

//------------------------------------------------------------------------------------
bool ActionableStatus::isEngaged() const {
  return mStateMachineId != kNullStateMachineId;
}


//------------------------------------------------------------------------------------
bool ActionableStatus::isRunning() const {
  return !mRunningActions.empty();
}

//------------------------------------------------------------------------------------
const std::string& ActionableStatus::getState() const {
  return mState;
}

//------------------------------------------------------------------------------------
const Functionoid* ActionableStatus::getLastRunningAction() const {
  return mRunningActions.back();
}


//------------------------------------------------------------------------------------
const std::vector<const Functionoid*>& ActionableStatus::getRunningActions() const {
  return mRunningActions;
}


//------------------------------------------------------------------------------------
ActionableStatus::ActionableStatus() :
  mAlive(true), 
  mStateMachineId(kNullStateMachineId),
  mState(kNullStateId) {
}


const std::string& ActionableStatus::getStateMachineId() const {
  return mStateMachineId;
}

  
}
}
