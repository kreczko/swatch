
#include <boost/test/unit_test.hpp>


// SWATCH headers
#include "swatch/core/CommandSequence.hpp"
#include "swatch/core/StateMachine.hpp"
#include "swatch/core/test/DummyActionableObject.hpp"
#include "swatch/core/test/DummyGateKeeper.hpp"
#include "swatch/core/test/DummyCommand.hpp"
#include "swatch/logger/Log.hpp"


using namespace swatch::logger;
namespace swatch {
namespace core {
namespace test {

  struct StateMachineTestSetup {
    StateMachineTestSetup() :
      obj(new DummyActionableObject("dummyObj"), ActionableObject::Deleter()),
      cmdNormal1( obj->registerFunctionoid<DummyCommand>("cmdNormal1") ),
      cmdNormal2( obj->registerFunctionoid<DummyCommand>("cmdNormal2") ),
      cmdWarning( obj->registerFunctionoid<DummyWarningCommand>("cmdWarning") ),
      cmdError( obj->registerFunctionoid<DummyErrorCommand>("cmdError") ),
      cmdSeq( obj->registerCommandSequence(cmdSeqId, cmdNormal1).then(cmdNormal2) ),
      testFSM( obj->registerStateMachine("myTestFSM" , fsmState0, fsmStateError) )
    {
      testFSM.addState(fsmStateA);
      testFSM.addState(fsmStateB);
      transitionItoA = & testFSM.addTransition("t1", fsmState0, fsmStateA);
      transitionAtoB = & testFSM.addTransition("t2", fsmStateA, fsmStateB);
      transitionBtoI = & testFSM.addTransition("t2", fsmStateB, fsmState0);

      GateKeeper::tTable tbl(new GateKeeper::tParameters());
      tbl->insert( GateKeeper::tParameters::value_type(DummyCommand::paramToDo, GateKeeper::tParameter(new xdata::String(""))));
      tbl->insert( GateKeeper::tParameters::value_type(DummyCommand::paramX, GateKeeper::tParameter(new xdata::Integer(42))));
      gk.addTable("common", tbl);
    }
    
    ~StateMachineTestSetup() {}
    
    boost::shared_ptr<DummyActionableObject> obj;
    Command& cmdNormal1;
    Command& cmdNormal2;
    Command& cmdWarning;
    Command& cmdError;
    CommandSequence& cmdSeq;
    StateMachine& testFSM;
    StateMachine::Transition* transitionItoA;
    StateMachine::Transition* transitionAtoB;
    StateMachine::Transition* transitionBtoI;
    DummyGateKeeper gk;

    static const std::string cmdSeqId;
    static const std::string fsmState0, fsmStateError, fsmStateA, fsmStateB;
  };
  

  
  const std::string StateMachineTestSetup::cmdSeqId = "cmdSeqA";

  const std::string StateMachineTestSetup::fsmState0 = "s0";
  const std::string StateMachineTestSetup::fsmStateError = "sErr";
  const std::string StateMachineTestSetup::fsmStateA = "sA";
  const std::string StateMachineTestSetup::fsmStateB = "sB";
  


BOOST_AUTO_TEST_SUITE( StateMachineTestSuite )


/* ------------------------------------------------------------------------- */
/*   PART 1: CONSTRUCTION & INITIALISATION (defining states & transitions)   */
/* ------------------------------------------------------------------------- */

BOOST_AUTO_TEST_CASE(TestConstruction) {
  LOG(kInfo) << "Running StateMachineTestSuite/TestConstruction";

  DummyActionableObject obj("dummy");
  swatch::core::StateMachine& fsm = obj.registerStateMachine("anOp", "someState", "myErrState");
  
  BOOST_CHECK_EQUAL(&fsm.getResource(), &obj);
  BOOST_CHECK_EQUAL(fsm.getInitialState(), "someState");
  BOOST_CHECK_EQUAL(fsm.getErrorState(), "myErrState");
  BOOST_CHECK_EQUAL( &obj.getObj(fsm.getId()), (Object*) &fsm);
  
  std::vector<std::string> expectedStates = {"someState", "myErrState"};
  BOOST_CHECK_EQUAL_COLLECTIONS( fsm.getStates().begin(), fsm.getStates().end(), expectedStates.begin(), expectedStates.end() );
}


BOOST_AUTO_TEST_CASE(TestAddState) {
  LOG(kInfo) << "Running StateMachineTestSuite/TestAddState";

  DummyActionableObject obj("dummy");
  swatch::core::StateMachine& fsm = obj.registerStateMachine("anOp", "someState", "myErrState");

  std::vector<std::string> expectedStates = {"someState", "myErrState"};
  BOOST_REQUIRE_EQUAL_COLLECTIONS( fsm.getStates().begin(), fsm.getStates().end(), expectedStates.begin(), expectedStates.end() );
  
  // addState should throw if state already defined
  BOOST_CHECK_THROW(fsm.addState("someState"), swatch::core::StateAlreadyDefined);
  BOOST_CHECK_THROW(fsm.addState("myErrState"), swatch::core::StateAlreadyDefined);
  
  // addState should add the state if not already defined
  BOOST_CHECK_NO_THROW(fsm.addState("anotherState"));
  expectedStates.push_back("anotherState");
  BOOST_CHECK_EQUAL_COLLECTIONS( fsm.getStates().begin(), fsm.getStates().end(), expectedStates.begin(), expectedStates.end() );
}


BOOST_AUTO_TEST_CASE(TestAddTransition) {
  LOG(kInfo) << "Running StateMachineTestSuite/TestAddTransition";

  DummyActionableObject obj("dummy");  
  swatch::core::StateMachine& fsm = obj.registerStateMachine("aFSM", "state0", "errState");
  fsm.addState("state1");
  
  BOOST_CHECK_EQUAL( fsm.getTransitions("state0").size(), size_t(0) );
  BOOST_CHECK_EQUAL( fsm.getTransitions("state1").size(), size_t(0) );

  // 1) Add transition
  StateMachine::Transition& tA = fsm.addTransition("transitionA", "state0", "state1");

  BOOST_CHECK_EQUAL(tA.getId(), "transitionA");
  BOOST_CHECK_EQUAL(tA.getPath(), fsm.getPath()+".state0.transitionA");
  BOOST_CHECK_EQUAL(tA.getEndState(), "state1");
  BOOST_CHECK_EQUAL(&tA.getStateMachine(), &fsm);
  
  BOOST_CHECK_EQUAL(fsm.getTransitions("state0").size(), size_t(1));
  BOOST_CHECK_EQUAL(fsm.getTransitions("state0").find("transitionA")->second, &tA);
  BOOST_CHECK_EQUAL(fsm.getTransitions("state1").size(), size_t(0));

  // 2) Check that exception is thrown in each error case
  BOOST_CHECK_THROW( fsm.addTransition("newId", "s", "state1"), StateNotDefined);
  BOOST_CHECK_THROW( fsm.addTransition("newId", "state1", "s"), StateNotDefined);
  BOOST_CHECK_EQUAL( fsm.getTransitions("state1").size(), size_t(0));
  fsm.addState("state2");
  BOOST_CHECK_THROW( fsm.addTransition("transitionA", "state0", "state2"), TransitionAlreadyDefined);
  BOOST_CHECK_EQUAL( fsm.getTransitions("state0").size(), size_t(1));
}


BOOST_AUTO_TEST_CASE(TestAddTransitionSteps) {
  LOG(kInfo) << "Running StateMachineTestSuite/TestAddTransitionSteps";

  DummyActionableObject obj("dummy");
  Command& cmd = obj.registerFunctionoid<DummyCommand>("aCmd");
  CommandSequence& cmdSeq = obj.registerCommandSequence("aCmdSeq", cmd);
  swatch::core::StateMachine& fsm = obj.registerStateMachine("anOp", "state0", "errState");
  fsm.addState("state1");
  
  // 1) Add transition, no steps yet
  StateMachine::Transition& tA = fsm.addTransition("transitionA", "state0", "state1");

  BOOST_CHECK_EQUAL(tA.size(), size_t(0));
  BOOST_CHECK(tA.begin() == tA.end());
  
  // 2) Add command to transition
  BOOST_CHECK_EQUAL(&(tA.add(cmd)), &tA);
  BOOST_CHECK_EQUAL(tA.size(), size_t(1));
  BOOST_CHECK_EQUAL(&tA.begin()->get(), &cmd);
  // BOOST_CHECK_EQUAL( tA.begin()->getAlias(), "");
  BOOST_CHECK_EQUAL( tA.begin()->getNamespace(), "");

  // 3) Add command to transition, using different namespace
  BOOST_CHECK_EQUAL(&(tA.add(cmd, "dummyNamespace")), &tA);
  BOOST_CHECK_EQUAL(tA.size(), size_t(2));
  BOOST_CHECK_EQUAL(&(tA.begin()+1)->get(), &cmd);
  // BOOST_CHECK_EQUAL( (tA.begin()+1)->getAlias(), "dummyAlias");
  BOOST_CHECK_EQUAL( (tA.begin()+1)->getNamespace(), "dummyNamespace");

  // 3) Add command sequence to transition
  BOOST_CHECK_EQUAL(&(tA.add(cmdSeq)), &tA);
  BOOST_CHECK_EQUAL(tA.size(), size_t(2 + cmdSeq.size()));
  CommandVec::const_iterator lIt1 = cmdSeq.begin();
  for(auto lIt2=(tA.begin()+2); lIt1!=cmdSeq.end(); lIt1++, lIt2++)
  {
    BOOST_CHECK_EQUAL(&lIt1->get(), &lIt2->get());
    // BOOST_CHECK_EQUAL(lIt1->getAlias(), lIt2->getAlias());
    BOOST_CHECK_EQUAL(lIt1->getNamespace(), lIt2->getNamespace());
  }
  
  // 4) Adding command from other object: Should throw, and leave transition unchanged
  DummyActionableObject obj2("dummy2");
  Command& cmd2 = obj2.registerFunctionoid<DummyCommand>("aCmd");
  BOOST_CHECK_THROW(tA.add(cmd2), InvalidResource);
  BOOST_CHECK_EQUAL(tA.size(), size_t(2 + cmdSeq.size()));
}



/* ---------------------------------------------------------------- */
/*   PART 2: ENGAGING & DISENGAGING                                 */
/* ---------------------------------------------------------------- */


BOOST_AUTO_TEST_CASE(TestEngageFSM) {
  LOG(kInfo) << "Running StateMachineTestSuite/TestEngageFSM";
  DummyActionableObject obj("dummy");
  const std::string initialState = "myInitialState";
  StateMachine& testFSM = obj.registerStateMachine("myFSM", initialState, "myErrorState");

  // Confirm correct construction before staring tests
  BOOST_REQUIRE_EQUAL( obj.getState().getEngagedFSM(), (const StateMachine*) NULL );
  BOOST_REQUIRE_EQUAL( obj.getState().getState(), "" );
  
  // Engaging FSM should put object into FSM's initial state
  BOOST_CHECK_NO_THROW( obj.engageStateMachine(testFSM.getId()) );
  BOOST_CHECK_EQUAL( obj.getState().getEngagedFSM(), & testFSM );
  BOOST_CHECK_EQUAL( obj.getState().getState(), initialState );

  // Trying to engage FSM again shouldn't work
  BOOST_CHECK_THROW( obj.engageStateMachine(testFSM.getId()), ResourceInWrongStateMachine );
  BOOST_CHECK_EQUAL( obj.getState().getEngagedFSM(), & testFSM );
  BOOST_CHECK_EQUAL( obj.getState().getState(), initialState );

  // Trying to engage another FSM also shouldn't work
  obj.registerStateMachine("anotherFSM", "otherInitialState", "otherErrorState");
  BOOST_CHECK_THROW( obj.engageStateMachine("anotherFSM"), ResourceInWrongStateMachine );
  BOOST_CHECK_EQUAL( obj.getState().getEngagedFSM(), & testFSM );
  BOOST_CHECK_EQUAL( obj.getState().getState(), initialState );
}


BOOST_AUTO_TEST_CASE(TestDisengageFSM) {
  LOG(kInfo) << "Running StateMachineTestSuite/TestDisengageFSM";
  DummyActionableObject obj("dummy");
  const std::string initialState = "myInitialState";
  StateMachine& testFSM = obj.registerStateMachine("myFSM", initialState, "myErrorState");

  // Confirm correct construction before testing disengage method
  BOOST_REQUIRE_EQUAL( obj.getState().getEngagedFSM(), (const StateMachine*) NULL );
  BOOST_REQUIRE_EQUAL( obj.getState().getState(), "" );
  
  // Can't disengage if FSM isn't yet engaged
  BOOST_CHECK_THROW( testFSM.disengage(), ResourceInWrongStateMachine);
  BOOST_CHECK_EQUAL( obj.getState().getEngagedFSM(), (const StateMachine*) NULL);
  BOOST_CHECK_EQUAL( obj.getState().getState(), "");
  
  // Engage FSM before continuing unit tests for disengage
  obj.engageStateMachine(testFSM.getId());
  BOOST_REQUIRE_EQUAL( obj.getState().getEngagedFSM(), & testFSM );
  BOOST_REQUIRE_EQUAL( obj.getState().getState(), initialState );

  // Disengaging another FSM: Should throw, and leave state unchanged
  StateMachine& otherFSM = obj.registerStateMachine("anotherFSM", "otherInitialState", "otherErrorState");
  BOOST_CHECK_THROW( otherFSM.disengage(), ResourceInWrongStateMachine );
  BOOST_CHECK_EQUAL( obj.getState().getEngagedFSM(), & testFSM );
  BOOST_CHECK_EQUAL( obj.getState().getState(), initialState );

  // Disengaging the engaged FSM
  BOOST_CHECK_NO_THROW( testFSM.disengage() );
  BOOST_CHECK_EQUAL( obj.getState().getEngagedFSM(), (const StateMachine*) NULL );
  BOOST_CHECK_EQUAL( obj.getState().getState(), "" );
}



/* ---------------------------------------------------------------- */
/*   PART 3: RUNNING TRANSITIONS                                    */
/* ---------------------------------------------------------------- */

BOOST_FIXTURE_TEST_CASE(TestRunTransitionDisengagedFSM, StateMachineTestSetup) {
  LOG(kInfo) << "Running StateMachineTestSuite/TestRunTransitionDisengagedFSM";
  DummyGateKeeper gk;

  // Running transition before FSM is engaged: should throw, and leave state/TransitionStatus unchanged
  BOOST_CHECK_THROW( transitionItoA->exec(gk, false), ResourceInWrongState);
  BOOST_CHECK_EQUAL( transitionItoA->getState(), Functionoid::kInitial);
  BOOST_CHECK_EQUAL( transitionItoA->getStatus().getState(), Functionoid::kInitial);
  BOOST_CHECK_EQUAL( obj->getState().getEngagedFSM(), (const StateMachine*) NULL);
  BOOST_CHECK_EQUAL( obj->getState().getState(), "");

  // Running transition with other FSM engaged: should throw, and leave state/TransitionStatus unchanged
  const StateMachine& otherFSM = obj->registerStateMachine("anotherFSM", fsmState0, fsmStateError);
  obj->engageStateMachine("anotherFSM");
  BOOST_REQUIRE_EQUAL( obj->getState().getEngagedFSM(), & otherFSM );
  BOOST_CHECK_THROW( transitionItoA->exec(gk, false), ResourceInWrongState );
  BOOST_CHECK_EQUAL( transitionItoA->getState(), Functionoid::kInitial);
  BOOST_CHECK_EQUAL( transitionItoA->getStatus().getState(), Functionoid::kInitial);
  BOOST_CHECK_EQUAL( obj->getState().getEngagedFSM(), & otherFSM);
  BOOST_CHECK_EQUAL( obj->getState().getState(), fsmState0);
}


BOOST_FIXTURE_TEST_CASE(TestRunEmptyTransitions, StateMachineTestSetup) {
  LOG(kInfo) << "Running StateMachineTestSuite/TestRunEmptyTransitions";
  DummyGateKeeper gk;

  // Engage state machine and confirm initial state to prepare for testing
  BOOST_REQUIRE_NO_THROW( obj->engageStateMachine(testFSM.getId()) );
  BOOST_REQUIRE_EQUAL( obj->getState().getEngagedFSM(), & testFSM );
  BOOST_REQUIRE_EQUAL( obj->getState().getState(), fsmState0 );
  
  // Check that invalid transitions not executed
  BOOST_CHECK_THROW( transitionAtoB->exec(gk, false), ResourceInWrongState);
  BOOST_CHECK_EQUAL( transitionAtoB->getState(), Functionoid::kInitial);
  BOOST_CHECK_EQUAL( transitionAtoB->getStatus().getState(), Functionoid::kInitial);
  BOOST_CHECK_EQUAL( obj->getState().getState(), fsmState0 );

  BOOST_CHECK_THROW( transitionBtoI->exec(gk, false), ResourceInWrongState);
  BOOST_CHECK_EQUAL( transitionBtoI->getState(), Functionoid::kInitial);
  BOOST_CHECK_EQUAL( transitionBtoI->getStatus().getState(), Functionoid::kInitial);
  BOOST_CHECK_EQUAL( obj->getState().getState(), fsmState0 );

  // Execute state transition: 'initial' -> A
  BOOST_CHECK_NO_THROW( transitionItoA->exec(gk, false) );
  BOOST_CHECK_EQUAL( obj->getState().getState(), fsmStateA );
  BOOST_CHECK_EQUAL( transitionItoA->getState(), Functionoid::kDone );
  StateMachine::TransitionStatus s = transitionItoA->getStatus();
  BOOST_CHECK_EQUAL( s.getActionPath(), transitionItoA->getPath() );
  BOOST_CHECK( s.getCommandStatus().empty());
  BOOST_CHECK_EQUAL( s.getNumberOfCompletedCommands(), size_t(0));
  BOOST_CHECK_EQUAL( s.getProgress(), 1.0);
  BOOST_CHECK( s.getResults().empty() );
  BOOST_CHECK( s.getRunningTime() >= 0.0 );
  BOOST_CHECK_EQUAL( s.getState(), Functionoid::kDone);
  BOOST_CHECK_EQUAL( s.getTotalNumberOfCommands(), size_t(0));

  // Check that invalid transitions not executed
  BOOST_CHECK_THROW( transitionItoA->exec(gk, false), ResourceInWrongState);
  BOOST_CHECK_EQUAL( obj->getState().getState(), fsmStateA );

  BOOST_CHECK_THROW( transitionBtoI->exec(gk, false), ResourceInWrongState);
  BOOST_CHECK_EQUAL( transitionBtoI->getStatus().getState(), Functionoid::kInitial);
  BOOST_CHECK_EQUAL( transitionBtoI->getState(), Functionoid::kInitial);
  BOOST_CHECK_EQUAL( obj->getState().getState(), fsmStateA );

  // Execute state transition: A -> B
  BOOST_CHECK_NO_THROW( transitionAtoB->exec(gk, false) );
  BOOST_CHECK_EQUAL( obj->getState().getState(), fsmStateB );
  s = transitionAtoB->getStatus();
  BOOST_CHECK_EQUAL( s.getActionPath(), transitionAtoB->getPath() );
  BOOST_CHECK( s.getCommandStatus().empty());
  BOOST_CHECK_EQUAL( s.getNumberOfCompletedCommands(), size_t(0));
  BOOST_CHECK_EQUAL( s.getProgress(), 1.0);
  BOOST_CHECK( s.getResults().empty() );
  BOOST_CHECK( s.getRunningTime() >= 0.0 );
  BOOST_CHECK_EQUAL( s.getState(), Functionoid::kDone);
  BOOST_CHECK_EQUAL( s.getTotalNumberOfCommands(), size_t(0));
}


BOOST_FIXTURE_TEST_CASE(TestTransitionMissingParams, StateMachineTestSetup) {
  LOG(kInfo) << "Running StateMachineTestSuite/TestTransitionMissingParams";
  DummyGateKeeper emptyGk;
  transitionItoA->add(cmdNormal1);
  transitionItoA->add(cmdSeq);

  // Engage state machine, and confirm object's state is ready for following tests
  obj->engageStateMachine( testFSM.getId() );
  BOOST_REQUIRE_EQUAL( obj->getState().getEngagedFSM(), & testFSM);
  BOOST_REQUIRE_EQUAL( obj->getState().getState(), fsmState0);
  
  // Running transition without parameter values defined: Should throw, and leave state unchanged
  swatch::logger::LogLevel lThr = swatch::logger::Log::logThreshold();
  swatch::logger::Log::setLogThreshold(swatch::logger::kFatal);
  BOOST_CHECK_THROW( transitionItoA->exec(emptyGk, false), ParameterNotFound );
  swatch::logger::Log::setLogThreshold(lThr);
  BOOST_CHECK_EQUAL( transitionItoA->getState(), Functionoid::kInitial );
  BOOST_CHECK_EQUAL( transitionItoA->getStatus().getState(), Functionoid::kInitial );
  BOOST_CHECK_EQUAL( obj->getState().getEngagedFSM(), & testFSM);
  BOOST_CHECK_EQUAL( obj->getState().getState(), fsmState0);

  // checkMissingParams method should return correct parameters 
  typedef CommandVec::MissingParam tMissingParam;
  std::vector<tMissingParam> result, expected;
  expected.push_back(tMissingParam("", cmdNormal1.getId(), DummyCommand::paramToDo));
  expected.push_back(tMissingParam("", cmdNormal1.getId(), DummyCommand::paramX));
  expected.push_back(tMissingParam(cmdSeq.getId(), cmdNormal1.getId(), DummyCommand::paramToDo));
  expected.push_back(tMissingParam(cmdSeq.getId(), cmdNormal1.getId(), DummyCommand::paramX));
  expected.push_back(tMissingParam(cmdSeq.getId(), cmdNormal2.getId(), DummyCommand::paramToDo));
  expected.push_back(tMissingParam(cmdSeq.getId(), cmdNormal2.getId(), DummyCommand::paramX));

  std::vector<ReadOnlyXParameterSet> paramSets;
  transitionItoA->checkForMissingParameters(emptyGk, paramSets, result);
  BOOST_CHECK_EQUAL_COLLECTIONS(result.begin(), result.end(), expected.begin(), expected.end());
  BOOST_REQUIRE_EQUAL( paramSets.size(), size_t(3) );
}


BOOST_FIXTURE_TEST_CASE(TestRunErrorTransition, StateMachineTestSetup) {
  LOG(kInfo) << "Running StateMachineTestSuite/TestRunErrorTransition";
  transitionItoA->add(cmdNormal1);
  transitionItoA->add(cmdError);
  transitionItoA->add(cmdNormal2);
  
  // Engage state machine, and confirm object's state is ready for following tests
  obj->engageStateMachine( testFSM.getId() );
  BOOST_REQUIRE_EQUAL( obj->getState().getEngagedFSM(), & testFSM);
  BOOST_REQUIRE_EQUAL( obj->getState().getState(), fsmState0);  

  // Running transition with error command: Object should go to error state, and subsequent commands shouldn't run
  BOOST_REQUIRE_NO_THROW(transitionItoA->exec(gk, false));
  BOOST_CHECK_EQUAL(obj->getState().getEngagedFSM(), & testFSM);
  BOOST_CHECK_EQUAL(obj->getState().getState(), fsmStateError);
  BOOST_CHECK_EQUAL(transitionItoA->getState(), Functionoid::kError);

  StateMachine::TransitionStatus s = transitionItoA->getStatus();
  BOOST_CHECK_EQUAL(s.getActionPath(), transitionItoA->getPath());
  BOOST_CHECK_EQUAL(s.getNumberOfCompletedCommands(), size_t(2));
  BOOST_CHECK_EQUAL(s.getTotalNumberOfCommands(), size_t(3));
  BOOST_CHECK_CLOSE(s.getProgress(), (1.0 + DummyErrorCommand::finalProgress)/3.0, 0.0001);
  BOOST_CHECK_LT(s.getProgress(), 0.66);
  BOOST_CHECK_GT(s.getRunningTime(), 0.0);
  BOOST_CHECK_EQUAL(s.getState(), Functionoid::kError);
  BOOST_REQUIRE_EQUAL(s.getCommandStatus().size(), size_t(2));
  BOOST_CHECK_EQUAL(s.getCommandStatus().at(0).getActionPath(), cmdNormal1.getPath());
  BOOST_CHECK_EQUAL(s.getCommandStatus().at(0).getState(), Functionoid::kDone);
  BOOST_CHECK_EQUAL(s.getCommandStatus().at(1).getActionPath(), cmdError.getPath());
  BOOST_CHECK_EQUAL(s.getCommandStatus().at(1).getState(), Functionoid::kError);
  
  BOOST_REQUIRE_EQUAL(s.getResults().size(), size_t(2));
  BOOST_CHECK_EQUAL(s.getResults().at(0) , cmdNormal1.getStatus().getResult() );
  BOOST_CHECK_EQUAL(s.getResults().at(1) , cmdError.getStatus().getResult() );

  BOOST_CHECK_EQUAL(cmdNormal2.getState(), Functionoid::kInitial);
  BOOST_CHECK_EQUAL(cmdNormal2.getStatus().getState(), Functionoid::kInitial);
}


BOOST_FIXTURE_TEST_CASE(TestRunWarningTransition, StateMachineTestSetup) {
  LOG(kInfo) << "Running StateMachineTestSuite/TestRunWarningTransition";
  transitionItoA->add(cmdNormal1);
  transitionItoA->add(cmdWarning);
  transitionItoA->add(cmdNormal2);
  
  // Engage state machine, and confirm object's state is ready for following tests
  obj->engageStateMachine( testFSM.getId() );
  BOOST_REQUIRE_EQUAL( obj->getState().getEngagedFSM(), & testFSM);
  BOOST_REQUIRE_EQUAL( obj->getState().getState(), fsmState0);  

  // Running transition with warning command: Object should go to state A, and all commands shouldt run
  BOOST_REQUIRE_NO_THROW(transitionItoA->exec(gk, false));
  BOOST_CHECK_EQUAL(obj->getState().getEngagedFSM(), & testFSM);
  BOOST_CHECK_EQUAL(obj->getState().getState(), fsmStateA);
  BOOST_CHECK_EQUAL(transitionItoA->getState(), Functionoid::kWarning);

  StateMachine::TransitionStatus s = transitionItoA->getStatus();
  BOOST_CHECK_EQUAL(s.getActionPath(), transitionItoA->getPath());
  BOOST_CHECK_EQUAL(s.getNumberOfCompletedCommands(), size_t(3));
  BOOST_CHECK_EQUAL(s.getTotalNumberOfCommands(), size_t(3));
  BOOST_CHECK_EQUAL(s.getProgress(), 1.0);
  BOOST_CHECK_EQUAL(s.getState(), Functionoid::kWarning);
  BOOST_REQUIRE_EQUAL(s.getCommandStatus().size(), size_t(3));
  BOOST_CHECK_EQUAL(s.getCommandStatus().at(0).getActionPath(), cmdNormal1.getPath());
  BOOST_CHECK_EQUAL(s.getCommandStatus().at(0).getState(), Functionoid::kDone);
  BOOST_CHECK_EQUAL(s.getCommandStatus().at(1).getActionPath(), cmdWarning.getPath());
  BOOST_CHECK_EQUAL(s.getCommandStatus().at(1).getState(), Functionoid::kWarning);
  BOOST_CHECK_EQUAL(s.getCommandStatus().at(2).getActionPath(), cmdNormal2.getPath());
  BOOST_CHECK_EQUAL(s.getCommandStatus().at(2).getState(), Functionoid::kDone);

  BOOST_REQUIRE_EQUAL(s.getResults().size(), size_t(3));
  BOOST_CHECK_EQUAL(s.getResults().at(0) , cmdNormal1.getStatus().getResult() );
  BOOST_CHECK_EQUAL(s.getResults().at(1) , cmdWarning.getStatus().getResult() );
  BOOST_CHECK_EQUAL(s.getResults().at(2) , cmdNormal2.getStatus().getResult() );
}


BOOST_FIXTURE_TEST_CASE(TestRunGoodTransition, StateMachineTestSetup) {
  LOG(kInfo) << "Running StateMachineTestSuite/TestRunGoodTransition";
  transitionItoA->add(cmdNormal1);
  transitionItoA->add(cmdNormal2);
  
  // Engage state machine, and confirm object's state is ready for following tests
  obj->engageStateMachine( testFSM.getId() );
  BOOST_REQUIRE_EQUAL( obj->getState().getEngagedFSM(), & testFSM);
  BOOST_REQUIRE_EQUAL( obj->getState().getState(), fsmState0);  

  // Running transition with warning command: Object should go to state A, and all commands shouldt run
  BOOST_REQUIRE_NO_THROW(transitionItoA->exec(gk, false));
  BOOST_CHECK_EQUAL(obj->getState().getEngagedFSM(), & testFSM);
  BOOST_CHECK_EQUAL(obj->getState().getState(), fsmStateA);
  BOOST_CHECK_EQUAL(transitionItoA->getState(), Functionoid::kDone);

  StateMachine::TransitionStatus s = transitionItoA->getStatus();
  BOOST_CHECK_EQUAL(s.getActionPath(), transitionItoA->getPath());
  BOOST_CHECK_EQUAL(s.getNumberOfCompletedCommands(), size_t(2));
  BOOST_CHECK_EQUAL(s.getTotalNumberOfCommands(), size_t(2));
  BOOST_CHECK_EQUAL(s.getProgress(), 1.0);
  BOOST_CHECK_EQUAL(s.getState(), Functionoid::kDone);
  BOOST_REQUIRE_EQUAL(s.getCommandStatus().size(), size_t(2));
  BOOST_CHECK_EQUAL(s.getCommandStatus().at(0).getActionPath(), cmdNormal1.getPath());
  BOOST_CHECK_EQUAL(s.getCommandStatus().at(0).getState(), Functionoid::kDone);
  BOOST_CHECK_EQUAL(s.getCommandStatus().at(1).getActionPath(), cmdNormal2.getPath());
  BOOST_CHECK_EQUAL(s.getCommandStatus().at(1).getState(), Functionoid::kDone);

  BOOST_REQUIRE_EQUAL(s.getResults().size(), size_t(2));
  BOOST_CHECK_EQUAL(s.getResults().at(0) , cmdNormal1.getStatus().getResult() );
  BOOST_CHECK_EQUAL(s.getResults().at(1) , cmdNormal2.getStatus().getResult() );
}



/* ---------------------------------------------------------------- */
/*   PART 4: RESETTING                                              */
/* ---------------------------------------------------------------- */


BOOST_FIXTURE_TEST_CASE(TestResetFSM, StateMachineTestSetup) {
  LOG(kInfo) << "Running StateMachineTestSuite/TestResetFSM";

  // Engage state machine and confirm initial state to prepare for following tests
  BOOST_REQUIRE_NO_THROW( obj->engageStateMachine(testFSM.getId()) );
  BOOST_REQUIRE_EQUAL( obj->getState().getEngagedFSM(), & testFSM );
  BOOST_REQUIRE_EQUAL( obj->getState().getState(), fsmState0 );
  
  // Reset state machine
  BOOST_CHECK_NO_THROW( testFSM.reset() );
  BOOST_CHECK_EQUAL( obj->getState().getEngagedFSM(), &testFSM );
  BOOST_CHECK_EQUAL( obj->getState().getState(), fsmState0 );
  
  // Run a transition to prepare for following tests
  BOOST_REQUIRE_NO_THROW( transitionItoA->exec(DummyGateKeeper(), false) );
  BOOST_REQUIRE_EQUAL( obj->getState().getEngagedFSM(), & testFSM );
  BOOST_REQUIRE_EQUAL( obj->getState().getState(), fsmStateA );
  
  // Resetting another FSM: Should throw, and leave state unchanged
  StateMachine& otherFSM = obj->registerStateMachine("anotherFSM", "otherInitialState", "otherErrorState");
  BOOST_CHECK_THROW(otherFSM.reset(), ResourceInWrongStateMachine);
  BOOST_CHECK_EQUAL( obj->getState().getEngagedFSM(), & testFSM );
  BOOST_CHECK_EQUAL( obj->getState().getState(), fsmStateA );

  // Reset correct state machine
  BOOST_CHECK_NO_THROW( testFSM.reset() );
  BOOST_CHECK_EQUAL( obj->getState().getEngagedFSM(), & testFSM );
  BOOST_CHECK_EQUAL( obj->getState().getState(), fsmState0 );
}


BOOST_FIXTURE_TEST_CASE(TestResetDisengagedFSM, StateMachineTestSetup) {
  LOG(kInfo) << "Running StateMachineTestSuite/TestResetDisengagedFSM";

  // Resetting FSM before engaged: Should throw, and leave state unchanged
  BOOST_CHECK_THROW( testFSM.reset(), ResourceInWrongStateMachine);
  BOOST_CHECK_EQUAL( obj->getState().getEngagedFSM(), (const StateMachine*) NULL );
  BOOST_CHECK_EQUAL( obj->getState().getState(), "" );
}

BOOST_AUTO_TEST_SUITE_END() // StateMachineTestSuite
} /* namespace test */
} /* namespace core */
} /* namespace swatch */
