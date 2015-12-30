
#include <boost/test/unit_test.hpp>


// SWATCH headers
#include "swatch/core/CommandSequence.hpp"
#include "swatch/core/StateMachine.hpp"
#include "swatch/core/test/DummyActionableObject.hpp"
#include "swatch/core/test/DummyCommand.hpp"
#include "swatch/core/test/DummyGateKeeper.hpp"
#include "swatch/core/test/DummyMaskableObject.hpp"
#include "swatch/logger/Log.hpp"


using namespace swatch::logger;
namespace swatch {
namespace core {
namespace test {

  struct StateMachineTestSetup {
    StateMachineTestSetup() :
      obj(new DummyActionableObject("dummyObj"), ActionableObject::Deleter()),
      child1(obj->addMonitorable(new DummyActionableObject::MonChild("child1", *obj))),
      grandChild1(child1.addMonitorable(new DummyActionableObject::MonChild("grandChild1", *obj))),
      grandChild2(child1.addMonitorable(new DummyActionableObject::MonChild("grandChild2", *obj))),
      maskableA(obj->addMonitorable(new DummyMaskableObject("maskableA"))),
      maskableB(child1.addMonitorable(new DummyMaskableObject("maskableB"))),
      maskableC(child1.addMonitorable(new DummyMaskableObject("maskableC"))),
      cmdNormal1( obj->registerCommand<DummyCommand>("cmdNormal1") ),
      cmdNormal2( obj->registerCommand<DummyCommand>("cmdNormal2") ),
      cmdWarning( obj->registerCommand<DummyWarningCommand>("cmdWarning") ),
      cmdError( obj->registerCommand<DummyErrorCommand>("cmdError") ),
      cmdSeq( obj->registerSequence(cmdSeqId, cmdNormal1).then(cmdNormal2) ),
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

      // for child1
      GateKeeper::tSettingsTable settings_child1(new GateKeeper::tMonitoringSettings());
      GateKeeper::tMonitoringSetting mon_setting1(new MonitoringSetting("child1", monitoring::kNonCritical));
      GateKeeper::tMonitoringSetting mon_setting2(new MonitoringSetting("child1.grandChild1", monitoring::kDisabled));
      GateKeeper::tMonitoringSetting mon_setting3(new MonitoringSetting("grandChild2", monitoring::kDisabled));
      // for child1 metric
      GateKeeper::tMonitoringSetting mon_setting4(new MonitoringSetting("child1.dummyMetric", monitoring::kDisabled));

      settings_child1->insert(GateKeeper::tMonitoringSettings::value_type(fsmStateB + ".child1", mon_setting1));
      settings_child1->insert(GateKeeper::tMonitoringSettings::value_type(fsmStateB + ".child1.grandChild1", mon_setting2));
      // this one should fail, as paths need to be relative to obj!
      settings_child1->insert(GateKeeper::tMonitoringSettings::value_type(fsmStateB + ".grandChild2", mon_setting3));
      // metric
      settings_child1->insert(GateKeeper::tMonitoringSettings::value_type(fsmStateB + ".child1.dummyMetric", mon_setting4));
      gk.addSettingsTable("common", settings_child1);
      
      GateKeeper::MasksTable_t lMasksTable(new GateKeeper::Masks_t());
      lMasksTable->insert("maskableA");
      lMasksTable->insert("child1.maskableB");
      // "maskableC": FALSE ENTRY (i.e. should not take effect on child1.maskableC)
      //   - here to check that masks are applied using MaskableObject's ID path relative to the ActionableObject (not just using ID string)
      lMasksTable->insert("maskableC");
      
      gk.addMasksTable("common", lMasksTable);
    }
    
    ~StateMachineTestSetup() {}
    
    boost::shared_ptr<DummyActionableObject> obj;
    DummyActionableObject::MonChild& child1;
    DummyActionableObject::MonChild& grandChild1, &grandChild2;
    DummyMaskableObject& maskableA, &maskableB, &maskableC;
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
  StateMachine& fsm = obj.registerStateMachine("anOp", "someState", "myErrState");

  BOOST_CHECK_EQUAL(fsm.getId(), "anOp");
  BOOST_CHECK_EQUAL(fsm.getPath(), obj.getId()+"."+fsm.getId());
  BOOST_CHECK_EQUAL(&obj.getStateMachine(fsm.getId()), &fsm);
  BOOST_CHECK_EQUAL(&obj.getObj(fsm.getId()), (Object*) &fsm);

  BOOST_CHECK_EQUAL(&fsm.getActionable(), &obj);
  BOOST_CHECK_EQUAL(fsm.getInitialState(), "someState");
  BOOST_CHECK_EQUAL(fsm.getErrorState(), "myErrState");
  
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
  Command& cmd = obj.registerCommand<DummyCommand>("aCmd");
  CommandSequence& cmdSeq = obj.registerSequence("aCmdSeq", cmd);
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
  Command& cmd2 = obj2.registerCommand<DummyCommand>("aCmd");
  BOOST_CHECK_THROW(tA.add(cmd2), InvalidResource);
  BOOST_CHECK_EQUAL(tA.size(), size_t(2 + cmdSeq.size()));
}



/* ---------------------------------------------------------------- */
/*   PART 2: ENGAGING & DISENGAGING                                 */
/* ---------------------------------------------------------------- */


BOOST_FIXTURE_TEST_CASE(TestEngageFSM, StateMachineTestSetup)
{
  LOG(kInfo) << "Running StateMachineTestSuite/TestEngageFSM";

  // Confirm correct construction before staring tests
  BOOST_REQUIRE_EQUAL( obj->getStatus().getStateMachineId(), ActionableStatus::kNullStateMachineId );
  BOOST_REQUIRE_EQUAL( obj->getStatus().getState(), ActionableStatus::kNullStateId );

  // Engaging FSM should put object into FSM's initial state
  BOOST_CHECK_NO_THROW( testFSM.engage(gk) );
  BOOST_CHECK_EQUAL( obj->getStatus().getStateMachineId(), testFSM.getId() );
  BOOST_CHECK_EQUAL( obj->getStatus().getState(), fsmState0 );

  // Reset MaskableObjects before continuing tests
  maskableA.setMasked(false);
  maskableB.setMasked(false);
  maskableC.setMasked(false);

  // Trying to engage FSM again shouldn't work
  BOOST_CHECK_THROW( testFSM.engage(gk), ResourceInWrongStateMachine );
  BOOST_CHECK_EQUAL( obj->getStatus().getStateMachineId(), testFSM.getId() );
  BOOST_CHECK_EQUAL( obj->getStatus().getState(), fsmState0 );
  BOOST_CHECK_EQUAL(maskableA.isMasked(), false);
  BOOST_CHECK_EQUAL(maskableB.isMasked(), false);
  BOOST_CHECK_EQUAL(maskableC.isMasked(), false);

  // Trying to engage another FSM also shouldn't work
  StateMachine& otherFSM = obj->registerStateMachine("anotherFSM", "otherInitialState", "otherErrorState");
  BOOST_CHECK_THROW( otherFSM.engage(gk), ResourceInWrongStateMachine );
  BOOST_CHECK_EQUAL( obj->getStatus().getStateMachineId(), testFSM.getId() );
  BOOST_CHECK_EQUAL( obj->getStatus().getState(), fsmState0 );
  BOOST_CHECK_EQUAL(maskableA.isMasked(), false);
  BOOST_CHECK_EQUAL(maskableB.isMasked(), false);
  BOOST_CHECK_EQUAL(maskableC.isMasked(), false);
}


BOOST_AUTO_TEST_CASE(TestDisengageFSM) {
  LOG(kInfo) << "Running StateMachineTestSuite/TestDisengageFSM";
  DummyActionableObject obj("dummy");
  const std::string initialState = "myInitialState";
  StateMachine& testFSM = obj.registerStateMachine("myFSM", initialState, "myErrorState");

  // Confirm correct construction before testing disengage method
  BOOST_REQUIRE_EQUAL( obj.getStatus().getStateMachineId(), ActionableStatus::kNullStateMachineId );
  BOOST_REQUIRE_EQUAL( obj.getStatus().getState(), ActionableStatus::kNullStateId );
  
  // Can't disengage if FSM isn't yet engaged
  BOOST_CHECK_THROW( testFSM.disengage(), ResourceInWrongStateMachine);
  BOOST_CHECK_EQUAL( obj.getStatus().getStateMachineId(), ActionableStatus::kNullStateMachineId);
  BOOST_CHECK_EQUAL( obj.getStatus().getState(), ActionableStatus::kNullStateId);
  
  // Engage FSM before continuing unit tests for disengage
  testFSM.engage(DummyGateKeeper());
  BOOST_REQUIRE_EQUAL( obj.getStatus().getStateMachineId(), testFSM.getId() );
  BOOST_REQUIRE_EQUAL( obj.getStatus().getState(), initialState );

  // Disengaging another FSM: Should throw, and leave state unchanged
  StateMachine& otherFSM = obj.registerStateMachine("anotherFSM", "otherInitialState", "otherErrorState");
  BOOST_CHECK_THROW( otherFSM.disengage(), ResourceInWrongStateMachine );
  BOOST_CHECK_EQUAL( obj.getStatus().getStateMachineId(), testFSM.getId() );
  BOOST_CHECK_EQUAL( obj.getStatus().getState(), initialState );

  // Disengaging the engaged FSM
  BOOST_CHECK_NO_THROW( testFSM.disengage() );
  BOOST_CHECK_EQUAL( obj.getStatus().getStateMachineId(), ActionableStatus::kNullStateMachineId );
  BOOST_CHECK_EQUAL( obj.getStatus().getState(), ActionableStatus::kNullStateId );
}


BOOST_FIXTURE_TEST_CASE(TestDescendantsMaskedDuringEngage, StateMachineTestSetup)
{
  LOG(kInfo) << "Running StateMachineTestSuite/TestDescendantsMaskedDuringEngage";

  // PART A :
  //  * Goal: Check that MaskableObjects we ask it to mask are being masked
  //  * Start off with everything unmasked

  // A,1) Require that all are unmasked before tests start
  BOOST_REQUIRE_EQUAL(maskableA.isMasked(), false);
  BOOST_REQUIRE_EQUAL(maskableB.isMasked(), false);
  BOOST_REQUIRE_EQUAL(maskableC.isMasked(), false);
  // A.2) Engage FSM; check that A & B now masked, but C remains unmasked
  testFSM.engage(gk);
  BOOST_CHECK_EQUAL(maskableA.isMasked(), true);
  BOOST_CHECK_EQUAL(maskableB.isMasked(), true);
  BOOST_CHECK_EQUAL(maskableC.isMasked(), false);

  // PART B :
  //   * Goal: Check that all MaskableObjects we don't ask it to mask are being reset to unmasked state
  //   * Start off with A & B unmasked, but C masked
  // B.1) Disengage, unmask A & B, mask C; require that starting assumptions are correct before testing
  testFSM.disengage();
  maskableA.setMasked(false);
  maskableB.setMasked(false);
  maskableC.setMasked(true);
  BOOST_REQUIRE_EQUAL(maskableA.isMasked(), false);
  BOOST_REQUIRE_EQUAL(maskableB.isMasked(), false);
  BOOST_REQUIRE_EQUAL(maskableC.isMasked(), true);
  
  // B.2) Engage FSM; check that A & B now masked, but C remains unmasked
  testFSM.engage(gk);
  BOOST_CHECK_EQUAL(maskableA.isMasked(), true);
  BOOST_CHECK_EQUAL(maskableB.isMasked(), true);
  BOOST_CHECK_EQUAL(maskableC.isMasked(), false);
}



/* ---------------------------------------------------------------- */
/*   PART 3: RUNNING TRANSITIONS                                    */
/* ---------------------------------------------------------------- */

BOOST_FIXTURE_TEST_CASE(TestRunTransitionDisengagedFSM, StateMachineTestSetup) {
  LOG(kInfo) << "Running StateMachineTestSuite/TestRunTransitionDisengagedFSM";
  DummyGateKeeper lEmptyGK;
  
  // Running transition before FSM is engaged: should throw, and leave state/TransitionStatus unchanged
  BOOST_CHECK_THROW( transitionItoA->exec(lEmptyGK, false), ResourceInWrongState);
  BOOST_CHECK_EQUAL( transitionItoA->getState(), Functionoid::State::kInitial);
  BOOST_CHECK_EQUAL( transitionItoA->getStatus().getState(), Functionoid::State::kInitial);
  BOOST_CHECK_EQUAL( obj->getStatus().getStateMachineId(), ActionableStatus::kNullStateMachineId);
  BOOST_CHECK_EQUAL( obj->getStatus().getState(), ActionableStatus::kNullStateId);

  // Running transition with other FSM engaged: should throw, and leave state/TransitionStatus unchanged
  StateMachine& otherFSM = obj->registerStateMachine("anotherFSM", fsmState0, fsmStateError);
  otherFSM.engage(lEmptyGK);
  BOOST_REQUIRE_EQUAL( obj->getStatus().getStateMachineId(), otherFSM.getId() );
  BOOST_CHECK_THROW( transitionItoA->exec(lEmptyGK, false), ResourceInWrongState );
  BOOST_CHECK_EQUAL( transitionItoA->getState(), Functionoid::State::kInitial);
  BOOST_CHECK_EQUAL( transitionItoA->getStatus().getState(), Functionoid::State::kInitial);
  BOOST_CHECK_EQUAL( obj->getStatus().getStateMachineId(), otherFSM.getId());
  BOOST_CHECK_EQUAL( obj->getStatus().getState(), fsmState0);
}


BOOST_FIXTURE_TEST_CASE(TestRunEmptyTransitions, StateMachineTestSetup) {
  LOG(kInfo) << "Running StateMachineTestSuite/TestRunEmptyTransitions";
  DummyGateKeeper emptyGK;

  // Engage state machine and confirm initial state to prepare for testing
  BOOST_REQUIRE_NO_THROW( testFSM.engage(emptyGK) );
  BOOST_REQUIRE_EQUAL( obj->getStatus().getStateMachineId(), testFSM.getId() );
  BOOST_REQUIRE_EQUAL( obj->getStatus().getState(), fsmState0 );
  
  // Check that invalid transitions not executed
  BOOST_CHECK_THROW( transitionAtoB->exec(emptyGK, false), ResourceInWrongState);
  BOOST_CHECK_EQUAL( transitionAtoB->getState(), Functionoid::State::kInitial);
  BOOST_CHECK_EQUAL( transitionAtoB->getStatus().getState(), Functionoid::State::kInitial);
  BOOST_CHECK_EQUAL( obj->getStatus().getState(), fsmState0 );

  BOOST_CHECK_THROW( transitionBtoI->exec(emptyGK, false), ResourceInWrongState);
  BOOST_CHECK_EQUAL( transitionBtoI->getState(), Functionoid::State::kInitial);
  BOOST_CHECK_EQUAL( transitionBtoI->getStatus().getState(), Functionoid::State::kInitial);
  BOOST_CHECK_EQUAL( obj->getStatus().getState(), fsmState0 );

  // Execute state transition: 'initial' -> A
  BOOST_CHECK_NO_THROW( transitionItoA->exec(emptyGK, false) );
  BOOST_CHECK_EQUAL( obj->getStatus().getState(), fsmStateA );
  BOOST_CHECK_EQUAL( transitionItoA->getState(), Functionoid::State::kDone );
  StateMachine::TransitionStatus s = transitionItoA->getStatus();
  BOOST_CHECK_EQUAL( s.getActionPath(), transitionItoA->getPath() );
  BOOST_CHECK( s.getCommandStatus().empty());
  BOOST_CHECK_EQUAL( s.getNumberOfCompletedCommands(), size_t(0));
  BOOST_CHECK_EQUAL( s.getProgress(), 1.0);
  BOOST_CHECK( s.getResults().empty() );
  BOOST_CHECK( s.getRunningTime() >= 0.0 );
  BOOST_CHECK_EQUAL( s.getState(), Functionoid::State::kDone);
  BOOST_CHECK_EQUAL( s.getTotalNumberOfCommands(), size_t(0));

  // Check that invalid transitions not executed
  BOOST_CHECK_THROW( transitionItoA->exec(emptyGK, false), ResourceInWrongState);
  BOOST_CHECK_EQUAL( obj->getStatus().getState(), fsmStateA );

  BOOST_CHECK_THROW( transitionBtoI->exec(emptyGK, false), ResourceInWrongState);
  BOOST_CHECK_EQUAL( transitionBtoI->getStatus().getState(), Functionoid::State::kInitial);
  BOOST_CHECK_EQUAL( transitionBtoI->getState(), Functionoid::State::kInitial);
  BOOST_CHECK_EQUAL( obj->getStatus().getState(), fsmStateA );

  // Execute state transition: A -> B
  BOOST_CHECK_NO_THROW( transitionAtoB->exec(emptyGK, false) );
  BOOST_CHECK_EQUAL( obj->getStatus().getState(), fsmStateB );
  s = transitionAtoB->getStatus();
  BOOST_CHECK_EQUAL( s.getActionPath(), transitionAtoB->getPath() );
  BOOST_CHECK( s.getCommandStatus().empty());
  BOOST_CHECK_EQUAL( s.getNumberOfCompletedCommands(), size_t(0));
  BOOST_CHECK_EQUAL( s.getProgress(), 1.0);
  BOOST_CHECK( s.getResults().empty() );
  BOOST_CHECK( s.getRunningTime() >= 0.0 );
  BOOST_CHECK_EQUAL( s.getState(), Functionoid::State::kDone);
  BOOST_CHECK_EQUAL( s.getTotalNumberOfCommands(), size_t(0));
}


BOOST_FIXTURE_TEST_CASE(TestTransitionMissingParams, StateMachineTestSetup) {
  LOG(kInfo) << "Running StateMachineTestSuite/TestTransitionMissingParams";
  DummyGateKeeper lEmptyGK;
  transitionItoA->add(cmdNormal1);
  transitionItoA->add(cmdSeq);

  // Engage state machine, and confirm object's state is ready for following tests
  testFSM.engage(lEmptyGK);
  BOOST_REQUIRE_EQUAL( obj->getStatus().getStateMachineId(), testFSM.getId());
  BOOST_REQUIRE_EQUAL( obj->getStatus().getState(), fsmState0);
  
  // Running transition without parameter values defined: Should throw, and leave state unchanged
  swatch::logger::LogLevel lThr = swatch::logger::Log::logThreshold();
  swatch::logger::Log::setLogThreshold(swatch::logger::kFatal);
  BOOST_CHECK_THROW( transitionItoA->exec(lEmptyGK, false), ParameterNotFound );
  swatch::logger::Log::setLogThreshold(lThr);
  BOOST_CHECK_EQUAL( transitionItoA->getState(), Functionoid::State::kInitial );
  BOOST_CHECK_EQUAL( transitionItoA->getStatus().getState(), Functionoid::State::kInitial );
  BOOST_CHECK_EQUAL( obj->getStatus().getStateMachineId(), testFSM.getId());
  BOOST_CHECK_EQUAL( obj->getStatus().getState(), fsmState0);

  // checkMissingParams method should return correct parameters 
  typedef CommandVec::MissingParam MissingParam_t;
  std::vector<MissingParam_t> result, expected;
  expected.push_back(MissingParam_t("", cmdNormal1.getId(), DummyCommand::paramToDo));
  expected.push_back(MissingParam_t("", cmdNormal1.getId(), DummyCommand::paramX));
  expected.push_back(MissingParam_t(cmdSeq.getId(), cmdNormal1.getId(), DummyCommand::paramToDo));
  expected.push_back(MissingParam_t(cmdSeq.getId(), cmdNormal1.getId(), DummyCommand::paramX));
  expected.push_back(MissingParam_t(cmdSeq.getId(), cmdNormal2.getId(), DummyCommand::paramToDo));
  expected.push_back(MissingParam_t(cmdSeq.getId(), cmdNormal2.getId(), DummyCommand::paramX));

  std::vector<ReadOnlyXParameterSet> paramSets;
  transitionItoA->checkForMissingParameters(lEmptyGK, paramSets, result);
  BOOST_CHECK_EQUAL_COLLECTIONS(result.begin(), result.end(), expected.begin(), expected.end());
  BOOST_REQUIRE_EQUAL( paramSets.size(), size_t(3) );
}


BOOST_FIXTURE_TEST_CASE(TestRunErrorTransition, StateMachineTestSetup) {
  LOG(kInfo) << "Running StateMachineTestSuite/TestRunErrorTransition";
  transitionItoA->add(cmdNormal1);
  transitionItoA->add(cmdError);
  transitionItoA->add(cmdNormal2);
  
  // Engage state machine, and confirm object's state is ready for following tests
  testFSM.engage(gk);
  BOOST_REQUIRE_EQUAL( obj->getStatus().getStateMachineId(), testFSM.getId());
  BOOST_REQUIRE_EQUAL( obj->getStatus().getState(), fsmState0);  

  // Running transition with error command: Object should go to error state, and subsequent commands shouldn't run
  BOOST_REQUIRE_NO_THROW(transitionItoA->exec(gk, false));
  BOOST_CHECK_EQUAL(obj->getStatus().getStateMachineId(), testFSM.getId());
  BOOST_CHECK_EQUAL(obj->getStatus().getState(), fsmStateError);
  BOOST_CHECK_EQUAL(transitionItoA->getState(), Functionoid::State::kError);

  StateMachine::TransitionStatus s = transitionItoA->getStatus();
  BOOST_CHECK_EQUAL(s.getActionPath(), transitionItoA->getPath());
  BOOST_CHECK_EQUAL(s.getNumberOfCompletedCommands(), size_t(2));
  BOOST_CHECK_EQUAL(s.getTotalNumberOfCommands(), size_t(3));
  BOOST_CHECK_CLOSE(s.getProgress(), (1.0 + DummyErrorCommand::finalProgress)/3.0, 0.0001);
  BOOST_CHECK_LT(s.getProgress(), 0.66);
  BOOST_CHECK_GT(s.getRunningTime(), 0.0);
  BOOST_CHECK_EQUAL(s.getState(), Functionoid::State::kError);
  BOOST_REQUIRE_EQUAL(s.getCommandStatus().size(), size_t(2));
  BOOST_CHECK_EQUAL(s.getCommandStatus().at(0).getActionPath(), cmdNormal1.getPath());
  BOOST_CHECK_EQUAL(s.getCommandStatus().at(0).getState(), Functionoid::State::kDone);
  BOOST_CHECK_EQUAL(s.getCommandStatus().at(1).getActionPath(), cmdError.getPath());
  BOOST_CHECK_EQUAL(s.getCommandStatus().at(1).getState(), Functionoid::State::kError);
  
  BOOST_REQUIRE_EQUAL(s.getResults().size(), size_t(2));
  BOOST_CHECK_EQUAL(s.getResults().at(0) , cmdNormal1.getStatus().getResult() );
  BOOST_CHECK_EQUAL(s.getResults().at(1) , cmdError.getStatus().getResult() );

  BOOST_CHECK_EQUAL(cmdNormal2.getState(), Functionoid::State::kInitial);
  BOOST_CHECK_EQUAL(cmdNormal2.getStatus().getState(), Functionoid::State::kInitial);
}


BOOST_FIXTURE_TEST_CASE(TestRunWarningTransition, StateMachineTestSetup) {
  LOG(kInfo) << "Running StateMachineTestSuite/TestRunWarningTransition";
  transitionItoA->add(cmdNormal1);
  transitionItoA->add(cmdWarning);
  transitionItoA->add(cmdNormal2);
  
  // Engage state machine, and confirm object's state is ready for following tests
  testFSM.engage(gk);
  BOOST_REQUIRE_EQUAL( obj->getStatus().getStateMachineId(), testFSM.getId());
  BOOST_REQUIRE_EQUAL( obj->getStatus().getState(), fsmState0);  

  // Running transition with warning command: Object should go to state A, and all commands shouldt run
  BOOST_REQUIRE_NO_THROW(transitionItoA->exec(gk, false));
  BOOST_CHECK_EQUAL(obj->getStatus().getStateMachineId(), testFSM.getId());
  BOOST_CHECK_EQUAL(obj->getStatus().getState(), fsmStateA);
  BOOST_CHECK_EQUAL(transitionItoA->getState(), Functionoid::State::kWarning);

  StateMachine::TransitionStatus s = transitionItoA->getStatus();
  BOOST_CHECK_EQUAL(s.getActionPath(), transitionItoA->getPath());
  BOOST_CHECK_EQUAL(s.getNumberOfCompletedCommands(), size_t(3));
  BOOST_CHECK_EQUAL(s.getTotalNumberOfCommands(), size_t(3));
  BOOST_CHECK_EQUAL(s.getProgress(), 1.0);
  BOOST_CHECK_EQUAL(s.getState(), Functionoid::State::kWarning);
  BOOST_REQUIRE_EQUAL(s.getCommandStatus().size(), size_t(3));
  BOOST_CHECK_EQUAL(s.getCommandStatus().at(0).getActionPath(), cmdNormal1.getPath());
  BOOST_CHECK_EQUAL(s.getCommandStatus().at(0).getState(), Functionoid::State::kDone);
  BOOST_CHECK_EQUAL(s.getCommandStatus().at(1).getActionPath(), cmdWarning.getPath());
  BOOST_CHECK_EQUAL(s.getCommandStatus().at(1).getState(), Functionoid::State::kWarning);
  BOOST_CHECK_EQUAL(s.getCommandStatus().at(2).getActionPath(), cmdNormal2.getPath());
  BOOST_CHECK_EQUAL(s.getCommandStatus().at(2).getState(), Functionoid::State::kDone);

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
  testFSM.engage(gk);
  BOOST_REQUIRE_EQUAL( obj->getStatus().getStateMachineId(), testFSM.getId());
  BOOST_REQUIRE_EQUAL( obj->getStatus().getState(), fsmState0);  

  // Running transition with warning command: Object should go to state A, and all commands shouldt run
  BOOST_REQUIRE_NO_THROW(transitionItoA->exec(gk, false));
  BOOST_CHECK_EQUAL(obj->getStatus().getStateMachineId(), testFSM.getId());
  BOOST_CHECK_EQUAL(obj->getStatus().getState(), fsmStateA);
  BOOST_CHECK_EQUAL(transitionItoA->getState(), Functionoid::State::kDone);

  StateMachine::TransitionStatus s = transitionItoA->getStatus();
  BOOST_CHECK_EQUAL(s.getActionPath(), transitionItoA->getPath());
  BOOST_CHECK_EQUAL(s.getNumberOfCompletedCommands(), size_t(2));
  BOOST_CHECK_EQUAL(s.getTotalNumberOfCommands(), size_t(2));
  BOOST_CHECK_EQUAL(s.getProgress(), 1.0);
  BOOST_CHECK_EQUAL(s.getState(), Functionoid::State::kDone);
  BOOST_REQUIRE_EQUAL(s.getCommandStatus().size(), size_t(2));
  BOOST_CHECK_EQUAL(s.getCommandStatus().at(0).getActionPath(), cmdNormal1.getPath());
  BOOST_CHECK_EQUAL(s.getCommandStatus().at(0).getState(), Functionoid::State::kDone);
  BOOST_CHECK_EQUAL(s.getCommandStatus().at(1).getActionPath(), cmdNormal2.getPath());
  BOOST_CHECK_EQUAL(s.getCommandStatus().at(1).getState(), Functionoid::State::kDone);

  BOOST_REQUIRE_EQUAL(s.getResults().size(), size_t(2));
  BOOST_CHECK_EQUAL(s.getResults().at(0) , cmdNormal1.getStatus().getResult() );
  BOOST_CHECK_EQUAL(s.getResults().at(1) , cmdNormal2.getStatus().getResult() );
}

BOOST_FIXTURE_TEST_CASE(TestDescendantMonitoringSettings, StateMachineTestSetup) {
  LOG(kInfo) << "Running StateMachineTestSuite/TestDescendantMonitoringSettings";

  BOOST_REQUIRE_EQUAL(child1.getMonitoringStatus(), monitoring::kEnabled);
  BOOST_REQUIRE_EQUAL(child1.getMetric("dummyMetric").getMonitoringStatus(), monitoring::kEnabled);
  testFSM.engage(gk);
  BOOST_REQUIRE_EQUAL( obj->getStatus().getStateMachineId(), testFSM.getId());
  BOOST_REQUIRE_EQUAL( obj->getStatus().getState(), fsmState0);

  BOOST_REQUIRE_NO_THROW(transitionItoA->exec(gk, false));
  BOOST_REQUIRE_NO_THROW(transitionAtoB->exec(gk, false));
  BOOST_REQUIRE_EQUAL(child1.getMonitoringStatus(), monitoring::kNonCritical);
  BOOST_REQUIRE_EQUAL(grandChild1.getMonitoringStatus(), monitoring::kDisabled);
  // this one should fail, as paths need to be relative to obj and this setting has been made with ID 'grandChild2'
  // not 'child1.grandChild2'
  BOOST_REQUIRE_NE(grandChild2.getMonitoringStatus(), monitoring::kDisabled);
  // metrics
  BOOST_REQUIRE_EQUAL(child1.getMetric("dummyMetric").getMonitoringStatus(), monitoring::kDisabled);
}



/* ---------------------------------------------------------------- */
/*   PART 4: RESETTING                                              */
/* ---------------------------------------------------------------- */


BOOST_FIXTURE_TEST_CASE(TestResetFSM, StateMachineTestSetup) {
  LOG(kInfo) << "Running StateMachineTestSuite/TestResetFSM";
  DummyGateKeeper lEmptyGK;
  
  // Engage state machine and confirm initial state to prepare for following tests
  BOOST_REQUIRE_NO_THROW( testFSM.engage(lEmptyGK) );
  BOOST_REQUIRE_EQUAL( obj->getStatus().getStateMachineId(), testFSM.getId() );
  BOOST_REQUIRE_EQUAL( obj->getStatus().getState(), fsmState0 );
  
  // Reset state machine
  BOOST_CHECK_NO_THROW( testFSM.reset(lEmptyGK) );
  BOOST_CHECK_EQUAL( obj->getStatus().getStateMachineId(), testFSM.getId() );
  BOOST_CHECK_EQUAL( obj->getStatus().getState(), fsmState0 );
  
  // Run a transition to prepare for following tests
  BOOST_REQUIRE_NO_THROW( transitionItoA->exec(DummyGateKeeper(), false) );
  BOOST_REQUIRE_EQUAL( obj->getStatus().getStateMachineId(), testFSM.getId() );
  BOOST_REQUIRE_EQUAL( obj->getStatus().getState(), fsmStateA );
  
  // Resetting another FSM: Should throw, and leave state unchanged
  StateMachine& otherFSM = obj->registerStateMachine("anotherFSM", "otherInitialState", "otherErrorState");
  BOOST_CHECK_THROW(otherFSM.reset(lEmptyGK), ResourceInWrongStateMachine);
  BOOST_CHECK_EQUAL( obj->getStatus().getStateMachineId(), testFSM.getId() );
  BOOST_CHECK_EQUAL( obj->getStatus().getState(), fsmStateA );

  // Reset correct state machine
  BOOST_CHECK_NO_THROW( testFSM.reset(lEmptyGK) );
  BOOST_CHECK_EQUAL( obj->getStatus().getStateMachineId(), testFSM.getId() );
  BOOST_CHECK_EQUAL( obj->getStatus().getState(), fsmState0 );
}


BOOST_FIXTURE_TEST_CASE(TestResetDisengagedFSM, StateMachineTestSetup) {
  LOG(kInfo) << "Running StateMachineTestSuite/TestResetDisengagedFSM";

  // Resetting FSM before engaged: Should throw, and leave state + MaskableObjects unchanged
  BOOST_CHECK_THROW( testFSM.reset(gk), ResourceInWrongStateMachine);
  BOOST_CHECK_EQUAL( obj->getStatus().getStateMachineId(), ActionableStatus::kNullStateMachineId );
  BOOST_CHECK_EQUAL( obj->getStatus().getState(), ActionableStatus::kNullStateId );
  
  BOOST_CHECK_EQUAL(maskableA.isMasked(), false);
  BOOST_CHECK_EQUAL(maskableB.isMasked(), false);
  BOOST_CHECK_EQUAL(maskableC.isMasked(), false);
}


BOOST_FIXTURE_TEST_CASE(TestDescendantsMaskedDuringReset, StateMachineTestSetup)
{
  LOG(kInfo) << "Running StateMachineTestSuite/TestDescendantsMaskedDuringReset";

  // PART A :
  //  * Goal: Check that MaskableObjects we ask it to mask are being masked
  //  * Start off with everything unmasked

  // First must engage FSM. N.B. use empty gatekeeper so that maskable objects remain unmasked
  testFSM.engage(DummyGateKeeper());

  // A.1) Require that all are unmasked before tests start
  BOOST_REQUIRE_EQUAL(maskableA.isMasked(), false);
  BOOST_REQUIRE_EQUAL(maskableB.isMasked(), false);
  BOOST_REQUIRE_EQUAL(maskableC.isMasked(), false);
  // A.2) Engage FSM; check that A & B now masked, but C remains unmasked
  testFSM.reset(gk);
  BOOST_CHECK_EQUAL(maskableA.isMasked(), true);
  BOOST_CHECK_EQUAL(maskableB.isMasked(), true);
  BOOST_CHECK_EQUAL(maskableC.isMasked(), false);

  // PART B :
  //   * Goal: Check that all MaskableObjects we don't ask it to mask are being reset to unmasked state
  //   * Start off with A & B unmasked, but C masked
  // B.1) Unmask A & B, mask C; require that starting assumptions are correct before testing
  maskableA.setMasked(false);
  maskableB.setMasked(false);
  maskableC.setMasked(true);
  BOOST_REQUIRE_EQUAL(maskableA.isMasked(), false);
  BOOST_REQUIRE_EQUAL(maskableB.isMasked(), false);
  BOOST_REQUIRE_EQUAL(maskableC.isMasked(), true);
  
  // B.2) Engage FSM; check that A & B now masked, but C remains unmasked
  testFSM.reset(gk);
  BOOST_CHECK_EQUAL(maskableA.isMasked(), true);
  BOOST_CHECK_EQUAL(maskableB.isMasked(), true);
  BOOST_CHECK_EQUAL(maskableC.isMasked(), false);
}


BOOST_AUTO_TEST_SUITE_END() // StateMachineTestSuite
} /* namespace test */
} /* namespace core */
} /* namespace swatch */
