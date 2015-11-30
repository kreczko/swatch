
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


namespace swatch {
namespace core {

//------------------------------------------------------------------------------------
ActionableSystem::ActionableSystem(const std::string& aId, const std::string& aLoggerName) :
  MonitorableObject(aId),
  mStatus(),
  mLogger(swatch::logger::Logger::getInstance(aLoggerName)){
}


//------------------------------------------------------------------------------------
ActionableSystem::~ActionableSystem() {
}

//------------------------------------------------------------------------------------
std::set<std::string> ActionableSystem::getStateMachines() const
{
  std::set<std::string> lNames;
  for(tStateMachineMap::const_iterator lIt=mFSMs.begin(); lIt!=mFSMs.end(); lIt++)
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
SystemStateMachine& ActionableSystem::registerStateMachine( const std::string& aId, const std::string& aInitialState, const std::string& aErrorState ) {
  if (mFSMs.count(aId))
    throw StateMachineAlreadyExistsInActionableObject( "State machine With ID '"+aId+"' already exists" );

  SystemStateMachine* lFSM = new SystemStateMachine(aId, *this, mStatus, aInitialState, aErrorState);
  addObj(lFSM);
  mFSMs.insert( std::make_pair( aId , lFSM ) );
  return *lFSM;
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
ActionableSystem::StatusGuardMap_t ActionableSystem::lockMutexes(const SystemStateMachine& aOp)
{
  // Lock the mutexes ...
  std::vector<boost::mutex*> lMutexes;
  lMutexes.push_back( &aOp.getActionable().mStatus.mMutex );
  for(std::set<const StateMachine*>::const_iterator lIt=aOp.getParticipants().begin(); lIt!=aOp.getParticipants().end(); lIt++)
    lMutexes.push_back( &(*lIt)->getActionable().mStatus.mMutex );

  boost::indirect_iterator<std::vector<boost::mutex*>::iterator> begin(lMutexes.begin()), end(lMutexes.end());
  boost::lock(begin, end);

  // ... then put them into lock guards
  std::map<const MonitorableObject*, StatusGuardPtr_t> lLockGuardMap;
  lLockGuardMap[&aOp.getActionable()] = StatusGuardPtr_t(new ActionableStatusGuard(aOp.getActionable().mStatus, boost::adopt_lock_t()));
  for(std::set<const StateMachine*>::const_iterator lIt=aOp.getParticipants().begin(); lIt!=aOp.getParticipants().end(); lIt++)
    lLockGuardMap[&(*lIt)->getActionable()] = StatusGuardPtr_t( new ActionableStatusGuard((*lIt)->getActionable().mStatus, boost::adopt_lock_t()) );
  
  return lLockGuardMap;
}


//------------------------------------------------------------------------------------
ActionableSystem::BusyGuard::BusyGuard(ActionableSystem& aResource, const Functionoid& aAction) : 
  mSystem(aResource),
  mStatus(aResource.mStatus),
  mAction(aAction)
{
  const SystemTransition& lTransition = dynamic_cast<const SystemTransition&>(aAction);
  const SystemStateMachine& lOp = lTransition.getStateMachine();
  
  // Put the mutexes into lock guards before continuing
  std::map<const MonitorableObject*, StatusGuardPtr_t> lStatusGuardMap = ActionableSystem::lockMutexes(lOp);
  const ActionableStatusGuard& lStatusGuard = *lStatusGuardMap.at(&mSystem);

  // 1a) Check that system is engaged in correct state machine, and is in the correct state
  if( mStatus.getStateMachineId(lStatusGuard) !=  lTransition.getStateMachine().getId())
    throw ResourceInWrongStateMachine("Resource '"+aResource.getPath()+"' is not yet engaged in state machine '"+lTransition.getStateMachine().getId()+"'");

  else if ( mStatus.getState(lStatusGuard) != lTransition.getStartState() )
    throw ResourceInWrongState("Resource '"+aResource.getPath()+"' is in state '"+mStatus.getState(lStatusGuard)+"'; transition '"+lTransition.getId()+"' cannot be run");
  
  // 1b) Check that children are engaged in correct state machine, and are in the correct state
  typedef std::map<ActionableObject*, const StateMachine::Transition* > tObjTransitionMap;
  tObjTransitionMap childTransitionMap;
  for(SystemTransition::const_iterator lIt=(lTransition.end()-1); lIt != (lTransition.begin()-1); lIt--)
  {
    for(std::vector<StateMachine::Transition*>::const_iterator lIt2=lIt->cget().begin(); lIt2!=lIt->cget().end(); lIt2++)
      childTransitionMap[ &(*lIt2)->getStateMachine().getActionable() ] = *lIt2;
  }

  BOOST_FOREACH( const tObjTransitionMap::value_type e, childTransitionMap ) {
    const ActionableStatusGuard& lGuard = *lStatusGuardMap.at(e.first);
    if( e.first->mStatus.getStateMachineId(lGuard) != e.second->getStateMachine().getId() )
      throw ResourceInWrongStateMachine("Resource '"+e.first->getPath()+"' is not yet engaged in state machine '"+e.second->getStateMachine().getId()+"'");
    else if ( e.first->mStatus.getState(lGuard) != e.second->getStartState() )
      throw ResourceInWrongState("Resource '"+e.first->getPath()+"' is in state "+e.first->mStatus.getState(lGuard)+", transition '"+e.second->getId()+"' cannot be run");
  }
  
  // 2a) Check that the system is not busy
  Status_t lSysSnapshot = mStatus.getSnapshot(lStatusGuard);
  if ( !lSysSnapshot.isAlive() || lSysSnapshot.isRunning() )
  {
    std::ostringstream oss;
    oss << "Could not run action '" << lTransition.getId() << "' on resource '" << aResource.getPath() << "'. ";

    if ( lSysSnapshot.isRunning() )
      oss << "Resource currently busy running functionoid '" << lSysSnapshot.getLastRunningAction()->getId() << "'.";
    else
      oss << "Actions currently disabled on this resource.";

    LOG4CPLUS_INFO(aResource.getLogger(), oss.str());
    throw ActionableObjectIsBusy(oss.str());
  }

  // 2b) Check that none of the children are busy
  BOOST_FOREACH( const tObjTransitionMap::value_type e, childTransitionMap )
  {
    const ActionableObject::MutableStatus& lChild = (e.first)->mStatus;
    const ActionableStatusGuard& lGuard = *lStatusGuardMap.at(e.first);

    if ( !lChild.isAlive(lGuard) || lChild.isBusy(lGuard) )
    {
      std::ostringstream oss;
      oss << "Could not run system action '" << lTransition.getId() << "' due to child resource '" << aResource.getPath() << "'. ";

      if ( lChild.isBusy(lGuard) )
        oss << "Child currently busy running functionoid '" << lChild.getLastRunningAction(lGuard)->getId() << "'.";
      else
        oss << "Actions currently disabled on this child.";

      LOG4CPLUS_INFO(aResource.getLogger(), oss.str());
      throw ActionableObjectIsBusy(oss.str());
    }
  } 
  
  // 3) If got this far, then all is good; create the busy guards for the children
  LOG4CPLUS_INFO(aResource.getLogger(), mSystem.getPath() << " : Starting action '" << mAction.getId() << "'");
  mStatus.addAction(mAction, lStatusGuard);
  
  BOOST_FOREACH( const tObjTransitionMap::value_type e, childTransitionMap )
    this->mChildGuardMap[ e.first ] = tChildGuardPtr(new swatch::core::BusyGuard(*e.first, (e.first)->mStatus, *lStatusGuardMap[e.first].get(), lTransition) );
}


//------------------------------------------------------------------------------------
const swatch::core::BusyGuard& ActionableSystem::BusyGuard::getChildGuard(const ActionableObject& aChild) const
{
  std::map<const ActionableObject*, tChildGuardPtr>::const_iterator lIt = mChildGuardMap.find(&aChild);
  if (lIt == mChildGuardMap.end())
    throw std::runtime_error("Non-child object '"+aChild.getPath()+"' passed to BusyGuard for system '"+mSystem.getPath()+"', action '"+mAction.getId()+"'");
  return *(lIt->second);
}


//------------------------------------------------------------------------------------
ActionableSystem::BusyGuard::~BusyGuard()
{
  ActionableStatusGuard lGuard(mStatus);
  LOG4CPLUS_INFO(mSystem.getLogger(), mSystem.getPath() << " : Finished action '" << mAction.getId() << "'");

  if ( mStatus.isBusy(lGuard) && (&mAction == mStatus.getLastRunningAction(lGuard)) )
  {
    mStatus.popAction(lGuard);
    
    // In case this was a transition, also update object's current state
    if (const SystemTransition* t = dynamic_cast<const SystemTransition*>(&mAction))
    {
      if(t->getStatus().getState() == ActionStatus::kError)
        mStatus.setState(t->getStateMachine().getErrorState(), lGuard);
      else
        mStatus.setState(t->getEndState(), lGuard);
    }
  }
  else
  {
    size_t lNrActions = mStatus.getRunningActions(lGuard).size();
    const std::string activeFuncId(lNrActions > 0 ? "NULL" : "'" + mStatus.getLastRunningAction(lGuard)->getId() + "' (innermost of "+boost::lexical_cast<std::string>(lNrActions)+")");
    LOG4CPLUS_ERROR(mSystem.getLogger(),
        "unexpected active functionoid " << activeFuncId << "  in BusyGuard destructor for system '"
        << mSystem.getPath() << "', functionoid '" << mAction.getId() << "'");
  }
}

log4cplus::Logger& ActionableSystem::getLogger() {
  return mLogger;
}

}
}
