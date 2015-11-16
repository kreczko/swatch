
#include "swatch/core/SystemStateMachine.hpp"


#include "swatch/core/ActionableSystem.hpp"
#include "swatch/core/ThreadPool.hpp"
#include "boost/thread/pthread/thread_data.hpp"


namespace swatch {
namespace core {
    

SystemTransition::Step::Step(const std::vector<StateMachine::Transition*>& aTransitions) :
  mTransitions(aTransitions)
{
}
    
const std::vector<StateMachine::Transition*>& SystemTransition::Step::get()
{
  return mTransitions;
}


const std::vector<StateMachine::Transition*>& SystemTransition::Step::cget() const
{
  return mTransitions;
}

//------------------------------------------------------------------------------------


SystemTransition::SystemTransition(const std::string& aId, SystemStateMachine& aOp, const std::string& aStartState, const std::string& aEndState) :
  Functionoid(aId),
  mFSM(aOp),
  mStartState(aStartState),
  mEndState(aEndState),
  mGateKeeper(NULL),
  mState(ActionStatus::kInitial),
  mStepIt(mSteps.end())
{
}


SystemTransition::~SystemTransition()
{
}


SystemTransition::iterator SystemTransition::begin()
{
  return mSteps.begin();
}


SystemTransition::iterator SystemTransition::end()
{
  return mSteps.end();
}


SystemTransition::const_iterator SystemTransition::begin() const
{
  return mSteps.begin();
}


SystemTransition::const_iterator SystemTransition::end() const
{
  return mSteps.end();
}


size_t SystemTransition::size() const
{
  return mSteps.size();
}


SystemTransitionStatus SystemTransition::getStatus() const
{
  boost::unique_lock<boost::mutex> lock(mMutex);
  
  float runningTime = 0.0;
  switch (mState) {
    case ActionStatus::kInitial : 
    case ActionStatus::kScheduled :
      break;
    default :
      boost::posix_time::ptime endTime;
      if (mState == ActionStatus::kRunning)
        endTime = boost::posix_time::microsec_clock::universal_time();
      else
        endTime = mExecEndTime;
      
      boost::posix_time::time_duration duration = endTime - mExecStartTime;
      runningTime = duration.total_seconds();
      runningTime += double(duration.fractional_seconds()) / pow(10.0, duration.num_fractional_digits());

      break;
  }
  
  const Step* currentStep =  ( ((mStepIt == mSteps.end()) || (mState == kError)) ? NULL : &*mStepIt);
  
  return SystemTransitionStatus(getPath(), mState, runningTime, currentStep, mStatusOfCompletedSteps, mSteps.size());
}



const std::string& SystemTransition::getStartState() const
{
  return mStartState;
}


const std::string& SystemTransition::getEndState() const 
{
  return mEndState;
}


const SystemStateMachine& SystemTransition::getStateMachine() const
{
  return mFSM;
}


SystemStateMachine& SystemTransition::getStateMachine()
{
  return mFSM;
}


SystemTransition& SystemTransition::add(const std::vector<StateMachine::Transition*>& aTransitions)
{
  // 1) Consistency checks
  if (aTransitions.empty())
    throw InvalidSystemTransition("Cannot add empty step to system transition");

  for(std::vector<StateMachine::Transition*>::const_iterator lIt=aTransitions.begin(); lIt!=aTransitions.end(); lIt++)
  {
    if(*lIt == NULL)
      throw InvalidSystemTransition("Element "+boost::lexical_cast<std::string>(lIt-aTransitions.begin())+" of child transition vector is NULL");
    
    const ActionableObject& lObj = (*lIt)->getStateMachine().getResource();
    
    //Throw if any transitions in vector are not from children of this system
    bool isChild = false;
    std::vector<std::string> children = mFSM.getResource().getChildren();
    for(std::vector<std::string>::const_iterator lIt2=children.begin(); lIt2!=children.end(); lIt2++)
    {
      ActionableObject* lChild = mFSM.getResource().getObj<ActionableObject>(*lIt2);
      if( &lObj == lChild )
        isChild = true;
    }
    if(isChild == false)
      throw InvalidSystemTransition("Cannot add transition on non-child object '"+lObj.getPath()+"' to system state machine '"+mFSM.getPath());
    
    
    // Throw if any two transitions in vector are from same child
    for(std::vector<StateMachine::Transition*>::const_iterator lIt2=aTransitions.begin(); lIt2!=aTransitions.end(); lIt2++)
    {
      if ( (lIt != lIt2) && (&lObj == &(*lIt2)->getStateMachine().getResource()) )
        throw InvalidSystemTransition("Multiple transitions requested for same child object  '"+lObj.getPath()+"' in system-level transition");
    }
    
    // Throw if (for any child) transition is from different state machine than has been registered before
    bool newParticipant = true;
    for(std::set<const StateMachine*>::const_iterator lIt2 = mFSM.getParticipants().begin(); lIt2 != mFSM.getParticipants().end(); lIt2++)
    {
      if( (&(*lIt2)->getResource() == &lObj) )
      {
        if (*lIt2 == &(*lIt)->getStateMachine())
          newParticipant = false;
        else
          throw InvalidSystemTransition("Object '"+lObj.getPath()+"', transition '"+(*lIt)->getId()+"' is inconsistent with transitions already in system-level state machine '"+mFSM.getPath()+"'");
      }
    }
    
    // Throw if any child transition is inconsistent with previous steps from this transition (i.e. if it's start state != end state from transition in previous step)
    const StateMachine::Transition* lLastTransition = NULL;
    for(std::vector<Step>::const_iterator lStepIt=mSteps.begin(); lStepIt!=mSteps.end(); lStepIt++)
    {
      for(std::vector<StateMachine::Transition*>::const_iterator lIt2=lStepIt->cget().begin(); lIt2!=lStepIt->cget().end(); lIt2++)
      {
        if(& (*lIt2)->getResource() == & (*lIt)->getResource())
          lLastTransition = *lIt2;
      }
    }
    if( (lLastTransition != NULL) && (lLastTransition->getEndState() != (*lIt)->getStartState()))
      throw InvalidSystemTransition("Object '"+lObj.getPath()+ "', transition '"+(*lIt)->getId()+"' (start state: '"+(*lIt)->getStartState()+"') is incompatible with last transition in same step, '"+lLastTransition->getId()+"' (end state: '"+lLastTransition->getEndState()+"')");
  }

  for(std::vector<StateMachine::Transition*>::const_iterator lIt=aTransitions.begin(); lIt!=aTransitions.end(); lIt++)
  {
    mFSM.mChildFSMs.insert( &(*lIt)->getStateMachine() );
    mFSM.mNonConstChildFSMs.insert( &(*lIt)->getStateMachine() );
  }
  mSteps.push_back( Step(aTransitions) );
  
  return *this;
}


void SystemTransition::checkForMissingParameters(const GateKeeper& aGateKeeper, std::map<const StateMachine::Transition*, std::vector<CommandVec::MissingParam> >& aMissingParams) const
{
  aMissingParams.clear();
  
  for(std::vector<Step>::const_iterator lIt=mSteps.begin(); lIt!=mSteps.end(); lIt++)
  {
    for(std::vector<StateMachine::Transition*>::const_iterator lIt2=lIt->cget().begin(); lIt2!=lIt->cget().end(); lIt2++)
    {
      std::vector<ReadOnlyXParameterSet> lParamSets;
      std::vector<CommandVec::MissingParam> lMissingParams;
      (*lIt2)->checkForMissingParameters(aGateKeeper, lParamSets, lMissingParams);
      if( ! lMissingParams.empty() )
        aMissingParams[*lIt2] = lMissingParams;
    }
  }
}


void SystemTransition::exec(const GateKeeper& aGateKeeper, const bool& aUseThreadPool)
{
  // 0) Throw if any parameters are missing from gatekeeper
  std::map<const StateMachine::Transition*, std::vector<CommandVec::MissingParam> > lMissingParams;
  checkForMissingParameters(aGateKeeper, lMissingParams);
  if ( ! lMissingParams.empty() )
    throw ParameterNotFound("Could not find value of parameters for " + boost::lexical_cast<std::string>(lMissingParams.size()) + " transitions");

  // 1) Request control of the resource (incl. children)
  boost::shared_ptr<ActionableSystem::BusyGuard> lBusyGuard(new ActionableSystem::BusyGuard(mFSM.getResource(), *this));

  // 2) Reset the status of this transition's state variables
  {
    boost::unique_lock<boost::mutex> lock( mMutex );

//    // Extract parameters first, so that don't change state if a parameter is missing
//    std::vector<std::pair<std::string,std::string> > missingParams;
//    extractParameters(aGateKeeper, mCachedParameters, missingParams, true);

    mGateKeeper = & aGateKeeper;
    mState = ActionStatus::kInitial;
    mStepIt = mSteps.end();
    mStatusOfCompletedSteps.clear();
    mStatusOfCompletedSteps.reserve(mSteps.size());
  }  

  // 3) Execute the command
  if ( aUseThreadPool){
    boost::unique_lock<boost::mutex> lock(mMutex);
    mState = ActionStatus::kScheduled;
    
    ThreadPool& pool = ThreadPool::getInstance();
    pool.addTask<SystemTransition, ActionableSystem::BusyGuard>( this , &SystemTransition::runSteps, lBusyGuard);
  }
  else{
    // otherwise execute in same thread
    this->runSteps(lBusyGuard);
  }
}


void SystemTransition::runSteps(boost::shared_ptr<ActionableSystem::BusyGuard> aGuard)
{
  // 1) Declare that I'm running
   {
    boost::unique_lock<boost::mutex> lock( mMutex );
    mExecStartTime = boost::posix_time::microsec_clock::universal_time();
    // Finish straight away if there aren't any steps to run
    if( mSteps.empty() )
    {
      mState = ActionStatus::kDone;
      mStepIt = mSteps.end();
      mExecEndTime = mExecStartTime;
      return;
    }
    else
    {
      mState = ActionStatus::kRunning;
      mStepIt = mSteps.begin();
    }
  }

  // 2) Run the steps
  try {
    bool lWarningOccurred = false;

    while( true ) 
    {
      // 2.i) Start running the child transitions in this step
      for(auto lIt=mStepIt->get().begin(); lIt!=mStepIt->get().end(); lIt++)
      {
        StateMachine::Transition& lChildTransition = *(*lIt);
        ActionableObject& lChild = (*lIt)->getStateMachine().getResource();
        lChildTransition.exec(&aGuard->getChildGuard(lChild), *mGateKeeper); 
      }
      
      // 2.ii) Wait for them all to complete
      bool lErrorOccurred = false;
      for(auto lIt=mStepIt->get().begin(); lIt!=mStepIt->get().end(); lIt++)
      {
        while( (*lIt)->getStateMachine().getResource().getStatus().getRunningActions().back() != this )
        {
          boost::this_thread::sleep_for(boost::chrono::microseconds(100));
        }
        if ( (*lIt)->getState() == ActionStatus::kError )
          lErrorOccurred = true;
        else if ( (*lIt)->getState() == ActionStatus::kWarning )
          lWarningOccurred = true;
      }
      
      // 2.iii) Harvest the status objects (before locking mutex, to avoid deadlock scenarios)
      std::vector<boost::shared_ptr<const StateMachine::TransitionStatus> > lStatusVec;
      for(auto lIt=mStepIt->get().begin(); lIt!=mStepIt->get().end(); lIt++)
        lStatusVec.push_back( boost::shared_ptr<const StateMachine::TransitionStatus>(new StateMachine::TransitionStatus( (*lIt)->getStatus())) );
      
      // 2.iv) Lock the mutex, and update member data
      boost::unique_lock<boost::mutex> lock(mMutex);
      mStatusOfCompletedSteps.push_back( lStatusVec );

      // Don't execute any more steps if there was an error
      if( lErrorOccurred )
      {
        mState = ActionStatus::kError;
        mExecEndTime = boost::posix_time::microsec_clock::universal_time();
        return;
      }

      // Increment the "current step" iterator 
      ++mStepIt;

      // Exit the loop if no more steps remain
      if( mStepIt == mSteps.end() )
      {
        mState = ( lWarningOccurred ? ActionStatus::kWarning : ActionStatus::kDone );
        mExecEndTime = boost::posix_time::microsec_clock::universal_time();
        return;
      }
    }
    
  } catch (const std::exception& e) {
//     this->setError( "An exception occured in Command::code(): " + std::string(e.what()));
    std::cout << "An exception occurred in CommandSequence::runCommands(): " << e.what() << std::endl;
    
    boost::unique_lock<boost::mutex> lock( mMutex );
    mState = ActionStatus::kError;
    mExecEndTime = boost::posix_time::microsec_clock::universal_time();
  }
  
  // 3) The resource is released by destruction of BusyGuard
}


//------------------------------------------------------------------------------------


SystemStateMachine::SystemStateMachine(const std::string& aId, ActionableSystem& aResource, const std::string& aInitialState, const std::string& aErrorState) :
  Object(aId),
  mResource(aResource),
  mInitialState(aInitialState),
  mErrorState(aErrorState)      
{
  addState(mInitialState);
  addState(mErrorState);
  
  aResource.addObj(this);
}


SystemStateMachine::~SystemStateMachine()
{
}


const ActionableSystem& SystemStateMachine::getResource() const
{
  return mResource;
}


ActionableSystem& SystemStateMachine::getResource()
{
  return mResource;
}


const std::string& SystemStateMachine::getInitialState() const
{
  return mInitialState;
}


const std::string& SystemStateMachine::getErrorState() const
{
  return mErrorState;
}


const std::set<const StateMachine*>& SystemStateMachine::getParticipants() const
{
  return mChildFSMs;
}


const std::vector<std::string>& SystemStateMachine::getStates() const
{
  return mStates;
}


const std::map<std::string, SystemTransition*>& SystemStateMachine::getTransitions(const std::string& aStateId) const
{
  return getState(aStateId).transitionMap;
}


void SystemStateMachine::addState(const std::string& aStateId)
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


SystemTransition& SystemStateMachine::addTransition(const std::string& aTransitionId, const std::string& aFromState, const std::string& aToState)
{
  State& lFromState = getState(aFromState);
  
  if (mStateMap.count(aToState) == 0 )
    throw StateNotDefined("State '"+aToState+"' does not exist in state machine '"+getPath()+"'");
  else if (lFromState.transitionMap.count(aTransitionId))
    throw TransitionAlreadyDefined("Transition '"+aTransitionId+"' from state '"+aFromState+"' already defined in state machine '"+getPath()+"'");
 
  SystemTransition* t = new SystemTransition(aTransitionId, *this, aFromState, aToState);
  lFromState.addTransition(t);

  return *t;
}


void SystemStateMachine::reset()
{
  ActionableSystem::tLockGuardMap lLockGuardMap = ActionableSystem::lockMutexes(*this);

  // Throw if system/children are not in this state machine or running transition
  checkStateMachineEngagedAndNotInTransition("reset");

  getResource().mStatus.mState = getInitialState();
  
  for(std::set<StateMachine*>::const_iterator lIt=mNonConstChildFSMs.begin(); lIt!=mNonConstChildFSMs.end(); lIt++)
  {
    ActionableObject& lChild = (*lIt)->getResource();
    if( lChild.mStatus.mFSM == *lIt)
      lChild.mStatus.mState = (*lIt)->getInitialState();
  }  
}


void SystemStateMachine::disengage()
{
  ActionableSystem::tLockGuardMap lLockGuardMap = ActionableSystem::lockMutexes(*this);
  
  // Throw if system/children are not in this state machine or running transition
  checkStateMachineEngagedAndNotInTransition("disengage");


  getResource().mStatus.mFSM = NULL;
  getResource().mStatus.mState = ActionableStatus::kNullStateId;
  
  for(std::set<StateMachine*>::const_iterator lIt=mNonConstChildFSMs.begin(); lIt!=mNonConstChildFSMs.end(); lIt++)
  {
    ActionableObject& lChild = (*lIt)->getResource();
    if( lChild.mStatus.mFSM == *lIt)
    {
      lChild.mStatus.mFSM = NULL;
      lChild.mStatus.mState = ActionableStatus::kNullStateId;
    }
  }  
}


void SystemStateMachine::checkStateMachineEngagedAndNotInTransition(const std::string& aAction) const
{
    // Throw if system is not in this state machine
  if ( getResource().mStatus.mFSM != this )
  {
    std::ostringstream oss;
    oss << "Cannot " << aAction << " state machine '" << getId() << "' of '" << getResource().getPath() << "'; ";
    if ( getResource().mStatus.mFSM == NULL )
      oss << "NOT in any state machine.";
    else
      oss << "currently in state machine '" << getResource().mStatus.mFSM->getPath() << "'";
    throw ResourceInWrongStateMachine(oss.str());
  }
  
  // Throw if system or any of the participating children are currently running a transition
  if(const SystemTransition* t = getResource().mStatus.getFirstRunningAction<SystemTransition>())
    throw ActionableSystemIsBusy("Cannot "+aAction+" state machine '"+getId()+"'; resource '"+getResource().getPath()+"' is busy in transition '"+t->getId()+"'");
  
  // Throw if any children in wrong state machine, or 
  for(std::set<const StateMachine*>::const_iterator lIt=getParticipants().begin(); lIt!=getParticipants().end(); lIt++)
  {
    const ActionableObject& lChild = (*lIt)->getResource();
    if(lChild.mStatus.mFSM != *lIt)
    {
      std::ostringstream oss;
      oss << "Cannot " << aAction << " state machine '" << (*lIt)->getId() << "' of '" << (*lIt)->getResource().getPath() << "'; ";
      if ( lChild.mStatus.mFSM == NULL )
        oss << "NOT in any state machine.";
      else
        oss << "currently in state machine '" << lChild.mStatus.mFSM->getPath() << "'";
      throw ResourceInWrongStateMachine(oss.str());
    }
    else
    {
      if (const StateMachine::Transition* t = (*lIt)->getResource().mStatus.getFirstRunningAction<StateMachine::Transition>())
        throw ActionableObjectIsBusy("Cannot "+aAction+" state machine '"+getId()+"'; child resource '"+lChild.getPath()+"' is busy in transition '"+t->getId()+"'");
    }
  }
}


SystemStateMachine::State::State(const std::string& aId) : 
  Object(aId)
{
}


void SystemStateMachine::State::addTransition(SystemTransition* aTransition)
{
  addObj(aTransition);
  transitionMap[ aTransition->getId() ] = aTransition;
}


const SystemStateMachine::State& SystemStateMachine::getState(const std::string& aStateId) const
{
  std::map<std::string, State*>::const_iterator lIt = mStateMap.find(aStateId);
  if(lIt != mStateMap.end())
    return *lIt->second;
  else
    throw StateNotDefined("State '" + aStateId + "' does not exist in system FSM '"+getPath()+"''");
}


SystemStateMachine::State& SystemStateMachine::getState(const std::string& aStateId)
{
  std::map<std::string, State*>::const_iterator lIt = mStateMap.find(aStateId);
  if(lIt != mStateMap.end())
    return *lIt->second;
  else
    throw StateNotDefined("State '" + aStateId + "' does not exist in system FSM '"+getPath()+"''");
}

//------------------------------------------------------------------------------------


float SystemTransitionStatus::getProgress() const
{
  if(mTotalNumSteps == 0)
    return (getState() == ActionStatus::kDone) ? 1.0: 0.0;
  else if(mStepStatuses.empty())
    return 0.0;
  else if((getState() == kError) || (getState() == kRunning))
  {
    float baseProgress = float(mStepStatuses.size() - 1)/float(mTotalNumSteps);
    
    // Determine progress of slowest parallel transition in last step
    typedef std::vector<boost::shared_ptr<const StateMachine::TransitionStatus> > tStepStatus;
    const tStepStatus& lStatusLastStep = mStepStatuses.back();
    std::vector<float> lLastStepProgressVec;
    for(tStepStatus::const_iterator lIt=lStatusLastStep.begin(); lIt!=lStatusLastStep.end(); lIt++)
      lLastStepProgressVec.push_back((*lIt)->getProgress());
    float lastStepProgress = (*std::min_element(lLastStepProgressVec.begin(), lLastStepProgressVec.end()))/float(mTotalNumSteps);
    return baseProgress + lastStepProgress;
  }
  else
    return 1.0;
}


size_t SystemTransitionStatus::getNumberOfCompletedSteps() const
{
  return mNumCompletedSteps;
}


size_t SystemTransitionStatus::getTotalNumberOfSteps() const
{
  return mTotalNumSteps;
}


const std::vector< std::vector<boost::shared_ptr<const StateMachine::TransitionStatus> > >& SystemTransitionStatus::getStepStatus() const
{
  return mStepStatuses;
}


SystemTransitionStatus::SystemTransitionStatus(const std::string& aPath, ActionStatus::State aState, float aRunningTime, const SystemTransition::Step* aCurrentStep, const tStepStatusVec& aFinishedStepStatuses, size_t aTotalNumSteps) :
  ActionStatus(aPath, aState, aRunningTime),
  mTotalNumSteps( aTotalNumSteps ),
  mNumCompletedSteps( aFinishedStepStatuses.size() ),
  mStepStatuses( aFinishedStepStatuses )
{
  if (aCurrentStep != NULL)
  {
    mStepStatuses.push_back( std::vector< boost::shared_ptr<const StateMachine::TransitionStatus> > ());
    for(std::vector<StateMachine::Transition*>::const_iterator lIt=aCurrentStep->cget().begin(); lIt!=aCurrentStep->cget().end(); lIt++)
    {
      boost::shared_ptr<const StateMachine::TransitionStatus> childStatus(new StateMachine::TransitionStatus((*lIt)->getStatus())); 
      mStepStatuses.back().push_back( childStatus );
    }
  } 
}


}
}
