
#include "swatch/core/StateMachine.hpp"


#include "swatch/logger/Log.hpp"
#include "swatch/core/ActionableObject.hpp"
#include "swatch/core/Command.hpp"
#include "swatch/core/CommandSequence.hpp"
#include "swatch/core/GateKeeper.hpp"
#include "swatch/core/ReadOnlyXParameterSet.hpp"
#include "swatch/core/ThreadPool.hpp"


namespace swatch {
namespace core {
    
StateMachine::StateMachine(const std::string& aId, ActionableObject& aResource, const std::string& aInitialState, const std::string& aErrorState) :
  Object(aId),
  mResource(aResource),
  mInitialState(aInitialState),
  mErrorState(aErrorState)      
{
  addState(mInitialState);
  addState(mErrorState);
  
  aResource.addObj(this);
}


StateMachine::~StateMachine()
{
  for(std::map<std::string, tTransitionMap>::iterator lIt1=mTransitionMap.begin(); lIt1!=mTransitionMap.end(); lIt1++)
  {
    for(tTransitionMap::iterator lIt2=lIt1->second.begin(); lIt2!=lIt1->second.begin(); lIt2++)
      delete lIt2->second;
    lIt1->second.clear();
  }
}


const ActionableObject& StateMachine::getResource() const
{
  return mResource;
}


ActionableObject& StateMachine::getResource()
{
  return mResource;
}


const std::string& StateMachine::getInitialState() const
{
  return mInitialState;
}


const std::string& StateMachine::getErrorState() const
{
  return mErrorState;
}


const std::vector<std::string>& StateMachine::getStates() const
{
  return mStates;
}


const std::map<std::string, StateMachine::Transition*>& StateMachine::getTransitions(const std::string& aStateId) const
{
  if ( mTransitionMap.count(aStateId) == 0 )
    throw StateNotDefined("State '"+aStateId+"' does not exist");
  else
  {
    return mTransitionMap.find(aStateId)->second;
  }
}


StateMachine::Transition& StateMachine::getTransition(const std::string& aStateId, const std::string& aTransition)
{
  const std::map<std::string, StateMachine::Transition*>& lTransitions = getTransitions(aStateId);
  
  if ( lTransitions.count(aTransition) == 0 )
    throw TransitionNotDefined("Transition '"+aTransition+"' from state '"+aStateId+"' does not exist");
  else
  {
    return *(lTransitions.find(aTransition)->second);
  }
}


void StateMachine::addState(const std::string& aStateId)
{
  if ( mTransitionMap.count(aStateId) )
    throw StateAlreadyDefined("State '"+aStateId+"' has already been defined");
  else
  {
    mStates.push_back(aStateId);
    mTransitionMap[aStateId] = std::map<std::string, Transition*>();
  }
}


StateMachine::Transition& StateMachine::addTransition(const std::string& aTransitionId, const std::string& aFromState, const std::string& aToState)
{
  if ( mTransitionMap.count(aFromState) == 0 )
    throw StateNotDefined("State '"+aFromState+"' does not exist in state machine '"+getPath()+"'");
  else if (mTransitionMap.count(aToState) == 0 )
    throw StateNotDefined("State '"+aToState+"' does not exist in state machine '"+getPath()+"'");
  else if (mTransitionMap[aFromState].count(aTransitionId))
    throw TransitionAlreadyDefined("Transition '"+aTransitionId+"' from state '"+aFromState+"' already defined in state machine '"+getPath()+"'");
 
  std::map<std::string, Transition*>::iterator lIt = mTransitionMap[aFromState].insert( std::make_pair(aTransitionId, new Transition(aTransitionId, *this, aFromState, aToState) ) ).first;
  return *(lIt->second);
}


void StateMachine::disengage()
{
  boost::lock_guard<boost::mutex> lGuard(mResource.mMutex);
  
  // Throw if currently in other state machine
  if(mResource.mState.mEngagedFSM != this)
  {
    std::ostringstream oss;
    oss << "Cannot reset resource '" << mResource.getPath() << "' state machine '" << getId() << "'; ";
    if ( mResource.mState.mEngagedFSM != NULL)
      oss << "currently in state machine '" << mResource.mState.mEngagedFSM->getPath() << "'";
    else
      oss << "NOT in any state machine";
    throw ResourceInWrongStateMachine(oss.str());
  }

  // Throw if running action
  if ( ! mResource.mState.mActions.empty() )
    throw ActionableObjectIsBusy("Cannot reset '"+mResource.getPath()+"', state machine '"+getId()+"'; busy running action '"+mResource.mState.mActions.back()->getPath()+"'");  
  
  
  mResource.mState.mEngagedFSM = NULL;
  mResource.mState.mState = "";
}


void StateMachine::reset()
{
  boost::lock_guard<boost::mutex> lGuard(mResource.mMutex);
  
  // Throw if currently in other state machine
  if(mResource.mState.mEngagedFSM != this)
  {
    std::ostringstream oss;
    oss << "Cannot reset '" << mResource.getPath() << "', state machine '" << getId() << "'; ";
    if ( mResource.mState.mEngagedFSM != NULL)
      oss << "currently in state machine '" << mResource.mState.mEngagedFSM->getPath() << "'";
    else
      oss << "NOT in any state machine";
    throw ResourceInWrongStateMachine(oss.str());
  }
  
  // Throw if running action
  if ( ! mResource.mState.mActions.empty() )
    throw ActionableObjectIsBusy("Cannot reset '"+mResource.getPath()+"', state machine '"+getId()+"'; busy running action '"+mResource.mState.mActions.back()->getPath()+"'");  
  
  mResource.mState.mState = getInitialState();
}


StateMachine::Transition::Transition(const std::string& aId, StateMachine& aOp, const std::string& aStartState, const std::string& aEndState) :
  CommandVec(aId, aOp.getResource()),
  mStateMachine(aOp),
  mStartState(aStartState),
  mEndState(aEndState)
{
}


const std::string& StateMachine::Transition::getStartState() const
{
  return mStartState;
}


const std::string& StateMachine::Transition::getEndState() const
{
  return mEndState;
}


const StateMachine& StateMachine::Transition::getStateMachine() const
{
  return mStateMachine;
}


StateMachine& StateMachine::Transition::getStateMachine()
{
  return mStateMachine;
}


StateMachine::Transition& StateMachine::Transition::add(Command& aCmd, const std::string& aNamespace)
{
  addCommand(aCmd, aNamespace);
  return *this;
}


StateMachine::Transition& StateMachine::Transition::add(CommandSequence& aSequence)
{
  CommandVec::const_iterator lIt = aSequence.begin();
  std::vector<Command*> lCmds = aSequence.getCommands();
  for(auto lIt2=lCmds.begin(); lIt2!=lCmds.end(); lIt++, lIt2++) {
    addCommand(*(*lIt2), lIt->getNamespace());
  }
  return *this;
}


}  //ns: core
}  //ns: swatch
