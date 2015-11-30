
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
    
//------------------------------------------------------------------------------------
StateMachine::StateMachine(const std::string& aId, ActionableObject& aResource, MutableActionableStatus& aStatus, const std::string& aInitialState, const std::string& aErrorState) :
  Object(aId),
  mResource(aResource),
  mStatus(aStatus),
  mInitialState(aInitialState),
  mErrorState(aErrorState)      
{
  addState(mInitialState);
  addState(mErrorState);

  aResource.addObj(this);
}


//------------------------------------------------------------------------------------
StateMachine::~StateMachine()
{
}


//------------------------------------------------------------------------------------
const ActionableObject& StateMachine::getActionable() const
{
  return mResource;
}


//------------------------------------------------------------------------------------
ActionableObject& StateMachine::getActionable()
{
  return mResource;
}


//------------------------------------------------------------------------------------
const std::string& StateMachine::getInitialState() const
{
  return mInitialState;
}


//------------------------------------------------------------------------------------
const std::string& StateMachine::getErrorState() const
{
  return mErrorState;
}


//------------------------------------------------------------------------------------
const std::vector<std::string>& StateMachine::getStates() const
{
  return mStates;
}


//------------------------------------------------------------------------------------
const std::map<std::string, StateMachine::Transition*>& StateMachine::getTransitions(const std::string& aStateId) const
{
  return getState(aStateId).transitionMap;
}


//------------------------------------------------------------------------------------
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


//------------------------------------------------------------------------------------
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


//------------------------------------------------------------------------------------
void StateMachine::engage()
{
  ActionableStatusGuard lGuard(mStatus);
  
  // Throw if currently in other state machine
  if (mStatus.getStateMachineId(lGuard) != ActionableStatus::kNullStateMachineId )
    throw ResourceInWrongStateMachine("Cannot engage other state machine; resource '"+getPath()+"' currently in state machine '"+mStatus.getStateMachineId(lGuard)+"'");

  mStatus.setStateMachine(getId(), getInitialState(), lGuard);
}


//------------------------------------------------------------------------------------
void StateMachine::disengage()
{
  ActionableStatusGuard lGuard(mStatus);

  // Throw if currently in other state machine
  if(mStatus.getStateMachineId(lGuard) != this->getId())
  {
    std::ostringstream oss;
    oss << "Cannot reset resource '" << mResource.getPath() << "' state machine '" << getId() << "'; ";
    if ( mStatus.isEngaged(lGuard) )
      oss << "currently in state machine '" << mStatus.getStateMachineId(lGuard) << "'";
    else
      oss << "NOT in any state machine";
    throw ResourceInWrongStateMachine(oss.str());
  }

  // Throw if running action
  if ( mStatus.isBusy(lGuard) )
    throw ActionableObjectIsBusy("Cannot reset '"+mResource.getPath()+"', state machine '"+getId()+"'; busy running action '"+mStatus.getLastRunningAction(lGuard)->getPath()+"'");  
  
  
  mStatus.setNoStateMachine(lGuard);
}


//------------------------------------------------------------------------------------
void StateMachine::reset()
{
  ActionableStatusGuard lGuard(mStatus);
  
  // Throw if currently in other state machine
  if ( mStatus.getStateMachineId(lGuard) != this->getId() )
  {
    std::ostringstream oss;
    oss << "Cannot reset '" << mResource.getPath() << "', state machine '" << getId() << "'; ";
    if ( mStatus.isEngaged(lGuard) )
      oss << "currently in state machine '" << mStatus.getStateMachineId(lGuard) << "'";
    else
      oss << "NOT in any state machine";
    throw ResourceInWrongStateMachine(oss.str());
  }
  
  // Throw if running action
  if ( mStatus.isBusy(lGuard) )
    throw ActionableObjectIsBusy("Cannot reset '"+mResource.getPath()+"', state machine '"+getId()+"'; busy running action '"+mStatus.getLastRunningAction(lGuard)->getPath()+"'");  
  
  mStatus.setState(getInitialState(), lGuard);
}


StateMachine::Transition::Transition(const std::string& aId, StateMachine& aOp, const std::string& aStartState, const std::string& aEndState) :
  CommandVec(aId, aOp.getActionable()),
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


//------------------------------------------------------------------------------------
StateMachine::Transition& StateMachine::Transition::add(Command& aCmd, const std::string& aNamespace)
{
  addCommand(aCmd, aNamespace);
  return *this;
}


//------------------------------------------------------------------------------------
StateMachine::Transition& StateMachine::Transition::add(CommandSequence& aSequence)
{
  CommandVec::const_iterator lIt = aSequence.begin();
  std::vector<Command*> lCmds = aSequence.getCommands();
  for(auto lIt2=lCmds.begin(); lIt2!=lCmds.end(); lIt++, lIt2++) {
    addCommand(*(*lIt2), lIt->getNamespace());
  }
  return *this;
}


//------------------------------------------------------------------------------------
void StateMachine::Transition::extractMonitoringSettings(const GateKeeper& aGateKeeper,
	tMonitoringSettings& aMonSettings) const {
  aMonSettings.clear();

  // get a list of all MonitorableObjects
  ActionableObject& lResource = mStateMachine.getActionable();
  std::vector<std::string> lDescendants = lResource.getDescendants();
  for (std::vector<std::string>::const_iterator lIt = lDescendants.begin(); lIt != lDescendants.end(); lIt++) {
    MonitorableObject* lDescendant = lResource.getObj<MonitorableObject>(*lIt);

    if (lDescendant) {
      // query the GateKeeper for relevant settings for each object
      std::string lPath = *lIt;
      const std::vector<std::string>& lTablesToLookIn = lResource.getGateKeeperTables();
      const GateKeeper::tMonitoringSetting lMonSetting = aGateKeeper.getMonitoringSetting(mEndState, lPath,
          lTablesToLookIn);
      if (lMonSetting)
        aMonSettings.push_back(*lMonSetting);

      // get settings for child metrics
      const std::vector<std::string> lMetricIds = lDescendant->getMetrics();
      for (auto lMetricIt = lMetricIds.begin(); lMetricIt != lMetricIds.end(); lMetricIt++) {
        const std::string lMetricPath = lPath + "." + *lMetricIt;
        const GateKeeper::tMonitoringSetting lMetricSetting = aGateKeeper.getMonitoringSetting(mEndState, lMetricPath,
            lTablesToLookIn);
        if (lMetricSetting)
          aMonSettings.push_back(*lMetricSetting);
      }
    }
  }
}


//------------------------------------------------------------------------------------
void StateMachine::Transition::prepareCommands(const tReadOnlyXParameterSets& aParameters, const tMonitoringSettings& aMonSettings) {
}


//------------------------------------------------------------------------------------
void StateMachine::Transition::finaliseCommands(const tReadOnlyXParameterSets& aParameters, const tMonitoringSettings& aMonSettings) {
  applyMonitoringSettings(aMonSettings);
}


//------------------------------------------------------------------------------------
void StateMachine::Transition::applyMonitoringSettings(const tMonitoringSettings& aMonSettings) {
  
  // get a list of all MonitorableObjects
  ActionableObject& lResource = mStateMachine.getActionable();
  std::vector<std::string> lDescendants = lResource.getDescendants();
  
  for (std::vector<std::string>::const_iterator lIt = lDescendants.begin(); lIt != lDescendants.end(); lIt++) {
    MonitorableObject* lDescendant = lResource.getObj<MonitorableObject>(*lIt);
    
    if (lDescendant) {
      // query the GateKeeper for relevant settings for each object
      std::string lPath = *lIt;
      
      for (tMonitoringSettings::const_iterator lMonSetting = aMonSettings.begin(); lMonSetting != aMonSettings.end();
          ++lMonSetting) {
        LOG(logger::kInfo) << lPath << " " << lMonSetting->getId();
        
        if (lMonSetting->getId() == lPath) {
          lDescendant->setMonitoringStatus(lMonSetting->getStatus());
        }
        
        // Apply settings to child metrics
        const std::vector<std::string> lMetricIds = lDescendant->getMetrics();
        
        for (auto lMetricIt = lMetricIds.begin(); lMetricIt != lMetricIds.end(); lMetricIt++) {
        
          if (lMonSetting->getId() == (lPath + "." + *lMetricIt))
            lDescendant->getMetric(*lMetricIt).setMonitoringStatus(lMonSetting->getStatus());
        }
      }
    }
  }
}


//------------------------------------------------------------------------------------
StateMachine::State::State(const std::string& aId) :
  Object(aId)
{
}


//------------------------------------------------------------------------------------
void StateMachine::State::addTransition(Transition* aTransition)
{
  addObj(aTransition);
  transitionMap[ aTransition->getId() ] = aTransition;
}


//------------------------------------------------------------------------------------
const StateMachine::State& StateMachine::getState(const std::string& aStateId) const
{
  std::map<std::string, State*>::const_iterator lIt = mStateMap.find(aStateId);
  if(lIt != mStateMap.end())
    return *lIt->second;
  else
    throw StateNotDefined("State '" + aStateId + "' does not exist in state machine '"+getPath()+"''");
}


//------------------------------------------------------------------------------------
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
