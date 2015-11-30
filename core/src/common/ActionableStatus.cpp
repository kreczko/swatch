
#include "swatch/core/ActionableStatus.hpp"


namespace swatch {
namespace core {

  
// STATIC MEMBERS INITIALIZATION
const std::string ActionableStatus::kNullStateMachineId = "";
const std::string ActionableStatus::kNullStateId = "";


//------------------------------------------------------------------------------------
ActionableStatus::ActionableStatus() :
  mAlive(true), 
  mStateMachineId(kNullStateMachineId),
  mState(kNullStateId) {
}


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
const std::string& ActionableStatus::getStateMachineId() const {
  return mStateMachineId;
}


//------------------------------------------------------------------------------------
ActionableStatusGuard::ActionableStatusGuard(const MutableActionableStatus& aStatus) : 
  mStatus(aStatus),
  mLockGuard(aStatus.mMutex)
{
}
  
//------------------------------------------------------------------------------------
ActionableStatusGuard::ActionableStatusGuard(const MutableActionableStatus& aStatus, boost::adopt_lock_t) :
  mStatus(aStatus),
  mLockGuard(aStatus.mMutex, boost::adopt_lock_t())
{
}

//------------------------------------------------------------------------------------
ActionableStatusGuard::~ActionableStatusGuard()
{
}
  
//------------------------------------------------------------------------------------
bool ActionableStatusGuard::isCorrectGuard(const MutableActionableStatus& aStatus) const
{
  return (&mStatus == &aStatus);
}



//------------------------------------------------------------------------------------
MutableActionableStatus::MutableActionableStatus() : 
  mStatus()
{
}

//------------------------------------------------------------------------------------
MutableActionableStatus::~MutableActionableStatus()
{
}

//------------------------------------------------------------------------------------
ActionableStatus MutableActionableStatus::getSnapshot(const ActionableStatusGuard& aGuard) const
{
  throwIfWrongGuard(aGuard);
  return mStatus;
}

//------------------------------------------------------------------------------------
bool MutableActionableStatus::isAlive(const ActionableStatusGuard& aGuard) const
{
  throwIfWrongGuard(aGuard);
  return mStatus.isAlive();
}

//------------------------------------------------------------------------------------
bool MutableActionableStatus::isEngaged(const ActionableStatusGuard& aGuard) const
{
  throwIfWrongGuard(aGuard);
  return mStatus.isEngaged();
}

//------------------------------------------------------------------------------------
const std::string& MutableActionableStatus::getStateMachineId(const ActionableStatusGuard& aGuard) const
{
  throwIfWrongGuard(aGuard);
  return mStatus.getStateMachineId();
}

//------------------------------------------------------------------------------------
const std::string& MutableActionableStatus::getState(const ActionableStatusGuard& aGuard) const
{
  throwIfWrongGuard(aGuard);
  return mStatus.getState();
}

//------------------------------------------------------------------------------------
bool MutableActionableStatus::isBusy(const ActionableStatusGuard& aGuard) const
{
  throwIfWrongGuard(aGuard);
  return mStatus.isRunning();
}

//------------------------------------------------------------------------------------
const std::vector<const Functionoid*>& MutableActionableStatus::getRunningActions(const ActionableStatusGuard& aGuard) const
{
  throwIfWrongGuard(aGuard);
  return mStatus.getRunningActions();
}

//------------------------------------------------------------------------------------
const Functionoid* MutableActionableStatus::getLastRunningAction(const ActionableStatusGuard& aGuard) const
{
  throwIfWrongGuard(aGuard);
  return mStatus.getLastRunningAction();
}

//------------------------------------------------------------------------------------
void MutableActionableStatus::setStateMachine(const std::string& aStateMachine, const std::string& aState, const ActionableStatusGuard& aGuard)
{
  throwIfWrongGuard(aGuard);
  mStatus.mStateMachineId = aStateMachine;
  mStatus.mState = aState;
}

//------------------------------------------------------------------------------------
void MutableActionableStatus::setNoStateMachine(const ActionableStatusGuard& aGuard)
{
  throwIfWrongGuard(aGuard);
  mStatus.mStateMachineId = ActionableStatus::kNullStateMachineId;
  mStatus.mState = ActionableStatus::kNullStateId;
}

//------------------------------------------------------------------------------------
void MutableActionableStatus::setState(const std::string& aState, const ActionableStatusGuard& aGuard)
{
  throwIfWrongGuard(aGuard);
  mStatus.mState = aState;
}

//------------------------------------------------------------------------------------
void MutableActionableStatus::addAction(const Functionoid& aAction, const ActionableStatusGuard& aGuard)
{
  throwIfWrongGuard(aGuard);
  mStatus.mRunningActions.push_back(&aAction);  
}

//------------------------------------------------------------------------------------
void MutableActionableStatus::popAction(const ActionableStatusGuard& aGuard)
{
  throwIfWrongGuard(aGuard);
  mStatus.mRunningActions.pop_back();
}

//------------------------------------------------------------------------------------
void MutableActionableStatus::kill(const ActionableStatusGuard& aGuard)
{
  throwIfWrongGuard(aGuard);
  mStatus.mAlive = false;
}

//------------------------------------------------------------------------------------
void MutableActionableStatus::throwIfWrongGuard(const ActionableStatusGuard& aGuard) const
{
  if ( ! aGuard.isCorrectGuard(*this) )
    throw IncorrectActionableGuard("");
}


} // end ns core
} // end ns swatch
