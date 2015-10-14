
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
  return getState(aStateId).transitionMap;
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
  if ( mStateMap.count(aStateId) )
    throw StateAlreadyDefined("State '"+aStateId+"' has already been defined");
  else
  {
    State* lState = new State(aStateId);
    addObj(lState);
    mStateMap[aStateId] = lState;
    mStates.push_back(aStateId);
  }
}


StateMachine::Transition& StateMachine::addTransition(const std::string& aTransitionId, const std::string& aFromState, const std::string& aToState)
{
  State& lFromState = getState(aFromState);

  if (mStateMap.count(aToState) == 0 )
    throw StateNotDefined("State '"+aToState+"' does not exist in state machine '"+getPath()+"'");
  else if (lFromState.transitionMap.count(aTransitionId))
    throw TransitionAlreadyDefined("Transition '"+aTransitionId+"' from state '"+aFromState+"' already defined in state machine '"+getPath()+"'");
 
  Transition* t = new Transition(aTransitionId, *this, aFromState, aToState);
  lFromState.addTransition(t);
  return *t;
}


void StateMachine::disengage()
{
  boost::lock_guard<boost::mutex> lGuard(mResource.mMutex);
  
  // Throw if currently in other state machine
  if(mResource.mState.mFSM != this)
  {
    std::ostringstream oss;
    oss << "Cannot reset resource '" << mResource.getPath() << "' state machine '" << getId() << "'; ";
    if ( mResource.mState.mFSM != NULL)
      oss << "currently in state machine '" << mResource.mState.mFSM->getPath() << "'";
    else
      oss << "NOT in any state machine";
    throw ResourceInWrongStateMachine(oss.str());
  }

  // Throw if running action
  if ( ! mResource.mState.mActions.empty() )
    throw ActionableObjectIsBusy("Cannot reset '"+mResource.getPath()+"', state machine '"+getId()+"'; busy running action '"+mResource.mState.mActions.back()->getPath()+"'");  
  
  
  mResource.mState.mFSM = NULL;
  mResource.mState.mState = "";
}


void StateMachine::reset()
{
  boost::lock_guard<boost::mutex> lGuard(mResource.mMutex);
  
  // Throw if currently in other state machine
  if(mResource.mState.mFSM != this)
  {
    std::ostringstream oss;
    oss << "Cannot reset '" << mResource.getPath() << "', state machine '" << getId() << "'; ";
    if ( mResource.mState.mFSM != NULL)
      oss << "currently in state machine '" << mResource.mState.mFSM->getPath() << "'";
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


StateMachine::State::State(const std::string& aId) :
  Object(aId)
{
}


void StateMachine::State::addTransition(Transition* aTransition)
{
  addObj(aTransition);
  transitionMap[ aTransition->getId() ] = aTransition;
}

const StateMachine::State& StateMachine::getState(const std::string& aStateId) const
{
  std::map<std::string, State*>::const_iterator lIt = mStateMap.find(aStateId);
  if(lIt != mStateMap.end())
    return *lIt->second;
  else
    throw StateNotDefined("State '" + aStateId + "' does not exist in state machine '"+getPath()+"''");
}


StateMachine::State& StateMachine::getState(const std::string& aStateId)
{
  std::map<std::string, State*>::const_iterator lIt = mStateMap.find(aStateId);
  if(lIt != mStateMap.end())
    return *lIt->second;
  else
    throw StateNotDefined("State '" + aStateId + "' does not exist in system FSM '"+getPath()+"''");
}


}  //ns: core
}  //ns: swatch
