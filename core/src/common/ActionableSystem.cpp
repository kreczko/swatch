
#include "swatch/core/ActionableSystem.hpp"


#include "boost/iterator/indirect_iterator.hpp"
#include "boost/thread/lock_options.hpp"

#include "swatch/core/SystemStateMachine.hpp"
#include "swatch/logger/Log.hpp"


namespace swatch {
namespace core {


ActionableSystem::State::State() :
  AbstractState(),
  mFSM(NULL)
{
}


const SystemStateMachine* ActionableSystem::State::getEngagedFSM() const
{
  return mFSM;
}


  
ActionableSystem::ActionableSystem(const std::string& aId) : 
  MonitorableObject(aId)
{
}


ActionableSystem::~ActionableSystem()
{
}

std::set<std::string> ActionableSystem::getStateMachines() const
{
  std::set<std::string> lNames;
  for(tStateMachineMap::const_iterator lIt=mFSMs.begin(); lIt!=mFSMs.end(); lIt++)
    lNames.insert( lIt->first );
  return lNames;
}


SystemStateMachine& ActionableSystem::getStateMachine( const std::string& aId )
{
  try {
    return *(mFSMs.at( aId ));
  } catch ( const std::out_of_range& e ) {
    throw StateMachineNotFoundInActionableObject( "Unable to find state machine with ID '" + aId + "' in object '" + getPath() + "'");
  }
}


ActionableSystem::State ActionableSystem::getState() const
{
  boost::lock_guard<boost::mutex> lGuard(mMutex);
  return mState;
}


void ActionableSystem::engageStateMachine(const std::string& aFSM)
{
  const SystemStateMachine& lOp = getStateMachine(aFSM);

  tLockGuardMap lLockGuardMap = lockMutexes(lOp);

  // Throw if system or any of the participating children are already in a state machine
  if(mState.mFSM != NULL)
    throw ResourceInWrongStateMachine("Cannot engage other state machine; system '"+getPath()+"' currently in state machine"+mState.mFSM->getId()+"'");
  for(std::set<const StateMachine*>::const_iterator lIt=lOp.getParticipants().begin(); lIt!=lOp.getParticipants().end(); lIt++)
  {
    const ActionableObject& lChild = (*lIt)->getResource();
    if( lChild.mState.mFSM != NULL )
      throw ResourceInWrongStateMachine("Cannot engage other state machine; resource '"+lChild.getPath()+"' currently in state machine '"+lChild.mState.mFSM->getId()+"'");
  }
  
  // ... otherwise all is good, engage system & all participating children in appropriate state machine
  mState.mFSM = & lOp;
  mState.mState = lOp.getInitialState();
  for(std::set<const StateMachine*>::const_iterator lIt=lOp.getParticipants().begin(); lIt!=lOp.getParticipants().end(); lIt++)
  {
    ActionableObject& lObj = (*lIt)->mResource;
    lObj.mState.mFSM = *lIt;
    lObj.mState.mState = (*lIt)->getInitialState();
  }
}


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
    } while ( ! lActionableSys->getState().getActions().empty() );

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
  mState.mEnabled = false;
}

ActionableSystem::tLockGuardMap ActionableSystem::lockMutexes(const SystemStateMachine& aOp)
{
  // Lock the mutexes ...
  std::vector<boost::mutex*> lMutexes;
  lMutexes.push_back( &aOp.getResource().mMutex );
  for(std::set<const StateMachine*>::const_iterator lIt=aOp.getParticipants().begin(); lIt!=aOp.getParticipants().end(); lIt++)
    lMutexes.push_back( &(*lIt)->mResource.mMutex );

  boost::indirect_iterator<std::vector<boost::mutex*>::iterator> begin(lMutexes.begin()), end(lMutexes.end());
  boost::lock(begin, end);

  // ... then put them into lock guards
  std::map<const MonitorableObject*, tLockGuardPtr> lLockGuardMap;
  lLockGuardMap[&aOp.getResource()] = tLockGuardPtr(new boost::unique_lock<boost::mutex>(aOp.getResource().mMutex, boost::adopt_lock_t()));
  for(std::set<const StateMachine*>::const_iterator lIt=aOp.getParticipants().begin(); lIt!=aOp.getParticipants().end(); lIt++)
    lLockGuardMap[&(*lIt)->getResource()] = tLockGuardPtr( new boost::unique_lock<boost::mutex>((*lIt)->getResource().mMutex, boost::adopt_lock_t()) );
  
  return lLockGuardMap;
}


ActionableSystem::BusyGuard::BusyGuard(ActionableSystem& aResource, const Functionoid& aAction) : 
  mResource(aResource),
  mAction(aAction)
{
  const SystemTransition& lTransition = dynamic_cast<const SystemTransition&>(aAction);
  const SystemStateMachine& lOp = lTransition.getStateMachine();
  
  // Put the mutexes into lock guards before continuing
  std::map<const MonitorableObject*, tLockGuardPtr> lLockGuardMap = ActionableSystem::lockMutexes(lOp);

  // 1a) Check that system is engaged in correct state machine, and is in the correct state
  if( aResource.mState.mFSM != & lTransition.getStateMachine())
    throw ResourceInWrongStateMachine("Resource '"+aResource.getPath()+"' is not yet engaged in state machine '"+lTransition.getStateMachine().getId()+"'");
  else if ( aResource.mState.mState != lTransition.getStartState() )
    throw ResourceInWrongState("Resource '"+aResource.getPath()+"' is in state '"+aResource.mState.mState+"'; transition '"+lTransition.getId()+"' cannot be run");
  
  // 1b) Check that children are engaged in correct state machine, and are in the correct state
  typedef std::map<ActionableObject*, const StateMachine::Transition* > tObjTransitionMap;
  tObjTransitionMap childTransitionMap;
  for(SystemTransition::const_iterator lIt=(lTransition.end()-1); lIt != (lTransition.begin()-1); lIt--)
  {
    for(std::vector<StateMachine::Transition*>::const_iterator lIt2=lIt->cget().begin(); lIt2!=lIt->cget().end(); lIt2++)
      childTransitionMap[ &(*lIt2)->getStateMachine().getResource() ] = *lIt2;
  }

  for(tObjTransitionMap::const_iterator lIt=childTransitionMap.begin(); lIt!=childTransitionMap.end(); lIt++)
  {
    if( lIt->first->mState.mFSM != & lIt->second->getStateMachine() )
      throw ResourceInWrongStateMachine("Resource '"+lIt->first->getPath()+"' is not yet engaged in state machine '"+lIt->second->getStateMachine().getId()+"'");
    else if ( lIt->first->mState.mState != lIt->second->getStartState() )
      throw ResourceInWrongState("Resource '"+lIt->first->getPath()+"' is in state "+lIt->first->mState.mState+", transition '"+lIt->second->getId()+"' cannot be run");
  }
  
  
  // 2a) Check that the system is not busy
  if ( (aResource.mState.mEnabled == false) || ( ! aResource.mState.mActions.empty() ) )
  {
    std::ostringstream oss;
    oss << "Could not run action '" << lTransition.getId() << "' on resource '" << aResource.getPath() << "'. ";

    if ( ! aResource.mState.mActions.empty() )
      oss << "Resource currently busy running functionoid '" << aResource.mState.mActions.back()->getId() << "'.";
    else
      oss << "Actions currently disabled on this resource.";

    LOG(swatch::logger::kNotice) << oss.str();
    throw ActionableObjectIsBusy(oss.str());
  }

  // 2b) Check that none of the children are busy
  for(tObjTransitionMap::const_iterator lIt=childTransitionMap.begin(); lIt!=childTransitionMap.end(); lIt++)
  {
    const ActionableObject& lChild = *lIt->first;
    
    if ( (lChild.mState.mEnabled == false) || (! lChild.mState.mActions.empty()) )
    {
      std::ostringstream oss;
      oss << "Could not run system action '" << lTransition.getId() << "' due to child resource '" << aResource.getPath() << "'. ";

      if ( ! lChild.mState.mActions.empty() )
        oss << "Child currently busy running functionoid '" << lChild.mState.mActions.back()->getId() << "'.";
      else
        oss << "Actions currently disabled on this child.";

      LOG(swatch::logger::kNotice) << oss.str();
      throw ActionableObjectIsBusy(oss.str());
    }
  } 
  
  // 3) If got this far, then all is good; create the busy guards for the children
  LOG(swatch::logger::kInfo) << mResource.getPath() << " : Starting action '" << mAction.getId() << "'";
  aResource.mState.mActions.push_back( &aAction );
  for(tObjTransitionMap::const_iterator lIt=childTransitionMap.begin(); lIt!=childTransitionMap.end(); lIt++)
    this->mChildGuardMap[ lIt->first ] = tChildGuardPtr(new ActionableObject::BusyGuard(*lIt->first, *lLockGuardMap[lIt->first].get(), lTransition) );
}


const ActionableObject::BusyGuard& ActionableSystem::BusyGuard::getChildGuard(const ActionableObject& aChild) const
{
  std::map<const ActionableObject*, tChildGuardPtr>::const_iterator lIt = mChildGuardMap.find(&aChild);
  if (lIt == mChildGuardMap.end())
    throw std::runtime_error("Non-child object '"+aChild.getPath()+"' passed to BusyGuard for system '"+mResource.getPath()+"', action '"+mAction.getId()+"'");
  return *(lIt->second);
}


ActionableSystem::BusyGuard::~BusyGuard()
{
  boost::lock_guard<boost::mutex> lGuard(mResource.mMutex);
  LOG(swatch::logger::kInfo) << mResource.getPath() << " : Finished action '" << mAction.getId() << "'";
  if ((!mResource.mState.mActions.empty()) && (&mAction == mResource.mState.mActions.back()))
  {
    mResource.mState.mActions.pop_back();
    
    // In case this was a transition, also update object's current state
    if (const SystemTransition* t = dynamic_cast<const SystemTransition*>(&mAction))
    {
      if(t->getStatus().getState() == ActionStatus::kError)
        mResource.mState.mState = t->getStateMachine().getErrorState();
      else
        mResource.mState.mState = t->getEndState();
    }
  }
  else
  {
    size_t lNrActions = mResource.mState.mActions.size();
    const std::string activeFuncId(lNrActions > 0 ? "NULL" : "'" + mResource.mState.mActions.back()->getId() + "' (innermost of "+boost::lexical_cast<std::string>(lNrActions)+")");
    LOG(swatch::logger::kError) << "unexpected active functionoid " << activeFuncId << "  in BusyGuard destructor for system '" << mResource.getPath() << "', functionoid '" << mAction.getId() << "'";
  }
}

}
}
