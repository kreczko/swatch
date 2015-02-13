/*
 * Finite State Machine (FSM) for swatch.
 *
 *  Created on: 18 Nov 2014
 *      Author: kreczko
 */

#ifndef __SWATCH_CORE_FSM_HPP__
#define __SWATCH_CORE_FSM_HPP__
#include "swatch/core/exception.hpp"
#include "toolbox/fsm/FiniteStateMachine.h"
#include "toolbox/Event.h"

namespace swatch {
namespace core {

typedef std::string State;
typedef toolbox::fsm::State XDAQState;
typedef std::string Input;

class FSM {
public:
	FSM();
	virtual ~FSM();

	void addState(const State s);
	void addInput(const Input i);

	void addStateTransition(const State& from, const State& to, const Input &input);
	template<class OBJECT> void addStateTransition(const State& from, const State& to, const Input& input, OBJECT * obj,
			void (OBJECT::*func)(toolbox::Event::Reference));

	/**
	 * sets the initial state for the FSM. The state has to exist (previously
	 * added via addState()) otherwise an exception is thrown;
	 */
	void setInitialState(State s);
	const State& getInitialState() const;

	void changeState(const State& from, const State& to);

	// useful function mostly used internally
	bool hasState(State s) const;

	const std::vector<State>& getStates() const;
	const std::map<Input, State> getStateTransitions(const State& s) const;

	static const std::string XDAQ_STATES;

	const XDAQState getNextXDAQState();
	const XDAQState getXDAQState(const State& s) const;


	void fireEvent(toolbox::Event::Reference event);

	void reset();

	State getCurrentState() const;

private:
	State initialState_;
	State currentState_;
	std::vector<State> states_;
	std::map<State, XDAQState> state_map_;
	std::map<State, std::map<Input, State> > stateTransitionTable_;
	toolbox::fsm::FiniteStateMachine fsm_;
	// xdaq FSM works with chars as States (don't ask why). We need something
	// to create them automatically for us
	// the following two variables are designed to do this

	unsigned int xdaq_state_index_;

	const State getState(const XDAQState& s) const;
};

} /* namespace core */
} /* namespace swatch */

#include "swatch/core/FSM.hxx"

#endif /* __SWATCH_CORE_FSM_HPP__ */