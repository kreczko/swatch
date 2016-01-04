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
class SystemStateMachine;


class StateMachine : public Object {
public:
  class Transition;
  
  StateMachine(const std::string& aId, ActionableObject& aResource, MutableActionableStatus& aStatus, const std::string& aInitialState, const std::string& aErrorState); 
 
  virtual ~StateMachine();

  //! Returns actionable object that this FSM belongs to
  const ActionableObject& getActionable() const;

  //! Returns actionable object that this FSM belongs to
  ActionableObject& getActionable();

  //! Returns ID of this FSM's initial state
  const std::string& getInitialState() const;

  //! Returns ID of this FSM's error state
  const std::string& getErrorState() const;

  //! Returns IDs of all of this FSM's states
  const std::vector<std::string>& getStates() const;

  //FIXME: const method SHOULD NOT give non-pointer to Transition
  //! Returns map of all transitions (keyed by transition ID string) that start from the specified state
  const std::map<std::string, Transition*>& getTransitions(const std::string& aStateId) const;

  /*!
   * @brief Returns transition of specified ID, from specified state
   * @param aStateId ID string of start state
   * @param aTransitionId Transition's ID string
   */
  Transition& getTransition(const std::string& aStateId, const std::string& aTransitionId);

  //! Add state of specified ID
  void addState(const std::string& aState);

  /*!
   * @brief Add transition to this FSM
   * @param gaTransitionId The new transition's ID
   * @param aFromState State that transition starts from
   * @param aToState State that transition goes to (in case no error occurs)
   */
  Transition& addTransition(const std::string& aTransitionId, const std::string& aFromState, const std::string& aToState);

  //! Engage this FSM
  void engage(const GateKeeper& aGateKeeper);
  
  //! Disengage this FSM
  void disengage();

  //! Reset this FSM to initial state
  void reset(const GateKeeper& aGateKeeper);

  static void resetMaskableObjects(ActionableObject& aObj, const GateKeeper& aGateKeeper);
  
  typedef CommandVecStatus TransitionStatus;

  class Transition : public CommandVec {
  public:
    Transition(const std::string& aId, StateMachine& aFSM, MutableActionableStatus& aActionableStatus, const std::string& aStartState, const std::string& aEndState);

    //! State that this transition starts from
    const std::string& getStartState() const;

    //! State that this transition goes to (in case no error occurs)
    const std::string& getEndState() const;

    //! Returns FSM that this transition belongs to
    const StateMachine& getStateMachine() const;

    //! Returns FSM that this transition belongs to
    StateMachine& getStateMachine();

    /*!
     * @brief Add an individual command to this transition
     * @param aCmd the command
     * @param aNamespace Namespace used when retreving parameters from gatekeeper
     */
    Transition& add(Command& aCmd, const std::string& aNamespace="");

    //! Add all commands from specified sequence to this transition
    Transition& add(CommandSequence& aSequence);

    /**
     * Run the sequence, extracting the parameters for each command from the supplied gatekeeper
     * 
     * @param aGateKeeper Gatekeeper that's used to extract the parameters
     * @param aUseThreadPool Run the sequence asynchronously in the swatch::core::ThreadPool ; if equals false, then the sequence is run synchronously
     */
    void exec(const GateKeeper& aGateKeeper, const bool& aUseThreadPool = true ); 

    /** 
     * Run this sequence from another functionoid that already has control of resource, extracting the parameters for each command from the supplied gatekeeper
     * 
     * @param aGateKeeper Gatekeeper that's used to extract the parameters
     * @param aUseThreadPool Run the command asynchronously in the swatch::core::ThreadPool ; if equals false, then the command is run synchronously
     */
    void exec(const BusyGuard* aGuard, const GateKeeper& aGateKeeper, const bool& aUseThreadPool = true );

  private:
    virtual void extractMonitoringSettings(const GateKeeper& aGateKeeper, MonitoringSettings_t& aMonSettings) const;

    void run(boost::shared_ptr<BusyGuard> aGuard);

    void applyMonitoringSettings();

    void changeState(const ActionableStatusGuard& aGuard);

    StateMachine& mStateMachine;
    MutableActionableStatus& mActionableStatus;
    const std::string mStartState;
    const std::string mEndState;
    MonitoringSettings_t mCachedMonitoringSettings;
  };

private:

  struct State : public Object {
    State(const std::string& aId);
    void addTransition(Transition* aTransition);
    std::map<std::string, Transition*> transitionMap;
  };
  
  const State& getState(const std::string& aStateId) const;
  
  State& getState(const std::string& aStateId);
    
  ActionableObject& mResource;
  MutableActionableStatus& mStatus;
    
  typedef std::vector<std::string> StateVec_t;
  typedef StateVec_t::const_iterator StateIt_t;
  const std::string mInitialState;
  const std::string mErrorState;
  std::vector<std::string> mStates;

  std::map<std::string, State*> mStateMap;
};

DEFINE_SWATCH_EXCEPTION(StateNotDefined);
DEFINE_SWATCH_EXCEPTION(StateAlreadyDefined);

DEFINE_SWATCH_EXCEPTION(TransitionNotDefined);
DEFINE_SWATCH_EXCEPTION(TransitionAlreadyDefined);
}
}


#endif /* __SWATCH_CORE_STATEMACHINE_HPP__ */

