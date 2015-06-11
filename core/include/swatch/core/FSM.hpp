/**
 * @file    FSM.hpp
 * @author  kreczko
 * @brief   Finite State Machine (FSM) for swatch.
 * @date    November 2014
 *
 */

#ifndef __SWATCH_CORE_FSM_HPP__
#define __SWATCH_CORE_FSM_HPP__
#include "swatch/core/exception.hpp"
#include "toolbox/fsm/FiniteStateMachine.h"
#include "toolbox/Event.h"

namespace swatch {
namespace core {

typedef std::pair<std::string, std::string> StateTransition;

class FSM {
public:
  FSM();
  virtual ~FSM();

  // Execute transition with a given name
  void executeTransition(const std::string& transition);

  void addState(const std::string s);
//  void addInput(const std::string i);

  void addStateTransition(const std::string& from, const std::string& to,  const std::string& input);

  template<class OBJECT>
  void addStateTransition(const std::string& from,  const std::string& to, const std::string& input, OBJECT * obj, void (OBJECT::*func)(toolbox::Event::Reference));

  template <class OBJECT>
  void addTransition(const std::string& from, const std::string& to, const std::string& event, OBJECT* object, bool (OBJECT::*condition)(), void (OBJECT::*function)());

  // for a simple FSM without function calls
  void addTransition ( const std::string& from, const std::string& to, const std::string& event);

  /**
   * sets the initial state for the FSM. The state has to exist (previously
   * added via addState()) otherwise an exception is thrown;
   */
  void setInitialState(const std::string s);

  const std::string& getInitialState() const;

// useful function mostly used internally
  bool hasState(const std::string& s) const;

  const std::vector<std::string>& getStates() const;

  const std::map<std::string, std::string> getStateTransitions( const std::string& s ) const;

  const toolbox::fsm::State getNextXDAQState();
  const toolbox::fsm::State getXDAQState( const std::string& s ) const;

  // Resets the FSM (sets state == initial state)
  void reset();

  // All existing transitions and states are removed
  void clear();

  const std::string& getCurrentState() const;

  std::string getRollbackTransition ( const std::string& from, const std::string& transition );

private:
  class StateMethodSignature {
    public:

      virtual ~StateMethodSignature() {
      }

      virtual bool condition() = 0;

      virtual void method() = 0;
    };

    template <class OBJECT> class StateMethod: public StateMethodSignature {
    public:
      StateMethod ( OBJECT* object, bool ( OBJECT::*condition ) (), void ( OBJECT::*method ) () );
      bool condition();
      void method();

    private:
      OBJECT* obj_;

      bool (OBJECT::*condition_)();

      void (OBJECT::*method_)();
    };

    
    typedef std::map<std::string, toolbox::fsm::State> StateMap;
    typedef std::map<std::string, std::map<std::string, std::string> > TransitionTable;

  std::string initialState_;
  std::string currentState_;
  std::vector<std::string> states_;
//  std::map<std::string, toolbox::fsm::State> state_map_;
//  std::map<std::string, std::map<std::string, std::string> > stateTransitionTable_;
  StateMap state_map_;
  TransitionTable stateTransitionTable_;
  toolbox::fsm::FiniteStateMachine fsm_;
  // xdaq FSM works with chars as States (don't ask why). We need something
  // to create them automatically for us
  // the following two variables are designed to do this

  unsigned int xdaq_state_index_;
  toolbox::fsm::State highestState_;
  std::map<std::string,StateMethodSignature*> stateMethods_;

  const std::string getState(const toolbox::fsm::State& s) const;

  void doNothing ( toolbox::Event::Reference e );
  void moveForward ( toolbox::fsm::FiniteStateMachine& fsm );
  void fireEvent(toolbox::Event::Reference event);
};

} /* namespace core */
} /* namespace swatch */

#include "swatch/core/FSM.hxx"

#endif /* __SWATCH_CORE_FSM_HPP__ */
