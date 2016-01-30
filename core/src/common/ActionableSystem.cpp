
#include "swatch/core/ActionableSystem.hpp"

// Boost Headers
#include "boost/iterator/indirect_iterator.hpp"
#include "boost/thread/lock_options.hpp"
#include "boost/foreach.hpp"

// Swatch Headers
#include "swatch/core/SystemStateMachine.hpp"
#include "swatch/logger/Logger.hpp"

//log4cplus headers
#include <log4cplus/loggingmacros.h>
#include <bits/stl_map.h>
#include <map>


namespace swatch {
namespace core {


const ActionableSystem& SystemFunctionoid::getActionable() const
{
  return mActionable;
}

ActionableSystem& SystemFunctionoid::getActionable()
{
  return mActionable;
}

const std::set<ActionableObject*>& SystemFunctionoid::getParticipants() 
{
  return mParticipants;
}

SystemFunctionoid::SystemFunctionoid(const std::string& aId, ActionableSystem& aActionable) :
  Functionoid(aId),
  mActionable(aActionable)
{
}


void SystemFunctionoid::addParticipant(ActionableObject& aObj)
{
  mParticipants.insert(&aObj);
}



ActionableSystem::StatusContainer::StatusContainer(const ActionableSystem& aSystem, ActionableStatus& aSysStatus) :
  mSystem(aSystem),
  mSysStatus(aSysStatus)
{
}

ActionableSystem::StatusContainer::~StatusContainer()
{
}

const ActionableStatus& ActionableSystem::StatusContainer::getSystemStatus() const
{
  return mSysStatus;
}

const ActionableStatus& ActionableSystem::StatusContainer::getStatus(const ActionableObject& aChild ) const
{
  return *mChildStatusMap.at(&aChild);
}

ActionableStatus& ActionableSystem::StatusContainer::getSystemStatus()
{
  return mSysStatus;
}


ActionableStatus& ActionableSystem::StatusContainer::getStatus(const ActionableObject& aChild )
{
  return *mChildStatusMap.at(&aChild);
}


ActionableSystem::GuardMap_t ActionableSystem::StatusContainer::lockMutexes() const
{
  // 1) Lock all of the actionable status instances
  std::vector<const ActionableStatus*> lStatusVec;
  lStatusVec.push_back(&mSysStatus);
  for(auto lIt = mChildStatusMap.begin(); lIt != mChildStatusMap.end(); lIt++)
    lStatusVec.push_back(lIt->second);
  ActionableStatusGuardVec_t lGuardVec = swatch::core::lockMutexes<>(lStatusVec.begin(), lStatusVec.end());
  
  // 2) Create map of actionable system/object to the corresponding actionable status guard instance
  ActionableSystem::GuardMap_t lGuardMap;
  lGuardMap[&mSystem] = *lGuardVec.begin();
  auto lChildStatusMapIt = mChildStatusMap.begin();
  for(auto lGuardIt = (lGuardVec.begin()+1); lGuardIt != lGuardVec.end(); lGuardIt++, lChildStatusMapIt++)
    lGuardMap[lChildStatusMapIt->first] = *lGuardIt;
  
  return lGuardMap;
}


//------------------------------------------------------------------------------------
ActionableSystem::ActionableSystem(const std::string& aId, const std::string& aLoggerName) :
  MonitorableObject(aId),
  mStatus(),
  mStatusMap(*this, mStatus),
  mLogger(swatch::logger::Logger::getInstance(aLoggerName))
{
}


//------------------------------------------------------------------------------------
ActionableSystem::~ActionableSystem() {
}

//------------------------------------------------------------------------------------
std::set<std::string> ActionableSystem::getStateMachines() const
{
  std::set<std::string> lNames;
  for(StateMachineMap_t::const_iterator lIt=mFSMs.begin(); lIt!=mFSMs.end(); lIt++)
    lNames.insert( lIt->first );
  return lNames;
}


//------------------------------------------------------------------------------------
SystemStateMachine& ActionableSystem::getStateMachine( const std::string& aId )
{
  try {
    return *(mFSMs.at( aId ));
  } catch ( const std::out_of_range& e ) {
    throw StateMachineNotFoundInActionableObject( "Unable to find state machine with ID '" + aId + "' in object '" + getPath() + "'");
  }
}


//------------------------------------------------------------------------------------
ActionableSystem::Status_t ActionableSystem::getStatus() const
{
  ActionableStatusGuard lGuard(mStatus);
  return mStatus.getSnapshot(lGuard);
}


//------------------------------------------------------------------------------------
const ActionableSystem::ActionableChildMap_t& ActionableSystem::getActionableChildren()
{
  return mActionableChildren;
}

//------------------------------------------------------------------------------------
SystemStateMachine& ActionableSystem::registerStateMachine( const std::string& aId, const std::string& aInitialState, const std::string& aErrorState ) {
  if (mFSMs.count(aId))
    throw StateMachineAlreadyExistsInActionableObject( "State machine With ID '"+aId+"' already exists" );

  SystemStateMachine* lFSM = new SystemStateMachine(aId, *this, mStatusMap, aInitialState, aErrorState);
  addObj(lFSM);
  mFSMs.insert( std::make_pair( aId , lFSM ) );
  return *lFSM;
}


//------------------------------------------------------------------------------------
void ActionableSystem::addActionable(ActionableObject* aChildActionable)
{
  addObj(aChildActionable, ActionableObject::Deleter());
  
  mStatusMap.mChildStatusMap[aChildActionable] = &aChildActionable->mStatus;
  
  mActionableChildren[aChildActionable->getId()] = aChildActionable;
}


//------------------------------------------------------------------------------------
void ActionableSystem::Deleter::operator ()(Object* aObject) {
  if(ActionableSystem* lActionableSys = dynamic_cast<ActionableSystem*>(aObject))
  {
    LOG4CPLUS_INFO(lActionableSys->getLogger(), aObject->getPath() << " : ActionableSystem deleter called");

    {
      ActionableStatusGuard lGuard(lActionableSys->mStatus);
      lActionableSys->mStatus.kill(lGuard);
    }

    //TODO (low-ish priority): Eventually replace this "spinning" do-loop with a more efficient implementation based on ActionableSystem/Functionoid methods that use conditional variables behind-the-scenes 
    do {
    } while ( ! lActionableSys->getStatus().getRunningActions().empty() );

    LOG4CPLUS_INFO(lActionableSys->getLogger(), aObject->getPath() << " : ActionableSystem now being deleted");

    delete lActionableSys;
  }
  else{
    log4cplus::Logger lLogger = swatch::logger::Logger::getInstance("swatch.core.ActionableSystem");
    LOG4CPLUS_WARN(lLogger,
        "ActionableSystem::Deleter being used on object '" << aObject->getPath() << "' of type '"
        << aObject->getTypeName() << "' that doesn't inherit from ActionableSystem");
    delete aObject;
  }
}


//------------------------------------------------------------------------------------
SystemBusyGuard::SystemBusyGuard(SystemFunctionoid& aAction, ActionableSystem::StatusContainer& aStatusMap, const ActionableSystem::GuardMap_t& aStatusGuardMap, const Callback_t& aCallback) : 
  mSystem(aAction.getActionable()),
  mSysStatus(aStatusMap.getSystemStatus()),
  mAction(aAction),
  mPostActionCallback(aCallback)
{
  ActionableStatusGuard& lSysGuard = *aStatusGuardMap.at(&mSystem);
  ActionableStatus& lSysStatus = aStatusMap.getSystemStatus();
  
  // 1) Check that the system is not busy
  ActionableSystem::Status_t lSysSnapshot = lSysStatus.getSnapshot(lSysGuard);
  if ( !lSysSnapshot.isAlive() || lSysSnapshot.isRunning() )
  {
    std::ostringstream oss;
    oss << "Could not run action '" << mAction.getId() << "' on resource '" << mSystem.getPath() << "'. ";

    if ( lSysSnapshot.isRunning() )
      oss << "Resource currently busy running functionoid '" << lSysSnapshot.getLastRunningAction()->getId() << "'.";
    else
      oss << "Actions currently disabled on this resource.";

    LOG4CPLUS_INFO(mSystem.getLogger(), oss.str());
    throw ActionableObjectIsBusy(oss.str());
  }

  // Generate list of ENABLED participating children
  std::set<ActionableObject*> lEnabledParticipants;
  typedef std::set<ActionableObject*>::const_iterator ChildIt_t;
  for (ChildIt_t lIt = aAction.getParticipants().begin(); lIt != aAction.getParticipants().end(); lIt++)
  {
    const ActionableStatusGuard& lChildGuard = *aStatusGuardMap.at(*lIt);
    if (aStatusMap.getStatus(**lIt).isEnabled(lChildGuard))
      lEnabledParticipants.insert(*lIt);
  }

  // 2) Check that none of the children are busy
  for (ChildIt_t lIt = lEnabledParticipants.begin(); lIt != lEnabledParticipants.end(); lIt++)
  {
    const ActionableStatus& lChildStatus = aStatusMap.getStatus(**lIt);
    const ActionableStatusGuard& lChildGuard = *aStatusGuardMap.at(*lIt);
    
    if ( !lChildStatus.isAlive(lChildGuard) || lChildStatus.isBusy(lChildGuard) )
    {
      std::ostringstream oss;
      oss << "Could not run system action '" << mAction.getId() << "' due to child resource '" << (*lIt)->getPath() << "'. ";

      if ( lChildStatus.isBusy(lChildGuard) )
        oss << "Child currently busy running functionoid '" << lChildStatus.getLastRunningAction(lChildGuard)->getId() << "'.";
      else
        oss << "Actions currently disabled on this child.";

      LOG4CPLUS_INFO(mSystem.getLogger(), oss.str());
      throw ActionableObjectIsBusy(oss.str());
    }
  } 
  
  // 3) If got this far, then all is good: 
  //    a) wait until actions ready to run on system and child objects; then ...
  //    b) create the busy guards for the children
  LOG4CPLUS_INFO(mSystem.getLogger(), "Starting system action '" << mAction.getId() << "'");

  std::vector<std::pair<ActionableStatus*, ActionableStatusGuard*> > lStatusVec;
  lStatusVec.push_back( std::pair<ActionableStatus*, ActionableStatusGuard*>(&lSysStatus, &lSysGuard) );
  for (ChildIt_t lIt = lEnabledParticipants.begin(); lIt != lEnabledParticipants.end(); lIt++)
    lStatusVec.push_back( std::pair<ActionableStatus*, ActionableStatusGuard*>(&aStatusMap.getStatus(**lIt), aStatusGuardMap.at(*lIt).get()) );
  ActionableStatus::waitUntilReadyToRunAction(lStatusVec, mAction);
  
  for (ChildIt_t lIt = lEnabledParticipants.begin(); lIt != lEnabledParticipants.end(); lIt++) {
    ActionableStatusGuard& lChildGuard = *aStatusGuardMap.at(*lIt).get();
    ActionableStatus& lChildStatus = aStatusMap.getStatus(**lIt);
    mChildGuardMap[ *lIt ] = ChildGuardPtr_t(new BusyGuard(**lIt, lChildStatus, lChildGuard, mAction, BusyGuard::Adopt()) );
  }
}


//------------------------------------------------------------------------------------
const BusyGuard& SystemBusyGuard::getChildGuard(const ActionableObject& aChild) const
{
  std::map<const ActionableObject*, ChildGuardPtr_t>::const_iterator lIt = mChildGuardMap.find(&aChild);
  if (lIt == mChildGuardMap.end())
    throw std::runtime_error("Non-participating/disabled object '"+aChild.getPath()+"' passed to SystemBusyGuard::getChildGuard(...) for system '"+mSystem.getPath()+"', action '"+mAction.getId()+"'");
  return *(lIt->second);
}


//------------------------------------------------------------------------------------
SystemBusyGuard::~SystemBusyGuard()
{
  ActionableStatusGuard lSysGuard(mSysStatus);

  if ( mSysStatus.isBusy(lSysGuard) && (&mAction == mSysStatus.getLastRunningAction(lSysGuard)) )
  {
    mSysStatus.popAction(lSysGuard);
    
    std::ostringstream lLogSuffixStream;
    if ( !mPostActionCallback.empty() )
      mPostActionCallback(lSysGuard, lLogSuffixStream);

    std::string lLogMsgSuffix = (lLogSuffixStream.tellp() > 0 ? ". " + lLogSuffixStream.str() : std::string());
    LOG4CPLUS_INFO(mSystem.getLogger(), "Finished system action '" << mAction.getId() << lLogMsgSuffix);
  }
  else
  {
    size_t lNrActions = mSysStatus.getRunningActions(lSysGuard).size();
    const std::string activeFuncId(lNrActions > 0 ? "NULL" : "'" + mSysStatus.getLastRunningAction(lSysGuard)->getId() + "' (innermost of "+boost::lexical_cast<std::string>(lNrActions)+")");
    LOG4CPLUS_ERROR(mSystem.getLogger(),
        "unexpected active functionoid " << activeFuncId << "  in SystemBusyGuard destructor for action '" << mAction.getId() << "'");
  }
}


log4cplus::Logger& ActionableSystem::getLogger()
{
  return mLogger;
}

}
}
