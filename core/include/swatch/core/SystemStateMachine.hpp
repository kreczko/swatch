/* 
 * File:   SystemStateMachine.hpp
 * Author: tom
 *
 * Created on 18 September 2015, 15:27
 */

#ifndef __SWATCH_CORE_SYSTEMSTATEMACHINE_HPP__
#define	__SWATCH_CORE_SYSTEMSTATEMACHINE_HPP__


#include "swatch/core/ActionableSystem.hpp"
#include "swatch/core/StateMachine.hpp"
#include "swatch/core/Object.hpp"


namespace swatch {
namespace core {

class SystemStateMachine;
class SystemTransitionStatus;



class SystemTransition : public Functionoid {
public:
  
  class Step {
  public:
    Step(const std::vector<StateMachine::Transition*>& aTransitions);
    
    const std::vector<StateMachine::Transition*>& get();

    //FIXME: Shouldn't give non-const pointers in const method!
    const std::vector<StateMachine::Transition*>& cget() const;
    
  private:
    std::vector<StateMachine::Transition*> mTransitions;
  };
    
  SystemTransition(const std::string& aId, SystemStateMachine& aFSM, const std::string& aStartState, const std::string& aEndState);
  virtual ~SystemTransition();

  // Iteration over all steps in the transition
  typedef std::vector<Step>::const_iterator const_iterator;
  typedef std::vector<Step>::iterator iterator;


  iterator begin();
  iterator end();
  const_iterator begin() const;
  const_iterator end() const;

  //! Number of steps
  size_t size() const; 

  SystemTransitionStatus getStatus() const;

  const std::string& getStartState() const;
  const std::string& getEndState() const;

  const SystemStateMachine& getStateMachine() const;

  SystemStateMachine& getStateMachine();

  template<class Iterator>
  SystemTransition& add(Iterator aBegin, Iterator aEnd, const std::string& aFromState, const std::string& aTransition);
  
  template<class Iterator>
  SystemTransition& add(Iterator aBegin, Iterator aEnd, const std::string& aStateMachine, const std::string& aFromState, const std::string& aTransition);

  template<class Collection>
  SystemTransition& add(const Collection& aCollection, const std::string& aFromState, const std::string& aTransition);
  
  template<class Collection>
  SystemTransition& add(const Collection& aCollection, const std::string& aStateMachine, const std::string& aFromState, const std::string& aTransition);

  SystemTransition& add(const std::vector<StateMachine::Transition*>& aTransitions);

  void checkForMissingParameters(const GateKeeper& aGateKeeper, std::map< const StateMachine::Transition*, std::vector<CommandVec::MissingParam> >& aMissingParams) const;

  /**
    * Run the transition, extracting the parameters for each child transition from the supplied gatekeeper
    * 
    * @param aGateKeeper Gatekeeper that will be used to extract the 
    * @param aUseThreadPool Run the transition asynchronously in the swatch::core::ThreadPool ; if equals false, then the sequence is run synchronously (i.e. in same thread)
    */
  void exec(const GateKeeper& aGateKeeper, const bool& aUseThreadPool = true ); 

private:
  void runSteps(boost::shared_ptr<ActionableSystem::BusyGuard> aGuard);

  SystemStateMachine& mFSM;
  const std::string mStartState;
  const std::string mEndState;
  std::vector<Step> mSteps;

  mutable boost::mutex mMutex;
  const GateKeeper* mGateKeeper;
  ActionStatus::State mState;
  std::vector<Step>::iterator mStepIt;
  boost::posix_time::ptime mExecStartTime;
  boost::posix_time::ptime mExecEndTime;
  std::vector< std::vector<boost::shared_ptr<const StateMachine::TransitionStatus> > > mStatusOfCompletedSteps;
};


class SystemTransitionStatus : public ActionStatus {
public:
  //! Returns fraction progress of transition - range [0,1] inclusive
  float getProgress() const;

  //! Returns count of steps that have already completed execution (regardless of whether their final state is kDone, kWarning, or kError)
  size_t getNumberOfCompletedSteps() const;

  //! Number of steps in the transition
  size_t getTotalNumberOfSteps() const;

  typedef boost::shared_ptr<const StateMachine::TransitionStatus> tChildStatusRef;
  typedef std::vector<tChildStatusRef> tStepStatus;
  typedef std::vector<tStepStatus> tStepStatusVec;
  typedef tStepStatusVec::const_iterator const_iterator;
  //! Returns status of steps that have started/completed execution 
  const tStepStatusVec& getStepStatus() const;

private:
  SystemTransitionStatus(const std::string& aPath, ActionStatus::State aState, float aRunningTime, const SystemTransition::Step* aCurrentStep, const tStepStatusVec& aFinishedStepStatuses, size_t aTotalNumSteps);

  size_t mTotalNumSteps;
  size_t mNumCompletedSteps;
  tStepStatusVec mStepStatuses;
  friend class SystemTransition;
};
  
  
class SystemStateMachine : public Object {
public:
  SystemStateMachine(const std::string& aId, ActionableSystem& aResource, const std::string& aInitialState, const std::string& aErrorState);
  virtual ~SystemStateMachine();

  const ActionableSystem& getResource() const;
  
  ActionableSystem& getResource();
  
  const std::string& getInitialState() const;
  
  const std::string& getErrorState() const;
  
  const std::set<const StateMachine*>& getParticipants() const;
  
  const std::vector<std::string>& getStates() const;
  
  const std::map<std::string, SystemTransition*>& getTransitions(const std::string& aStateId) const;
  
  void addState(const std::string& aState);
  
  SystemTransition& addTransition(const std::string& aTransitionId, const std::string& aFromState, const std::string& aToState);

  void disengage();
  
  void reset();
  
private:
  // Throws if system/children are in other state machine, or running transition; need to lock externally ...
  void checkStateMachineEngagedAndNotInTransition(const std::string& aAction) const;
  
  struct State : public Object {
    State(const std::string& aId);
    void addTransition(SystemTransition* aTransition);
    std::map<std::string, SystemTransition*> transitionMap;
  };
  
  const State& getState(const std::string& aStateId) const;
  
  State& getState(const std::string& aStateId);

  ActionableSystem& mResource;

  typedef std::vector<std::string> tStateVec;
  typedef tStateVec::const_iterator tStateIt;
  const std::string mInitialState;
  const std::string mErrorState;
  std::vector<std::string> mStates;
  std::map<std::string, State*> mStateMap;
  std::set<const StateMachine*> mChildFSMs;
  std::set<StateMachine*> mNonConstChildFSMs;
  
  friend SystemTransition& SystemTransition::add(const std::vector<StateMachine::Transition*>& aTransitions);
};

// --------------------------------------------------------
template<class Iterator>
SystemTransition& 
SystemTransition::add(Iterator aBegin, Iterator aEnd, const std::string& aFromState, const std::string& aTransition)
{
  typedef typename std::iterator_traits< Iterator >::value_type val_type;
  BOOST_STATIC_ASSERT_MSG( (boost::is_convertible<val_type, ActionableObject*>::value) , "Dereferencing type Iterator must result in a pointer to a type that inherits from swatch::core::ActionableObject");
  
  return add(aBegin, aEnd, getStateMachine().getId(), aFromState, aTransition);
}


// --------------------------------------------------------
template<class Iterator>
SystemTransition& 
SystemTransition::add(Iterator aBegin, Iterator aEnd, const std::string& aFSM, const std::string& aFromState, const std::string& aTransition)
{
  typedef typename std::iterator_traits< Iterator >::value_type val_type;
  BOOST_STATIC_ASSERT_MSG( (boost::is_convertible<val_type, ActionableObject*>::value) , "Dereferencing type Iterator must result in a pointer to a type that inherits from swatch::core::ActionableObject");
  
  std::vector< StateMachine::Transition* > lTransitions;
  for(Iterator lIt=aBegin; lIt!=aEnd; lIt++)
    lTransitions.push_back( &(*lIt)->getStateMachine(aFSM).getTransition(aFromState, aTransition) );
  
  return add(lTransitions);
}


// --------------------------------------------------------
template<class Collection>
SystemTransition& 
SystemTransition::add(const Collection& aCollection, const std::string& aFromState, const std::string& aTransition) {
  return add( aCollection.begin(), aCollection.end(), aFromState, aTransition);
}


// --------------------------------------------------------
template<class Collection>
SystemTransition&
SystemTransition::add(const Collection& aCollection, const std::string& aFSMId, const std::string& aFromState, const std::string& aTransition){
  return add( aCollection.begin(), aCollection.end(), aFSMId, aFromState, aTransition);
}
DEFINE_SWATCH_EXCEPTION(InvalidSystemTransition);


}
}

#endif	/* __SWATCH_CORE_SYSTEMSTATEMACHINE_HPP__ */

