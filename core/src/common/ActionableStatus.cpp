
#include "swatch/core/ActionableStatus.hpp"


// Standard C++ headers
#include <stdexcept>

// boost headers
#include "boost/iterator/indirect_iterator.hpp"



namespace swatch {
namespace core {

  
// STATIC MEMBERS INITIALIZATION
const std::string ActionableStatus::kNullStateMachineId = "";
const std::string ActionableStatus::kNullStateId = "";


//------------------------------------------------------------------------------------
ActionableStatus::ActionableStatus() :
  mAlive(true), 
  mStateMachineId(kNullStateMachineId),
  mState(kNullStateId),
  mUpdatingMetrics(false),
  mWaitingToRunAction(false)
{
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
bool ActionableStatus::isUpdatingMetrics() const {
  return mUpdatingMetrics;
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


bool ActionableStatus::isActionWaitingToRun() const
{
  return mWaitingToRunAction;
}




//------------------------------------------------------------------------------------
ActionableStatusGuardMap_t lockMutexes(const std::map<const MonitorableObject*, const MutableActionableStatus*>& aStatusMap)
{
  typedef std::map<const MonitorableObject*, const MutableActionableStatus*>::const_iterator ConstIt_t;

  // Lock the mutexes ...
  std::vector<boost::mutex*> lMutexes;
  for(ConstIt_t lIt=aStatusMap.begin(); lIt != aStatusMap.end(); lIt++)
    lMutexes.push_back(& lIt->second->mMutex);

  boost::indirect_iterator<std::vector<boost::mutex*>::iterator> begin(lMutexes.begin()), end(lMutexes.end());
  boost::lock(begin, end);

  // ... then put them into lock guards
  ActionableStatusGuardMap_t lLockGuardMap;
  for(ConstIt_t lIt=aStatusMap.begin(); lIt != aStatusMap.end(); lIt++)
    lLockGuardMap[lIt->first] = ActionableStatusGuardPtr_t(new ActionableStatusGuard(*lIt->second, boost::adopt_lock_t()));
  
  return lLockGuardMap;
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
bool MutableActionableStatus::isUpdatingMetrics(const MonitorableStatusGuard& aGuard) const
{
  throwIfWrongGuard(aGuard);
  return mStatus.isUpdatingMetrics();
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
  if( mStatus.mRunningActions.empty() )
    mConditionVar.notify_all();
}

//------------------------------------------------------------------------------------
void MutableActionableStatus::kill(const ActionableStatusGuard& aGuard)
{
  throwIfWrongGuard(aGuard);
  mStatus.mAlive = false;
}


//------------------------------------------------------------------------------------
void MutableActionableStatus::finishedUpdatingMetrics(const MonitorableStatusGuard& aGuard)
{
  throwIfWrongGuard(aGuard);
  assert ( mStatus.mUpdatingMetrics );

  mStatus.mUpdatingMetrics = false;
  mConditionVar.notify_all();
}


//------------------------------------------------------------------------------------
void MutableActionableStatus::waitUntilReadyToUpdateMetrics(MonitorableStatusGuard& aGuard)
{
  throwIfWrongGuard(aGuard);

  while((mStatus.mRunningActions.size() > 0) || mStatus.mWaitingToRunAction || isUpdatingMetrics(aGuard))
  {
    mConditionVar.wait(getUniqueLock(aGuard));
  }
  
  mStatus.mUpdatingMetrics = true;
}


//------------------------------------------------------------------------------------
void MutableActionableStatus::waitUntilReadyToRunAction(const Functionoid& aAction, ActionableStatusGuard& aGuard)
{
  throwIfWrongGuard(aGuard);
  
  if ( getRunningActions(aGuard).size() )
    throw std::runtime_error("waitUntilReadyToRunAction: actions already running");
  else if ( mStatus.mWaitingToRunAction )
    throw std::runtime_error("waitUntilReadyToRunAction: another thread already waiting");
  
  mStatus.mWaitingToRunAction = true;
  
  while ( isUpdatingMetrics(aGuard) )
  {
    mConditionVar.wait(getUniqueLock(aGuard));
  }

  mStatus.mRunningActions.push_back(&aAction);  
  mStatus.mWaitingToRunAction = false;
}


void MutableActionableStatus::waitUntilReadyToRunAction(const std::vector<std::pair<MutableActionableStatus*, ActionableStatusGuard*> >& aVec, const Functionoid& aAction)
{
  // 1) Check that:
  //     - there aren't any NULL pointers; and ...
  //     - the guards are all for the correct status instance;
  typedef std::vector<std::pair<MutableActionableStatus*, ActionableStatusGuard*> > Vec_t;
  for(Vec_t::const_iterator lIt=aVec.begin(); lIt!=aVec.end(); lIt++)
  {
    if (lIt->first == NULL)
      std::runtime_error("waitUntilReadyToRunAction: status pointer in element " + boost::lexical_cast<std::string>(lIt-aVec.begin()) + " is NULL");
    else if (lIt->second == NULL)
      std::runtime_error("waitUntilReadyToRunAction: guard pointer in element " + boost::lexical_cast<std::string>(lIt-aVec.begin()) + " is NULL");

    lIt->first->throwIfWrongGuard(*lIt->second);
  }

  // 2) Check that:
  //     - no actions are already running; and ...
  //     - no other threads are already waiting
  for(Vec_t::const_iterator lIt=aVec.begin(); lIt!=aVec.end(); lIt++)
  {
    if ( lIt->first->getRunningActions(*lIt->second).size() )
      throw std::runtime_error("waitUntilReadyToRunAction: actions already running");
    else if ( lIt->first->mStatus.mWaitingToRunAction )
      throw std::runtime_error("waitUntilReadyToRunAction: another thread already waiting");
  }

  // 3) Declare action waiting to run on each status instance, and unlock the mutexes (to avoid deadlocks whilst waiting for each status to change)
  for(Vec_t::const_iterator lIt=aVec.begin(); lIt!=aVec.end(); lIt++)
  {
    lIt->first->mStatus.mWaitingToRunAction = true;
    lIt->first->getUniqueLock(*lIt->second).unlock();
  }

  // 4) Re-lock the mutex for each status instance individually, and wait until isUpdatingMetrics() returns false
  for(Vec_t::const_iterator lIt=aVec.begin(); lIt!=aVec.end(); lIt++)
  {
    ActionableStatusGuard lGuard(*lIt->first);
    while ( lIt->first->isUpdatingMetrics(lGuard) )
    {
      lIt->first->mConditionVar.wait(lIt->first->getUniqueLock(lGuard));
    }
    lIt->first->mStatus.mRunningActions.push_back(&aAction);  
    lIt->first->mStatus.mWaitingToRunAction = false;
  }

  // 5) Re-lock all the original lock guard objects 
  std::vector<boost::unique_lock<boost::mutex>*> lLockGuardVec;
  for(Vec_t::const_iterator lIt=aVec.begin(); lIt!=aVec.end(); lIt++)
    lLockGuardVec.push_back( &lIt->first->getUniqueLock(*lIt->second) );

  boost::indirect_iterator<std::vector<boost::unique_lock<boost::mutex>*>::iterator> begin(lLockGuardVec.begin()), end(lLockGuardVec.end());
  boost::lock(begin, end);
}


//------------------------------------------------------------------------------------
void MutableActionableStatus::throwIfWrongGuard(const MonitorableStatusGuard& aGuard) const
{
  if ( ! aGuard.isCorrectGuard(*this) )
    throw IncorrectActionableGuard("");
}


} // end ns core
} // end ns swatch
