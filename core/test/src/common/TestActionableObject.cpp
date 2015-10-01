/*
 * Command_test.cpp
 *
 *  Created on: 16 Feb 2015
 *      Author: kreczko
 */
#include <boost/test/unit_test.hpp>

// XDAQ headers
#include "xdata/UnsignedInteger.h"

// SWATCH headers
#include "swatch/logger/Log.hpp"
#include "swatch/core/ReadWriteXParameterSet.hpp"
#include "swatch/core/test/DummyActionableObject.hpp"
#include "swatch/core/test/DummyCommand.hpp"
#include "swatch/core/test/DummyGateKeeper.hpp"
#include "swatch/core/test/DummySleepCommand.hpp"
#include "swatch/core/StateMachine.hpp"


using namespace swatch::logger;
using namespace swatch::core;
namespace swatch {
namespace core {
namespace test {

struct ActionableObjectTestSetup {
  ActionableObjectTestSetup();
  ~ActionableObjectTestSetup();

  DummyActionableObject* handler;
  boost::scoped_ptr<Object::Deleter> deleter;
  StateMachine& testOp;
  StateMachine::Transition* transitionItoA;
  StateMachine::Transition* transitionAtoB;
  StateMachine::Transition* transitionBtoI;
  static const std::string opInitialState;
  static const std::string opStateA;
  static const std::string opStateB;
};


ActionableObjectTestSetup::ActionableObjectTestSetup() :
  handler( new DummyActionableObject() ),
  deleter( new ActionableObject::Deleter() ),
  testOp( handler->registerOperation("myTestOp" , opInitialState, "StateE") )
{ 
  handler->registerFunctionoid<DummyCommand>("dummy_1");//, new DummyCommand(&handler));
  handler->registerFunctionoid<DummyCommand>("dummy_2");//, new DummyCommand(&handler));
  handler->registerFunctionoid<DummyCommand>("dummy_3");//, new DummyCommand(&handler));
  handler->registerFunctionoid<DummySleepCommand>("sleep");
  handler->registerOperation("test_2", "0", "err");
  
  testOp.addState(opStateA);
  testOp.addState(opStateB);
  transitionItoA = & testOp.addTransition("t1", opInitialState, opStateA);
  transitionAtoB = & testOp.addTransition("t2", opStateA, opStateB);
  transitionBtoI = & testOp.addTransition("t3", opStateB, opInitialState);
}


ActionableObjectTestSetup::~ActionableObjectTestSetup()
{
  if (deleter.get() != NULL)
    (*deleter)(handler);
  else
    delete handler;
}

const std::string ActionableObjectTestSetup::opInitialState = "s0";
const std::string ActionableObjectTestSetup::opStateA = "sA";
const std::string ActionableObjectTestSetup::opStateB = "sB";



BOOST_AUTO_TEST_SUITE( ActionableObjectTestSuite)


BOOST_FIXTURE_TEST_CASE(TestRegisterCommand,  ActionableObjectTestSetup) {
  LOG(kInfo) << "Running ActionableObjectTestSuite/TestRegisterCommand";
  size_t n_commands = handler->getCommands().size();
  Command& registeredCmd = handler->registerFunctionoid<DummyCommand>("dummy_5000");
  
  size_t n_commands_after = handler->getCommands().size();
  BOOST_CHECK_EQUAL(n_commands_after, n_commands + 1);
  BOOST_CHECK_EQUAL( & handler->getCommand("dummy_5000"), & registeredCmd);
}


BOOST_FIXTURE_TEST_CASE(TestGetCommand,  ActionableObjectTestSetup) {
  LOG(kInfo) << "Running ActionableObjectTestSuite/TestGetCommand";
  Command& dummy_1 = handler->getCommand("dummy_1");
  BOOST_CHECK_EQUAL(dummy_1.getState(), ActionStatus::kInitial);
}


BOOST_FIXTURE_TEST_CASE(TestRegisterOperation,  ActionableObjectTestSetup) {
  LOG(kInfo) << "Running ActionableObjectTestSuite/TestRegisterOperation";
  size_t n_ctrl = handler->getStateMachines().size();
  handler->registerOperation("dummyOp_5000", "NULL", "ERR");
  size_t n_ctrl_after = handler->getStateMachines().size();
  BOOST_CHECK_EQUAL(n_ctrl_after, n_ctrl + 1);
}


BOOST_FIXTURE_TEST_CASE(TestGetOperation,  ActionableObjectTestSetup) {
  LOG(kInfo) << "Running ActionableObjectTestSuite/TestGetOperation";
  BOOST_CHECK_EQUAL( &testOp, & handler->getStateMachine("myTestOp"));
}


BOOST_FIXTURE_TEST_CASE(TestActionableIntialState,  ActionableObjectTestSetup) {
  LOG(kInfo) << "Running ActionableObjectTestSuite/TestActionableIntialState";
  
  BOOST_CHECK( handler->getState().getActions().empty() );
  BOOST_CHECK_EQUAL( handler->getState().getEngagedFSM(), (const StateMachine *) NULL );
  BOOST_CHECK_EQUAL( handler->getState().getState(), "");

//  NewOperation& test_1 = handler->getOperation("test_1").getTransitions("");
}


BOOST_FIXTURE_TEST_CASE(TestActionableEngageOperation, ActionableObjectTestSetup) {
  LOG(kInfo) << "Running ActionableObjectTestSuite/TestActionableEngageOperation";

  ActionableObject::State s = handler->getState();
  BOOST_CHECK_EQUAL( s.getEngagedFSM(), (const StateMachine*) NULL );
  BOOST_CHECK_EQUAL( s.getState(), "" );
  
  // Engaging FSM should put object into FSM's initial state
  BOOST_CHECK_NO_THROW( handler->engageStateMachine(testOp.getId()) );
  BOOST_CHECK_EQUAL( handler->getState().getEngagedFSM(), & testOp );
  BOOST_CHECK_EQUAL( handler->getState().getState(), opInitialState );

  // Trying to engage FSM again shouldn't work
  BOOST_CHECK_THROW( handler->engageStateMachine(testOp.getId()), ResourceInWrongStateMachine );
  BOOST_CHECK_EQUAL( handler->getState().getEngagedFSM(), & testOp );
  BOOST_CHECK_EQUAL( handler->getState().getState(), opInitialState );

  // Disengaging the FSM should reset object's state
  BOOST_CHECK_NO_THROW( testOp.disengage() );
  BOOST_CHECK_EQUAL( handler->getState().getEngagedFSM(), (const StateMachine*) NULL );
  BOOST_CHECK_EQUAL( handler->getState().getState(), "" );
}


BOOST_FIXTURE_TEST_CASE(TestActionableStateTransitions, ActionableObjectTestSetup) {
  LOG(kInfo) << "Running ActionableObjectTestSuite/TestActionableStateTransitions";

  DummyGateKeeper gk;
  
  // Check that transitions not executed before FSM is engaged
  BOOST_CHECK_THROW( transitionItoA->exec(gk, false), ResourceInWrongState);

  // Engage state machine and check initial state
  BOOST_CHECK_NO_THROW( handler->engageStateMachine(testOp.getId()) );
  BOOST_CHECK_EQUAL( handler->getState().getEngagedFSM(), & testOp );
  BOOST_CHECK_EQUAL( handler->getState().getState(), opInitialState );
  
  // Check that invalid transitions not executed
  BOOST_CHECK_THROW( transitionAtoB->exec(gk, false), ResourceInWrongState);
  BOOST_CHECK_EQUAL( handler->getState().getState(), opInitialState );
  BOOST_CHECK_THROW( transitionBtoI->exec(gk, false), ResourceInWrongState);
  BOOST_CHECK_EQUAL( handler->getState().getState(), opInitialState );

  // Execute state transition: 'initial' -> A
  BOOST_CHECK_NO_THROW( transitionItoA->exec(gk, false) );
  BOOST_CHECK_EQUAL( handler->getState().getState(), opStateA );

  // Check that invalid transitions not executed
  BOOST_CHECK_THROW( transitionItoA->exec(gk, false), ResourceInWrongState);
  BOOST_CHECK_EQUAL( handler->getState().getState(), opStateA );
  BOOST_CHECK_THROW( transitionBtoI->exec(gk, false), ResourceInWrongState);
  BOOST_CHECK_EQUAL( handler->getState().getState(), opStateA );

  // Execute state transition: A -> B
  BOOST_CHECK_NO_THROW( transitionAtoB->exec(gk, false) );
  BOOST_CHECK_EQUAL( handler->getState().getState(), opStateB );

  // Engage state machine again; should throw and have no effect on state
  BOOST_CHECK_THROW( handler->engageStateMachine(testOp.getId()), ResourceInWrongStateMachine );
  BOOST_CHECK_EQUAL( handler->getState().getEngagedFSM(), &testOp );
  BOOST_CHECK_EQUAL( handler->getState().getState(), opStateB );
  
  // Reset state machine
  BOOST_CHECK_NO_THROW( testOp.reset() );
  BOOST_CHECK_EQUAL( handler->getState().getEngagedFSM(), &testOp );
  BOOST_CHECK_EQUAL( handler->getState().getState(), opInitialState );
}


BOOST_AUTO_TEST_CASE(TestSafeDeletionOfBusyActionableObject) {
  {
    ActionableObjectTestSetup setup;
    LOG(kInfo) << "Running ActionableObjectTestSuite/TestSafeDeletionOfBusyActionableObject";
  
    Command& sleepCommand = setup.handler->getCommand("sleep");
  
    ReadWriteXParameterSet params;
    params.add("n", xdata::UnsignedInteger(5000));
    params.add("millisecPerSleep", xdata::UnsignedInteger(5));
  
    sleepCommand.exec(params);

    boost::this_thread::sleep_for(boost::chrono::milliseconds(25));
  }
  
  // Sleep for 25ms after the actionable object has been destroyed, ...
  // ... in order to ensure that if the object has been deleted before the command has finished, ...
  // ... then the object is accessed by the Command::code method in the execution thread after the object's deletion ...
  // ... i.e. to reveal bug explained in ticket #1230
  boost::this_thread::sleep_for(boost::chrono::milliseconds(25));
}


BOOST_AUTO_TEST_SUITE_END() // ProcessorTestSuite

} /* namespace test */
} /* namespace core */
} /* namespace swatch */

