
#include "swatch/core/ActionableSystem.hpp"

// Boost Headers
#include "boost/iterator/indirect_iterator.hpp"
#include "boost/thread/lock_options.hpp"
#include "boost/foreach.hpp"

// Swatch Headers
#include "swatch/core/SystemStateMachine.hpp"
#include "swatch/logger/Log.hpp"


namespace swatch {
namespace core {

  
//------------------------------------------------------------------------------------
ActionableSystem::ActionableSystem(const std::string& aId) : 
  MonitorableObject(aId) {
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
ActionableSystem::Status ActionableSystem::getStatus() const
{
  boost::lock_guard<boost::mutex> lGuard(mMutex);
  return mStatus;
}


//------------------------------------------------------------------------------------
void ActionableSystem::engageStateMachine(const std::string& aFSM)
{
  const SystemStateMachine& lOp = getStateMachine(aFSM);

  tLockGuardMap lLockGuardMap = lockMutexes(lOp);

  // Throw if system or any of the participating children are already in a state machine
  if(mStatus.getStateMachineId() != ActionableStatus::kNullStateMachineId )
    throw ResourceInWrongStateMachine("Cannot engage other state machine; system '"+getPath()+"' currently in state machine"+mStatus.getStateMachineId()+"'");
  
  for(std::set<const StateMachine*>::const_iterator lIt=lOp.getParticipants().begin(); lIt!=lOp.getParticipants().end(); lIt++)
  {
    const ActionableObject& lChild = (*lIt)->getActionable();
    if( lChild.mStatus.isEngaged() )
  	  throw ResourceInWrongStateMachine("Cannot engage other state machine; resource '"+lChild.getPath()+"' currently in state machine '"+lChild.mStatus.getStateMachineId()+"'");
  }
  
  // ... otherwise all is good, engage system & all participating children in appropriate state machine
	mStatus.mStateMachineId = lOp.getId();
  mStatus.mState = lOp.getInitialState();
  
  BOOST_FOREACH( const StateMachine* sm, lOp.getParticipants() ) {
  	ActionableObject& lObj = sm->mResource;
  	lObj.mStatus.mStateMachineId = sm->getId();
    lObj.mStatus.mState = sm->getInitialState();
  }
}


//------------------------------------------------------------------------------------
SystemStateMachine& ActionableSystem::registerStateMachine( const std::string& aId, const std::string& aInitialState, const std::string& aErrorState ) {
  if (mFSMs.count(aId))
    throw StateMachineAlreadyExistsInActionableObject( "State machine With ID '"+aId+"' already exists" );

  SystemStateMachine* lFSM = new SystemStateMachine(aId, *this, aInitialState, aErrorState);
  mFSMs.insert( std::make_pair( aId , lFSM ) );
  return *lFSM;
}


//------------------------------------------------------------------------------------
void ActionableSystem::Deleter::operator ()(Object* aObject) {
  if(ActionableSystem* lActionableSys = dynamic_cast<ActionableSystem*>(aObject))
  {
    LOG(swatch::logger::kNotice) << aObject->getPath() << " : ActionableSystem deleter called";

    lActionableSys->disableActions();

    //TODO (low-ish priority): Eventually replace this "spinning" do-loop with a more efficient implementation based on ActionableSystem/Functionoid methods that use conditional variables behind-the-scenes 
    do {
    } while ( ! lActionableSys->getStatus().getRunningActions().empty() );

    LOG(swatch::logger::kNotice) << aObject->getPath() << " : ActionableSystem now being deleted";

    delete lActionableSys;
  }
  else{
    LOG(swatch::logger::kWarning) << "ActionableSystem::Deleter being used on object '" << aObject->getPath() << "' of type '" << aObject->getTypeName() << "' that doesn't inherit from ActionableSystem";
    delete aObject;
  }
}


//------------------------------------------------------------------------------------
void ActionableSystem::disableActions(){
  boost::lock_guard<boost::mutex> lGuard(mMutex);
  mStatus.mAlive = false;
}


//------------------------------------------------------------------------------------
ActionableSystem::tLockGuardMap ActionableSystem::lockMutexes(const SystemStateMachine& aOp)
{
  // Lock the mutexes ...
  std::vector<boost::mutex*> lMutexes;
  lMutexes.push_back( &aOp.getActionable().mMutex );
  for(std::set<const StateMachine*>::const_iterator lIt=aOp.getParticipants().begin(); lIt!=aOp.getParticipants().end(); lIt++)
    lMutexes.push_back( &(*lIt)->mResource.mMutex );

  boost::indirect_iterator<std::vector<boost::mutex*>::iterator> begin(lMutexes.begin()), end(lMutexes.end());
  boost::lock(begin, end);

  // ... then put them into lock guards
  std::map<const MonitorableObject*, tLockGuardPtr> lLockGuardMap;
  lLockGuardMap[&aOp.getActionable()] = tLockGuardPtr(new boost::unique_lock<boost::mutex>(aOp.getActionable().mMutex, boost::adopt_lock_t()));
  for(std::set<const StateMachine*>::const_iterator lIt=aOp.getParticipants().begin(); lIt!=aOp.getParticipants().end(); lIt++)
    lLockGuardMap[&(*lIt)->getActionable()] = tLockGuardPtr( new boost::unique_lock<boost::mutex>((*lIt)->getActionable().mMutex, boost::adopt_lock_t()) );
  
  return lLockGuardMap;
}


//------------------------------------------------------------------------------------
ActionableSystem::BusyGuard::BusyGuard(ActionableSystem& aResource, const Functionoid& aAction) : 
  mSystem(aResource),
  mAction(aAction)
{
  const SystemTransition& lTransition = dynamic_cast<const SystemTransition&>(aAction);
  const SystemStateMachine& lOp = lTransition.getStateMachine();
  
  // Put the mutexes into lock guards before continuing
  std::map<const MonitorableObject*, tLockGuardPtr> lLockGuardMap = ActionableSystem::lockMutexes(lOp);

  // 1a) Check that system is engaged in correct state machine, and is in the correct state
  if( aResource.mStatus.getStateMachineId() !=  lTransition.getStateMachine().getId())
    throw ResourceInWrongStateMachine("Resource '"+aResource.getPath()+"' is not yet engaged in state machine '"+lTransition.getStateMachine().getId()+"'");
	
  else if ( aResource.mStatus.mState != lTransition.getStartState() )
    throw ResourceInWrongState("Resource '"+aResource.getPath()+"' is in state '"+aResource.mStatus.mState+"'; transition '"+lTransition.getId()+"' cannot be run");
  
  // 1b) Check that children are engaged in correct state machine, and are in the correct state
  typedef std::map<ActionableObject*, const StateMachine::Transition* > tObjTransitionMap;
  tObjTransitionMap childTransitionMap;
  for(SystemTransition::const_iterator lIt=(lTransition.end()-1); lIt != (lTransition.begin()-1); lIt--)
  {
    for(std::vector<StateMachine::Transition*>::const_iterator lIt2=lIt->cget().begin(); lIt2!=lIt->cget().end(); lIt2++)
      childTransitionMap[ &(*lIt2)->getStateMachine().getActionable() ] = *lIt2;
  }

  BOOST_FOREACH( const tObjTransitionMap::value_type e, childTransitionMap ) {
    if( e.first->mStatus.getStateMachineId() != e.second->getStateMachine().getId() )
      throw ResourceInWrongStateMachine("Resource '"+e.first->getPath()+"' is not yet engaged in state machine '"+e.second->getStateMachine().getId()+"'");
    else if ( e.first->mStatus.mState != e.second->getStartState() )
      throw ResourceInWrongState("Resource '"+e.first->getPath()+"' is in state "+e.first->mStatus.mState+", transition '"+e.second->getId()+"' cannot be run");
  }
  
  // 2a) Check that the system is not busy
  if ( !aResource.mStatus.isAlive() || aResource.mStatus.isRunning() )
  {
    std::ostringstream oss;
    oss << "Could not run action '" << lTransition.getId() << "' on resource '" << aResource.getPath() << "'. ";

    if ( aResource.mStatus.isRunning() )
      oss << "Resource currently busy running functionoid '" << aResource.mStatus.getLastRunningAction()->getId() << "'.";
    else
      oss << "Actions currently disabled on this resource.";

    LOG(swatch::logger::kNotice) << oss.str();
    throw ActionableObjectIsBusy(oss.str());
  }

  // 2b) Check that none of the children are busy
  BOOST_FOREACH( const tObjTransitionMap::value_type e, childTransitionMap )
  {
    const ActionableObject& lChild = *(e.first);
    
    if ( !lChild.mStatus.isAlive() || lChild.mStatus.isRunning() )
    {
      std::ostringstream oss;
      oss << "Could not run system action '" << lTransition.getId() << "' due to child resource '" << aResource.getPath() << "'. ";

      if ( lChild.mStatus.isRunning() )
        oss << "Child currently busy running functionoid '" << lChild.mStatus.getLastRunningAction()->getId() << "'.";
      else
        oss << "Actions currently disabled on this child.";

      LOG(swatch::logger::kNotice) << oss.str();
      throw ActionableObjectIsBusy(oss.str());
    }
  } 
  
  // 3) If got this far, then all is good; create the busy guards for the children
  LOG(swatch::logger::kInfo) << mSystem.getPath() << " : Starting action '" << mAction.getId() << "'";
  aResource.mStatus.mRunningActions.push_back( &aAction );
  
  BOOST_FOREACH( const tObjTransitionMap::value_type e, childTransitionMap )
    this->mChildGuardMap[ e.first ] = tChildGuardPtr(new ActionableObject::BusyGuard(*e.first, *lLockGuardMap[e.first].get(), lTransition) );
}


//------------------------------------------------------------------------------------
const ActionableObject::BusyGuard& ActionableSystem::BusyGuard::getChildGuard(const ActionableObject& aChild) const
{
  std::map<const ActionableObject*, tChildGuardPtr>::const_iterator lIt = mChildGuardMap.find(&aChild);
  if (lIt == mChildGuardMap.end())
    throw std::runtime_error("Non-child object '"+aChild.getPath()+"' passed to BusyGuard for system '"+mSystem.getPath()+"', action '"+mAction.getId()+"'");
  return *(lIt->second);
}


//------------------------------------------------------------------------------------
ActionableSystem::BusyGuard::~BusyGuard()
{
  boost::lock_guard<boost::mutex> lGuard(mSystem.mMutex);
  // TO CHECK: Logging statment in destructor could be dangerous?
  LOG(swatch::logger::kInfo) << mSystem.getPath() << " : Finished action '" << mAction.getId() << "'";

  if ( mSystem.mStatus.isRunning() && (&mAction == mSystem.mStatus.getLastRunningAction()) )
  {
    mSystem.mStatus.mRunningActions.pop_back();
    
    // In case this was a transition, also update object's current state
    if (const SystemTransition* t = dynamic_cast<const SystemTransition*>(&mAction))
    {
      if(t->getStatus().getState() == ActionStatus::kError)
        mSystem.mStatus.mState = t->getStateMachine().getErrorState();
      else
        mSystem.mStatus.mState = t->getEndState();
    }
  }
  else
  {
    size_t lNrActions = mSystem.mStatus.mRunningActions.size();
    const std::string activeFuncId(lNrActions > 0 ? "NULL" : "'" + mSystem.mStatus.getLastRunningAction()->getId() + "' (innermost of "+boost::lexical_cast<std::string>(lNrActions)+")");
    LOG(swatch::logger::kError) << "unexpected active functionoid " << activeFuncId << "  in BusyGuard destructor for system '" << mSystem.getPath() << "', functionoid '" << mAction.getId() << "'";
  }
}

}
}
