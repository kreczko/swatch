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
  void callMe() {
  }
  bool check() {
    return true;
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
          // executeTransition does not use 'origin' so the second parameter
          // can stay empty
          configure("configure"),
          enable("enable"),
          suspend("suspend"),
          stop("stop"),
          coldReset("coldReset"),
          caller(new DummyCaller()) {
    default_fsm.addState(halted);
    default_fsm.addState(configured);
    default_fsm.addState(enabled);
    default_fsm.addState(suspended);
    default_fsm.setInitialState(halted);
    default_fsm.reset();

    default_fsm.addTransition(halted, halted, "coldReset");
    default_fsm.addTransition(halted, configured, "configure");
    default_fsm.addTransition(configured, enabled, "enable");
    default_fsm.addTransition(enabled, suspended, "suspend");
    default_fsm.addTransition(enabled, configured, "stop");
    default_fsm.addTransition(suspended, enabled, "enable");
    default_fsm.addTransition(suspended, configured, "stop");

    default_fsm_with_functions.addState(halted);
    default_fsm_with_functions.addState(configured);
    default_fsm_with_functions.addState(enabled);
    default_fsm_with_functions.addState(suspended);
    default_fsm_with_functions.setInitialState(halted);
    default_fsm_with_functions.reset();

    default_fsm_with_functions.addTransition(halted, halted, "coldReset", caller, &DummyCaller::check, &DummyCaller::callMe);
    default_fsm_with_functions.addTransition(halted, configured, "configure", caller, &DummyCaller::check, &DummyCaller::callMe);
    default_fsm_with_functions.addTransition(configured, enabled, "enable", caller, &DummyCaller::check, &DummyCaller::callMe);
    default_fsm_with_functions.addTransition(enabled, suspended, "suspend", caller, &DummyCaller::check, &DummyCaller::callMe);
    default_fsm_with_functions.addTransition(enabled, configured, "stop", caller, &DummyCaller::check, &DummyCaller::callMe);
    default_fsm_with_functions.addTransition(suspended, enabled, "enable", caller, &DummyCaller::check, &DummyCaller::callMe);
    default_fsm_with_functions.addTransition(suspended, configured, "stop", caller, &DummyCaller::check, &DummyCaller::callMe);

  }
  ~FSMs() {

  }

  FSM default_fsm;
  FSM default_fsm_with_functions;
  std::string halted, configured, enabled, suspended;
  std::string configure, enable, suspend, stop, coldReset;
  DummyCaller* caller;
};


BOOST_AUTO_TEST_SUITE( FSMTestSuite )

BOOST_AUTO_TEST_CASE(SetInitialState) {
	LOG(kInfo) << "Running FSMTestSuite/SetInitialState";
	std::string s("start");
	FSM fsm;
	fsm.addState(s);
	fsm.setInitialState(s);
	BOOST_CHECK_EQUAL(fsm.getInitialState(), s);
}

BOOST_AUTO_TEST_CASE(SetInitialNonExistingState) {
	LOG(kInfo) << "Running FSMTestSuite/SetInitialNonExistingState";
	std::string s("start");
	FSM fsm;
	BOOST_CHECK_THROW(fsm.setInitialState(s), swatch::core::exception);
}

BOOST_AUTO_TEST_CASE(AddState) {
	LOG(kInfo) << "Running FSMTestSuite/AddState";
	std::string s1("start");
	std::string s2("end");
	FSM fsm;
	fsm.addState(s1);
	BOOST_CHECK(fsm.hasState(s1));
	BOOST_CHECK(!fsm.hasState(s2));
}

BOOST_AUTO_TEST_CASE(AddingExistingStateThrowsException) {
	LOG(kInfo) << "Running FSMTestSuite/AddingExistingStateThrowsException";
	std::string s("init");
	FSM fsm;
	fsm.addState(s);
	BOOST_CHECK_THROW(fsm.addState(s), swatch::core::exception);
}

BOOST_AUTO_TEST_CASE(GetStates) {
	LOG(kInfo) << "Running FSMTestSuite/GetStates";
	std::string s1("start");
	std::string s2("end");
	FSM fsm;
	fsm.addState(s1);
	LOG(kInfo) << "Running FSMTestSuite/GetStates";
	fsm.addState(s2);
	LOG(kInfo) << "Running FSMTestSuite/GetStates";
	const std::vector<std::string> states = fsm.getStates();
	LOG(kInfo) << states.at(0);
	BOOST_CHECK_EQUAL(states.size(), size_t(2));
	BOOST_CHECK_EQUAL(states.at(0), s1);
	BOOST_CHECK_EQUAL(states.at(1), s2);
}

BOOST_AUTO_TEST_CASE(GetNextXDAQState) {
	LOG(kInfo) << "Running FSMTestSuite/GetNextXDAQState";
	FSM fsm;
	BOOST_CHECK_EQUAL(fsm.getNextXDAQState(), 0x1);
	BOOST_CHECK_EQUAL(fsm.getNextXDAQState(), 0x2);
	BOOST_CHECK_EQUAL(fsm.getNextXDAQState(), 0x3);
}

BOOST_AUTO_TEST_CASE(GetTooManyXDAQStates) {
	LOG(kInfo) << "Running FSMTestSuite/GetTooManyXDAQStates";
	FSM fsm;
	for(unsigned int i=0; i < 254; ++i)
		fsm.getNextXDAQState();
	// no more states available, throw exception
	BOOST_CHECK_THROW(fsm.getNextXDAQState(), swatch::core::exception);
}

BOOST_AUTO_TEST_CASE(GetXDAQState) {
	LOG(kInfo) << "Running FSMTestSuite/GetXDAQState";
	std::string s1("start");
	std::string s2("end");
	std::string s3("something");
	FSM fsm;
	fsm.addState(s1);
	fsm.addState(s2);
	fsm.addState(s3);
	BOOST_CHECK_EQUAL(fsm.getXDAQState(s1), 0x1);
	BOOST_CHECK_EQUAL(fsm.getXDAQState(s2), 0x2);
	BOOST_CHECK_EQUAL(fsm.getXDAQState(s3), 0x3);
}

BOOST_AUTO_TEST_CASE(GetNonExistingXDAQState) {
	LOG(kInfo) << "Running FSMTestSuite/GetNonExistingXDAQState";
	std::string s1("start");
	FSM fsm;
	BOOST_CHECK_THROW(fsm.getXDAQState(s1), swatch::core::exception);
}

BOOST_AUTO_TEST_CASE(AddStateTransition) {
	LOG(kInfo) << "Running FSMTestSuite/AddStateTransition";
	std::string s1("started");
	std::string s2("stopped");
	FSM fsm;
	fsm.addState(s1);
	fsm.addState(s2);
	fsm.addStateTransition(s1, s2, "stoping");
	std::map<std::string, std::string> transitions = fsm.getStateTransitions(s1);
	BOOST_CHECK_EQUAL(transitions["stoping"], s2);
}

BOOST_AUTO_TEST_CASE(Reset) {
  LOG(kInfo) << "Running FSMTestSuite/Reset";
  std::string s1("started");
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
//  BOOST_CHECK_THROW(default_fsm.executeTransition(suspend), swatch::core::exception);
  default_fsm.executeTransition(configure);
  BOOST_CHECK_EQUAL(default_fsm.getCurrentState(), configured);
  default_fsm.executeTransition(enable);
  BOOST_CHECK_EQUAL(default_fsm.getCurrentState(), enabled);
  default_fsm.executeTransition(suspend);
  BOOST_CHECK_EQUAL(default_fsm.getCurrentState(), suspended);
  default_fsm.executeTransition(stop);
  BOOST_CHECK_EQUAL(default_fsm.getCurrentState(), configured);
}

BOOST_FIXTURE_TEST_CASE(SimpleStateMachineWithObject, FSMs) {
  LOG(kInfo) << "Running FSMTestSuite/SimpleStateMachineWithObject";
  // HALTED, CONFIGURED, ENABLED, SUSPENDED
  // inital state == halted
  BOOST_CHECK_EQUAL(default_fsm_with_functions.getCurrentState(), halted);
  // can't go from halted to suspended!
//  BOOST_CHECK_THROW(default_fsm_with_functions.executeTransition(suspend), swatch::core::exception);
  default_fsm_with_functions.executeTransition(configure);
  BOOST_CHECK_EQUAL(default_fsm_with_functions.getCurrentState(), configured);
  default_fsm_with_functions.executeTransition(enable);
  BOOST_CHECK_EQUAL(default_fsm_with_functions.getCurrentState(), enabled);
  default_fsm_with_functions.executeTransition(suspend);
  BOOST_CHECK_EQUAL(default_fsm_with_functions.getCurrentState(), suspended);
  default_fsm_with_functions.executeTransition(stop);
  BOOST_CHECK_EQUAL(default_fsm_with_functions.getCurrentState(), configured);
}

BOOST_FIXTURE_TEST_CASE(Clear, FSMs) {
  BOOST_CHECK(default_fsm_with_functions.getCurrentState() != "\0");
  BOOST_CHECK(default_fsm_with_functions.getInitialState() != "\0");
  BOOST_CHECK(default_fsm_with_functions.getStates().size() != size_t(0));
  default_fsm_with_functions.clear();
  BOOST_CHECK_EQUAL(default_fsm_with_functions.getCurrentState(), "\0");
  BOOST_CHECK_EQUAL(default_fsm_with_functions.getInitialState(), "\0");
  BOOST_CHECK_EQUAL(default_fsm_with_functions.getStates().size(), size_t(0));
}

BOOST_AUTO_TEST_SUITE_END() // FSMTestSuite

} /* namespace core */
} /* namespace swatch */
