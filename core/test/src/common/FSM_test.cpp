/*
 * FSM_test.cpp
 *
 *  Created on: 18 Nov 2014
 *      Author: kreczko
 */
#include <boost/test/unit_test.hpp>

// swatch headers
#include "swatch/core/FSM.hpp"
#include "swatch/logger/Log.hpp"

using namespace swatch::logger;
namespace swatch {
namespace core {
class DummyCaller {
public:
  DummyCaller() {
  }
  ~DummyCaller() {
  }
  void callMe(toolbox::Event::Reference event) {
  }
};

struct FSMs {
  FSMs() :
          default_fsm(),
          default_fsm_with_functions(),
          halted("HALTED"),
          configured("CONFIGURED"),
          enabled("ENABLED"),
          suspended("SUSPENDED"),
          // fireEvent does not use 'origin' so the second parameter
          // can stay empty
          configure(new toolbox::Event("configure")),
          enable(new toolbox::Event("enable", this)),
          suspend(new toolbox::Event("suspend")),
          stop(new toolbox::Event("stop")),
          coldReset(new toolbox::Event("coldReset")),
          caller(new DummyCaller()) {
    default_fsm.addState(halted);
    default_fsm.addState(configured);
    default_fsm.addState(enabled);
    default_fsm.addState(suspended);
    default_fsm.setInitialState(halted);
    default_fsm.reset();

    default_fsm.addStateTransition(halted, halted, "coldReset");
    default_fsm.addStateTransition(halted, configured, "configure");
    default_fsm.addStateTransition(configured, enabled, "enable");
    default_fsm.addStateTransition(enabled, suspended, "suspend");
    default_fsm.addStateTransition(enabled, configured, "stop");
    default_fsm.addStateTransition(suspended, enabled, "enable");
    default_fsm.addStateTransition(suspended, configured, "stop");

    default_fsm_with_functions.addState(halted);
    default_fsm_with_functions.addState(configured);
    default_fsm_with_functions.addState(enabled);
    default_fsm_with_functions.addState(suspended);
    default_fsm_with_functions.setInitialState(halted);
    default_fsm_with_functions.reset();

    default_fsm_with_functions.addStateTransition(halted, halted, "coldReset", caller, &DummyCaller::callMe);
    default_fsm_with_functions.addStateTransition(halted, configured, "configure", caller, &DummyCaller::callMe);
    default_fsm_with_functions.addStateTransition(configured, enabled, "enable", caller, &DummyCaller::callMe);
    default_fsm_with_functions.addStateTransition(enabled, suspended, "suspend", caller, &DummyCaller::callMe);
    default_fsm_with_functions.addStateTransition(enabled, configured, "stop", caller, &DummyCaller::callMe);
    default_fsm_with_functions.addStateTransition(suspended, enabled, "enable", caller, &DummyCaller::callMe);
    default_fsm_with_functions.addStateTransition(suspended, configured, "stop", caller, &DummyCaller::callMe);

  }
  ~FSMs() {

  }

  FSM default_fsm;
  FSM default_fsm_with_functions;
  State halted, configured, enabled, suspended;
  toolbox::Event::Reference configure, enable, suspend, stop, coldReset;
  DummyCaller* caller;
};


BOOST_AUTO_TEST_SUITE( FSMTestSuite )

BOOST_AUTO_TEST_CASE(SetInitialState) {
	LOG(kInfo) << "Running FSMTestSuite/SetInitialState";
	State s("start");
	FSM fsm;
	fsm.addState(s);
	fsm.setInitialState(s);
	BOOST_CHECK_EQUAL(fsm.getInitialState(), s);
}

BOOST_AUTO_TEST_CASE(SetInitialNonExistingState) {
	LOG(kInfo) << "Running FSMTestSuite/SetInitialNonExistingState";
	State s("start");
	FSM fsm;
	BOOST_CHECK_THROW(fsm.setInitialState(s), swatch::core::exception);
}

BOOST_AUTO_TEST_CASE(AddState) {
	LOG(kInfo) << "Running FSMTestSuite/AddState";
	State s1("start");
	State s2("end");
	FSM fsm;
	fsm.addState(s1);
	BOOST_CHECK(fsm.hasState(s1));
	BOOST_CHECK(!fsm.hasState(s2));
}

BOOST_AUTO_TEST_CASE(AddingExistingStateThrowsException) {
	LOG(kInfo) << "Running FSMTestSuite/AddingExistingStateThrowsException";
	State s("init");
	FSM fsm;
	fsm.addState(s);
	BOOST_CHECK_THROW(fsm.addState(s), swatch::core::exception);
}

BOOST_AUTO_TEST_CASE(GetStates) {
	LOG(kInfo) << "Running FSMTestSuite/GetStates";
	State s1("start");
	State s2("end");
	FSM fsm;
	fsm.addState(s1);
	fsm.addState(s2);
	const std::vector<State> states = fsm.getStates();
	BOOST_CHECK_EQUAL(states.size(), size_t(2));
	BOOST_CHECK_EQUAL(states.at(0), s1);
	BOOST_CHECK_EQUAL(states.at(1), s2);
}

BOOST_AUTO_TEST_CASE(GetNextXDAQState) {
	LOG(kInfo) << "Running FSMTestSuite/GetNextXDAQState";
	FSM fsm;
	BOOST_CHECK_EQUAL(fsm.getNextXDAQState(), 'a');
	BOOST_CHECK_EQUAL(fsm.getNextXDAQState(), 'b');
	BOOST_CHECK_EQUAL(fsm.getNextXDAQState(), 'c');
}

BOOST_AUTO_TEST_CASE(GetTooManyXDAQStates) {
	LOG(kInfo) << "Running FSMTestSuite/GetTooManyXDAQStates";
	const std::string xdaq_states = FSM::XDAQ_STATES;
	FSM fsm;
	for(size_t i=0; i < FSM::XDAQ_STATES.size(); ++i){
		fsm.getNextXDAQState();
	}
	// no more states available, throw exception
	BOOST_CHECK_THROW(fsm.getNextXDAQState(), swatch::core::exception);
}

BOOST_AUTO_TEST_CASE(GetXDAQState) {
	LOG(kInfo) << "Running FSMTestSuite/GetXDAQState";
	State s1("start");
	State s2("end");
	State s3("something");
	FSM fsm;
	fsm.addState(s1);
	fsm.addState(s2);
	fsm.addState(s3);
	BOOST_CHECK_EQUAL(fsm.getXDAQState(s1), 'a');
	BOOST_CHECK_EQUAL(fsm.getXDAQState(s2), 'b');
	BOOST_CHECK_EQUAL(fsm.getXDAQState(s3), 'c');
}

BOOST_AUTO_TEST_CASE(GetNonExistingXDAQState) {
	LOG(kInfo) << "Running FSMTestSuite/GetNonExistingXDAQState";
	State s1("start");
	FSM fsm;
	BOOST_CHECK_THROW(fsm.getXDAQState(s1), swatch::core::exception);
}

BOOST_AUTO_TEST_CASE(AddStateTransition) {
	LOG(kInfo) << "Running FSMTestSuite/AddStateTransition";
	State s1("started");
	State s2("stopped");
	FSM fsm;
	fsm.addState(s1);
	fsm.addState(s2);
	fsm.addStateTransition(s1, s2, "stoping");
	std::map<Input, State> transitions = fsm.getStateTransitions(s1);
	BOOST_CHECK_EQUAL(transitions["stoping"], s2);
}

BOOST_AUTO_TEST_CASE(Reset) {
  LOG(kInfo) << "Running FSMTestSuite/Reset";
  State s1("started");
  FSM fsm;
  fsm.addState(s1);
  BOOST_CHECK_THROW(fsm.reset(), swatch::core::exception);
  fsm.setInitialState(s1);
  fsm.reset();
  BOOST_CHECK_EQUAL(fsm.getCurrentState(), s1);
}

BOOST_FIXTURE_TEST_CASE(SimpleStateMachine, FSMs) {
  LOG(kInfo) << "Running FSMTestSuite/SimpleStateMachine";
  // HALTED, CONFIGURED, ENABLED, SUSPENDED
  // inital state == halted
  BOOST_CHECK_EQUAL(default_fsm.getCurrentState(), halted);
  // can't go from halted to suspended!
  BOOST_CHECK_THROW(default_fsm.fireEvent(suspend), swatch::core::exception);
  default_fsm.fireEvent(configure);
  BOOST_CHECK_EQUAL(default_fsm.getCurrentState(), configured);
  default_fsm.fireEvent(enable);
  BOOST_CHECK_EQUAL(default_fsm.getCurrentState(), enabled);
  default_fsm.fireEvent(suspend);
  BOOST_CHECK_EQUAL(default_fsm.getCurrentState(), suspended);
  default_fsm.fireEvent(stop);
  BOOST_CHECK_EQUAL(default_fsm.getCurrentState(), configured);
}

BOOST_FIXTURE_TEST_CASE(SimpleStateMachineWithObject, FSMs) {
  LOG(kInfo) << "Running FSMTestSuite/SimpleStateMachineWithObject";
  // HALTED, CONFIGURED, ENABLED, SUSPENDED
  // inital state == halted
  BOOST_CHECK_EQUAL(default_fsm_with_functions.getCurrentState(), halted);
  // can't go from halted to suspended!
  BOOST_CHECK_THROW(default_fsm_with_functions.fireEvent(suspend), swatch::core::exception);
  default_fsm_with_functions.fireEvent(configure);
  BOOST_CHECK_EQUAL(default_fsm_with_functions.getCurrentState(), configured);
  default_fsm_with_functions.fireEvent(enable);
  BOOST_CHECK_EQUAL(default_fsm_with_functions.getCurrentState(), enabled);
  default_fsm_with_functions.fireEvent(suspend);
  BOOST_CHECK_EQUAL(default_fsm_with_functions.getCurrentState(), suspended);
  default_fsm_with_functions.fireEvent(stop);
  BOOST_CHECK_EQUAL(default_fsm_with_functions.getCurrentState(), configured);
}

BOOST_AUTO_TEST_SUITE_END() // ProcessorTestSuite

} /* namespace core */
} /* namespace swatch */
