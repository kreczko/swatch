/* 
 * File:   StateMachine.hpp
 * Author: tom
 *
 * Created on 18 August 2015, 22:58
 */

#ifndef __SWATCH_CORE_STATEMACHINE_HPP__
#define	__SWATCH_CORE_STATEMACHINE_HPP__


#include "swatch/core/ActionableObject.hpp"
#include "swatch/core/CommandVec.hpp"


namespace swatch {
namespace core {

class ActionableSystem;
class ActionStatus;
class Command;
class CommandSequence;
class GateKeeper;
class ReadOnlyXParameterSet;

class StateMachine : public Object {
public:
  class Transition;
  
  virtual ~StateMachine();

  const ActionableObject& getResource() const;

  ActionableObject& getResource();
  
  const std::string& getInitialState() const;
  
  const std::string& getErrorState() const;
  
  const std::vector<std::string>& getStates() const;

  //FIXME: const method SHOULD NOT give non-const pointer to Transition
  const std::map<std::string, Transition*>& getTransitions(const std::string& aStateId) const;
  
  Transition& getTransition(const std::string& aStateId, const std::string&);

  void addState(const std::string& aState);
  
  Transition& addTransition(const std::string& aTransitionId, const std::string& aFromState, const std::string& aToState);
  
  void disengage();
  
  void reset();
  
  typedef CommandVecStatus TransitionStatus;

  class Transition : public CommandVec {
  public:
    Transition(const std::string& aId, StateMachine& aFSM, const std::string& aStartState, const std::string& aEndState);

    const std::string& getStartState() const;
    const std::string& getEndState() const;

    const StateMachine& getStateMachine() const;
    
    StateMachine& getStateMachine();

    Transition& add(Command& aCmd, const std::string& aAlias="");
    Transition& add(CommandSequence& aSequence);

  private:
    StateMachine& mStateMachine;
    const std::string mStartState;
    const std::string mEndState;
  };

  
public:
  StateMachine(const std::string& aId, ActionableObject& aResource, const std::string& aInitialState, const std::string& aErrorState); // when you "engage" an actionable object into a particular transition, its state is set to the "base" state ..
  
private:

  ActionableObject& mResource;
    
  typedef std::vector<std::string> tStateVec;
  typedef tStateVec::const_iterator tStateIt;
  const std::string mInitialState;
  const std::string mErrorState;
  std::vector<std::string> mStates;

  typedef std::map<std::string, Transition*> tTransitionMap;
  std::map<std::string, tTransitionMap> mTransitionMap;
  
  friend class ActionableSystem;
};

DEFINE_SWATCH_EXCEPTION(StateNotDefined);
DEFINE_SWATCH_EXCEPTION(StateAlreadyDefined);

DEFINE_SWATCH_EXCEPTION(TransitionNotDefined);
DEFINE_SWATCH_EXCEPTION(TransitionAlreadyDefined);
}
}


#endif /* __SWATCH_CORE_STATEMACHINE_HPP__ */

