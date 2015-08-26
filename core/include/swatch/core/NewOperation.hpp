/* 
 * File:   NewOperation.hpp
 * Author: tom
 *
 * Created on 18 August 2015, 22:58
 */

#ifndef __SWATCH_CORE_NEWOPERATION_HPP__
#define	__SWATCH_CORE_NEWOPERATION_HPP__


#include "swatch/core/Functionoid.hpp"


namespace swatch {
namespace core {

class ActionStatus;
class GateKeeper;
    
class NewOperation : public Functionoid {
public:
  class Transition;
    
  virtual ~NewOperation();
  
  void executeTransition(const std::string& aId, const GateKeeper& aGateKeeper);
  
  const std::string& getCurrentState() const;
  
  Transition& getCurrentTransition() const; //FIXME: Maybe remove, with only access to current transition from
  
  const std::vector<std::string>& getStates() const;
  
  const std::map<std::string, Transition*>& getStateTransitions(const std::string& aStateId) const;
  
  void addState(const std::string& aState);
  
  void addTransition(const std::string& aTransition, const std::string& aInitialState, const std::string& aFinalState);
  
//  void addTransitionStep(const std::string& aTransition, const std::vector<std::string>& aObjIds, const std::string& aChildTransitionId ); // Throw if any of child objs don't have cmdSeq of same ID
  
  void addTransitionStep(const std::string& aTransition, const std::string& aCommandOrCommandSeqId); // Throw if resource doesn't have command/cmdSeq with same IDs
  
  // Maybe, if not covered by getStateTransitions?? 
  const Transition& getTransition(const std::string& aTransitionId) const;

  class TransitionStep;
  // Transition composed of consecutive steps
  //  * each step can be one or more commands, or command sequences on this resource, ...
  //    ... or a transition on another resource that is a descendant of this operation's resource
  //  * each action must be a descendant of functionoid
  
  class TransitionStep {
      //FIXME: Need some constructors :)
      
      // Iteration over all actions making up this transition step; all items in any given step are executed in parallel
      class const_iterator; // Dereferencing => Functionoid& - a command / commandSequence / transition
      const_iterator begin();
      const_iterator end();
  };
  
  class TransitionStatus {
    enum State {
      kInitial,
      kScheduled,
      kRunning,
      kDone,
      kWarning,
      kError
    }; 

    State getState() const;
      
    float getRunningTime() const;
      
    float getProgress() const;
      
    const TransitionStep* getCurrentStep() const;
      
    size_t getNumberOfCompletedSteps() const;
      
    //TODO: N.B. Would have to make {Command,CommandSequence,Transition}Status all inherit from ActionStatus
    //      That maybe implies a common ActionState enum
    const std::vector<std::vector<const ActionStatus*> >& getStepStatus() const;
  };
  
  class Transition : public Functionoid {
      //FIXME: Need some constructors :)
      
      // Iteration over all steps in this transition 
      class const_iterator; // Dereferencing => TransitionStep
      const_iterator begin();
      const_iterator end();
      
      //! Number of steps
      size_t size(); 
      
      TransitionStatus getStatus() const;
      
      const NewOperation& getOperation() const;
  };
  
//protected:
public:
  NewOperation(const std::string& aId, const std::string aInitialState); // when you "engage" an actionable object into a particular transition, its state is set to the "base" state ..
  
private:
  std::vector<std::string> mStates;

  std::map<std::string, std::map<std::string, Transition*> > mTransitionMap;
  
//  //TODO: Will have to review the FSM ...
//  FSM* fsm_;  
};

}
}



#endif	/* __SWATCH_CORE_NEWOPERATION_HPP__ */

