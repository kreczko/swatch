
#include <boost/test/unit_test.hpp>


// SWATCH headers
#include "swatch/core/CommandSequence.hpp"
#include "swatch/core/StateMachine.hpp"
#include "swatch/core/SystemStateMachine.hpp"
#include "swatch/core/test/DummyActionableSystem.hpp"
#include "swatch/core/test/DummyActionableObject.hpp"
#include "swatch/core/test/DummyCommand.hpp"
#include "swatch/logger/Log.hpp"


using namespace swatch::logger;


namespace swatch {
namespace core {
namespace test {

struct SystemOperationTestSetup {
  SystemOperationTestSetup() :
    sys(new DummyActionableSystem("dummySys")),
    sysOp( sys->registerStateMachine(sysOpId, sysStateInitial, sysStateErr) ),
    alice1( sys->add(new DummyActionableObject("alice1"), ActionableObject::Deleter()) ),
    alice2( sys->add(new DummyActionableObject("alice2"), ActionableObject::Deleter()) ),
    bob( sys->add(new DummyActionableObject("bob"), ActionableObject::Deleter()) )
  {
    sysOp.addState(sysStateA);
    sysOp.addState(sysStateB);
    sysOp.addState(sysStateC);
    sysItoA = &sysOp.addTransition(sysIdItoA, sysStateInitial, sysStateA);
    sysAtoB = &sysOp.addTransition(sysIdAtoB, sysStateA, sysStateB);
    sysBtoC = &sysOp.addTransition(sysIdBtoC, sysStateB, sysStateC);
  }

  ~SystemOperationTestSetup() {}

  struct Child {
    Child(DummyActionableObject& aObj);
    
    DummyActionableObject& obj;
    StateMachine& op;
    StateMachine::Transition* ItoA;
    StateMachine::Transition* AtoB;
    StateMachine::Transition* BtoC;
  };


  boost::shared_ptr<DummyActionableSystem> sys;
  SystemStateMachine& sysOp;
  Child alice1;
  Child alice2;
  Child bob;
  StateMachine::Transition* bobItoA;
  SystemTransition* sysItoA;
  SystemTransition* sysAtoB;
  SystemTransition* sysBtoC;  
  
  static const std::string commandIdNormal1, commandIdNormal2, commandIdWarning, commandIdError, commandIdThrow;
  static const std::string cmdSeqId;
  static const std::string childOpId;
  static const std::string childStateInitial, childStateErr, childStateA, childStateB, childStateC;
  static const std::string childItoA, childAtoB, childBtoC;

  static const std::string sysOpId, sysStateInitial, sysStateErr, sysStateA, sysStateB, sysStateC;
  static const std::string sysIdItoA, sysIdAtoB, sysIdBtoC;
};

SystemOperationTestSetup::Child::Child(DummyActionableObject& aObj) :
  obj(aObj),
  op( obj.registerOperation(childOpId, childStateInitial, childStateErr) ),
  ItoA(NULL), 
  AtoB(NULL), 
  BtoC(NULL)
{
  obj.registerFunctionoid<DummyCommand>(commandIdNormal1);
  obj.registerFunctionoid<DummyCommand>(commandIdNormal2);
  obj.registerFunctionoid<DummyWarningCommand>(commandIdWarning);
  obj.registerFunctionoid<DummyErrorCommand>(commandIdError);
  obj.registerFunctionoid<DummyThrowCommand>(commandIdThrow);

  obj.registerCommandSequence(cmdSeqId, commandIdNormal1).then(commandIdNormal2);

  op.addState(childStateA);
  op.addState(childStateB);
  op.addState(childStateC);
  ItoA = & op.addTransition(childItoA, childStateInitial, childStateA);
  AtoB = & op.addTransition(childAtoB, childStateA, childStateB);
  BtoC = & op.addTransition(childBtoC, childStateB, childStateC);
}

const std::string SystemOperationTestSetup::commandIdNormal1 = "cmdNormal1";
const std::string SystemOperationTestSetup::commandIdNormal2 = "cmdNormal2";
const std::string SystemOperationTestSetup::commandIdWarning = "cmdWarning";
const std::string SystemOperationTestSetup::commandIdError = "cmdError";
const std::string SystemOperationTestSetup::commandIdThrow = "cmdThrow";

const std::string SystemOperationTestSetup::cmdSeqId = "cmdSeqA";
const std::string SystemOperationTestSetup::childOpId = "anOp";
const std::string SystemOperationTestSetup::childStateInitial = "i";
const std::string SystemOperationTestSetup::childStateErr = "e";
const std::string SystemOperationTestSetup::childStateA = "A";
const std::string SystemOperationTestSetup::childStateB = "B";
const std::string SystemOperationTestSetup::childStateC = "C";
const std::string SystemOperationTestSetup::childItoA = "initialToA";
const std::string SystemOperationTestSetup::childAtoB = "AtoB";
const std::string SystemOperationTestSetup::childBtoC = "BtoC";
  
const std::string SystemOperationTestSetup::sysOpId = "mySysOp";
const std::string SystemOperationTestSetup::sysStateInitial = "state0";
const std::string SystemOperationTestSetup::sysStateErr = "error";
const std::string SystemOperationTestSetup::sysStateA = "stateA";
const std::string SystemOperationTestSetup::sysStateB = "stateB";
const std::string SystemOperationTestSetup::sysStateC = "stateC";
const std::string SystemOperationTestSetup::sysIdItoA = "sysIToA";
const std::string SystemOperationTestSetup::sysIdAtoB = "sysAtoB";
const std::string SystemOperationTestSetup::sysIdBtoC = "sysBtoC";



BOOST_AUTO_TEST_SUITE( SystemOperationTestSuite )


BOOST_AUTO_TEST_CASE(TestAddState) {
  LOG(kInfo) << "Running SystemOperationTestSuite/TestAddState";

  DummyActionableSystem sys("dummySys");  
  swatch::core::SystemStateMachine& op = * new SystemStateMachine("anOp", sys, "someState", "myErrState");
  BOOST_CHECK_EQUAL( &sys.getObj(op.getId()), (Object*) &op);
  
  BOOST_CHECK_EQUAL(op.getInitialState(), "someState");
  BOOST_CHECK_EQUAL(op.getErrorState(), "myErrState");
  std::vector<std::string> expectedStates = {"someState", "myErrState"};
  BOOST_CHECK_EQUAL_COLLECTIONS( op.getStates().begin(), op.getStates().end(), expectedStates.begin(), expectedStates.end() );
  
  // addState should throw if state already defined
  BOOST_CHECK_THROW(op.addState("someState"), swatch::core::StateAlreadyDefined);
  BOOST_CHECK_THROW(op.addState("myErrState"), swatch::core::StateAlreadyDefined);
  
  // addState should add the state if not already defined
  BOOST_CHECK_NO_THROW(op.addState("anotherState"));
  expectedStates.push_back("anotherState");
  BOOST_CHECK_EQUAL_COLLECTIONS( op.getStates().begin(), op.getStates().end(), expectedStates.begin(), expectedStates.end() );
}


BOOST_AUTO_TEST_CASE(TestAddTransition) {
  LOG(kInfo) << "Running SystemOperationTestSuite/TestAddTransition";

  DummyActionableSystem sys("dummySys");  
  swatch::core::SystemStateMachine& op = * new SystemStateMachine("anOp", sys, "state0", "errState");
  op.addState("state1");
  
  BOOST_CHECK_EQUAL( op.getTransitions("state0").size(), size_t(0) );
  BOOST_CHECK_EQUAL( op.getTransitions("state1").size(), size_t(0) );

  // 1) Add transition
  SystemTransition& tA = op.addTransition("transitionA", "state0", "state1");

  BOOST_CHECK_EQUAL(tA.getId(), "transitionA");
  BOOST_CHECK_EQUAL(tA.getEndState(), "state1");
  BOOST_CHECK_EQUAL(&tA.getStateMachine(), &op);
  
  BOOST_CHECK_EQUAL(op.getTransitions("state0").size(), size_t(1));
  BOOST_CHECK_EQUAL(op.getTransitions("state0").find("transitionA")->second, &tA);
  BOOST_CHECK_EQUAL(op.getTransitions("state1").size(), size_t(0));

  // 2) Check that exception is thrown in each error case
  BOOST_CHECK_THROW( op.addTransition("newId", "s", "state1"), StateNotDefined);
  BOOST_CHECK_THROW( op.addTransition("newId", "state1", "s"), StateNotDefined);
  BOOST_CHECK_THROW( op.addTransition("transitionA", "state0", "state1"), TransitionAlreadyDefined);
}


BOOST_FIXTURE_TEST_CASE(TestAddTransitionSteps, SystemOperationTestSetup ) {
  LOG(kInfo) << "Running SystemOperationTestSuite/TestAddTransitionSteps";

  BOOST_REQUIRE_EQUAL( sysItoA->size(), size_t(0) );
  BOOST_CHECK( sysItoA->begin() == sysItoA->end());
  BOOST_CHECK_EQUAL( sysOp.getParticipants().size(), size_t(0));
  std::set<StateMachine*> participants;
  BOOST_CHECK_EQUAL_COLLECTIONS( sysOp.getParticipants().begin(), sysOp.getParticipants().end(), participants.begin(), participants.end());

  // 1) add step method should throw if receives empty vector
  BOOST_CHECK_THROW ( sysItoA->add(std::vector<StateMachine::Transition*>{}), InvalidSystemTransition );
  BOOST_CHECK_EQUAL( sysItoA->size(), size_t(0) );
  
  // 2) Add a transition step 
  std::vector<StateMachine::Transition*> childTransitions = {alice1.ItoA, alice2.ItoA, bob.ItoA};
  sysItoA->add(childTransitions);
  BOOST_CHECK_EQUAL( sysItoA->size(), size_t(1) );
  BOOST_CHECK( (sysItoA->begin() + 1) == sysItoA->end() );
  BOOST_CHECK_EQUAL_COLLECTIONS( sysItoA->begin()->get().begin(), sysItoA->begin()->get().end(), childTransitions.begin(), childTransitions.end());
  participants = {&alice1.op, &alice2.op, &bob.op};
  BOOST_CHECK_EQUAL_COLLECTIONS( sysOp.getParticipants().begin(), sysOp.getParticipants().end(), participants.begin(), participants.end() );
  
  // 3) Add step method should throw if add transition from a non-child
  DummyActionableObject otherObj("intruder");
  otherObj.registerOperation("anOp", "s0", "sE").addState("sA");
  StateMachine::Transition& t = otherObj.getStateMachine("anOp").addTransition("t0A", "s0", "sA");
  BOOST_CHECK_THROW ( sysItoA->add(std::vector<StateMachine::Transition*>{&t}), InvalidSystemTransition );
  BOOST_CHECK_EQUAL( sysItoA->size(), size_t(1) );
  BOOST_CHECK_EQUAL_COLLECTIONS( sysOp.getParticipants().begin(), sysOp.getParticipants().end(), participants.begin(), participants.end() );
}


BOOST_FIXTURE_TEST_CASE(TestEngageOperation, SystemOperationTestSetup ) {
  LOG(kInfo) << "Running SystemOperationTestSuite/TestExecNullTransitions";

  BOOST_CHECK_EQUAL( sys->getState().first, (SystemStateMachine*) NULL);
  BOOST_CHECK_EQUAL( sys->getState().second, "");
  BOOST_CHECK_EQUAL( alice1.obj.getState().getEngagedFSM(), (StateMachine*) NULL);
  BOOST_CHECK_EQUAL( alice1.obj.getState().getState(), "");
  BOOST_CHECK_EQUAL( alice2.obj.getState().getEngagedFSM(), (StateMachine*) NULL);
  BOOST_CHECK_EQUAL( alice2.obj.getState().getState(), "");
  BOOST_CHECK_EQUAL( bob.obj.getState().getEngagedFSM(), (StateMachine*) NULL);
  BOOST_CHECK_EQUAL( bob.obj.getState().getState(), "");
  
  BOOST_REQUIRE( sysOp.getParticipants().empty() );
  sysItoA->add( std::vector<StateMachine::Transition*>{alice1.ItoA, bob.ItoA} );
  std::set<const StateMachine*> expectedParticipants = {&alice1.op, &bob.op};
  BOOST_REQUIRE_EQUAL_COLLECTIONS(sysOp.getParticipants().begin(), sysOp.getParticipants().end(), expectedParticipants.begin(), expectedParticipants.end() );
  
  sys->engageStateMachine( sysOpId );
  BOOST_CHECK_EQUAL( sys->getState().first, &sysOp);
  BOOST_CHECK_EQUAL( sys->getState().first, &sysOp);
  BOOST_CHECK_EQUAL( alice1.obj.getState().getEngagedFSM(), & alice1.obj.getStateMachine(childOpId));
  BOOST_CHECK_EQUAL( alice1.obj.getState().getState(), childStateInitial);
  BOOST_CHECK_EQUAL( alice2.obj.getState().getEngagedFSM(), (StateMachine*) NULL);
  BOOST_CHECK_EQUAL( alice2.obj.getState().getState(), "");
  BOOST_CHECK_EQUAL( bob.obj.getState().getEngagedFSM(), & bob.obj.getStateMachine(childOpId));
  BOOST_CHECK_EQUAL( bob.obj.getState().getState(), childStateInitial);
}

BOOST_FIXTURE_TEST_CASE(TestExecNullTransitions, SystemOperationTestSetup ) {
  LOG(kInfo) << "Running SystemOperationTestSuite/TestExecNullTransitions";

  // 1) Check that can't run transition when in other state machine
  
  // 2) Engage the state machine

  // ...
}


BOOST_FIXTURE_TEST_CASE(TestExecNonNullTransitions, SystemOperationTestSetup ) {
  LOG(kInfo) << "Running SystemOperationTestSuite/TestExecNonNullTransitions";

  // 1) Engage the state machine
  BOOST_CHECK_EQUAL( sys->getState().first, (SystemStateMachine*) NULL);
  BOOST_CHECK_EQUAL( sys->getState().second, "");
  sys->engageStateMachine( sysOpId );
  BOOST_CHECK_EQUAL( sys->getState().first, &sysOp);
  BOOST_CHECK_EQUAL( sys->getState().second, sysStateInitial);

  // ...
}

BOOST_AUTO_TEST_SUITE_END() // OperationTestSuite

} /* namespace test */
} /* namespace core */
} /* namespace swatch */

