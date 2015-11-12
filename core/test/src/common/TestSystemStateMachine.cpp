
#include <boost/test/unit_test.hpp>


// SWATCH headers
#include "swatch/core/CommandSequence.hpp"
#include "swatch/core/StateMachine.hpp"
#include "swatch/core/SystemStateMachine.hpp"
#include "swatch/core/test/DummyActionableSystem.hpp"
#include "swatch/core/test/DummyActionableObject.hpp"
#include "swatch/core/test/DummyCommand.hpp"
#include "swatch/core/test/DummyGateKeeper.hpp"
#include "swatch/logger/Log.hpp"


using namespace swatch::logger;


namespace swatch {
namespace core {
namespace test {

struct SystemStateMachineTestSetup {
  SystemStateMachineTestSetup() :
    sys(new DummyActionableSystem("dummySys"), ActionableSystem::Deleter()),
    fsm( sys->registerStateMachine("mySysFSM", sysState0, sysStateErr) ),
    child1( sys->add(new DummyActionableObject("child1"), ActionableObject::Deleter()) ),
    child2( sys->add(new DummyActionableObject("child2"), ActionableObject::Deleter()) ),
    child3( sys->add(new DummyActionableObject("child3"), ActionableObject::Deleter()) ),
    children{&child1.obj, &child2.obj, &child3.obj}
  {
    fsm.addState(sysStateA);
    fsm.addState(sysStateB);
    sysItoA = &fsm.addTransition("sysIToA", sysState0, sysStateA);
    sysAtoB = &fsm.addTransition("sysAToB", sysStateA, sysStateB);
    
    GateKeeper::tTable tbl(new GateKeeper::tParameters());
    tbl->insert( GateKeeper::tParameters::value_type(DummyCommand::paramToDo, GateKeeper::tParameter(new xdata::String(""))));
    tbl->insert( GateKeeper::tParameters::value_type(DummyCommand::paramX, GateKeeper::tParameter(new xdata::Integer(42))));
    gk.addTable(sys->getId()+".common", tbl);
  }

  ~SystemStateMachineTestSetup() {}
  
  struct Child {
    Child(DummyActionableObject& aObj);
    
    DummyActionableObject& obj;
    Command& cmdNormal1;
    Command& cmdNormal2;
    Command& cmdWarning;
    Command& cmdError;
    StateMachine& fsm;
    StateMachine::Transition* ItoA;
    StateMachine::Transition* ItoB;
    StateMachine::Transition* AtoB;
  };


  boost::shared_ptr<DummyActionableSystem> sys;
  SystemStateMachine& fsm;
  Child child1, child2, child3;
  std::vector<DummyActionableObject*> children;
  SystemTransition* sysItoA;
  SystemTransition* sysAtoB;
  DummyGateKeeper gk;

  static const std::string childState0, childStateErr, childStateA, childStateB;

  static const std::string sysState0, sysStateErr, sysStateA, sysStateB;
};

SystemStateMachineTestSetup::Child::Child(DummyActionableObject& aObj) :
  obj(aObj),
  cmdNormal1( obj.registerFunctionoid<DummyCommand>("cmdNormal1") ),
  cmdNormal2( obj.registerFunctionoid<DummyCommand>("cmdNormal2") ),
  cmdWarning( obj.registerFunctionoid<DummyWarningCommand>("cmdWarning") ),
  cmdError( obj.registerFunctionoid<DummyErrorCommand>("cmdError") ),
  fsm( obj.registerStateMachine("anFSM", childState0, childStateErr) ),
  ItoA(NULL), 
  ItoB(NULL),
  AtoB(NULL)
{
  fsm.addState(childStateA);
  fsm.addState(childStateB);
  ItoA = & fsm.addTransition("initialToA", childState0, childStateA);
  ItoB = & fsm.addTransition("initialToB", childStateB, childStateA);
  AtoB = & fsm.addTransition("AtoB", childStateA, childStateB);
}

const std::string SystemStateMachineTestSetup::childState0 = "s0";
const std::string SystemStateMachineTestSetup::childStateErr = "sE";
const std::string SystemStateMachineTestSetup::childStateA = "sA";
const std::string SystemStateMachineTestSetup::childStateB = "sB";
  
const std::string SystemStateMachineTestSetup::sysState0 = "state0";
const std::string SystemStateMachineTestSetup::sysStateErr = "error";
const std::string SystemStateMachineTestSetup::sysStateA = "stateA";
const std::string SystemStateMachineTestSetup::sysStateB = "stateB";



BOOST_AUTO_TEST_SUITE( SystemStateMachineTestSuite )



/* ------------------------------------------------------------------------- */
/*   PART 1: CONSTRUCTION & INITIALISATION (defining states & transitions)   */
/* ------------------------------------------------------------------------- */

BOOST_AUTO_TEST_CASE(TestConstruction) {
  LOG(kInfo) << "Running SystemStateMachineTestSuite/TestConstruction";

  DummyActionableSystem sys("dummySys");
  swatch::core::SystemStateMachine& fsm = sys.registerStateMachine("aFSM", "someState", "myErrState");
  
  BOOST_CHECK_EQUAL(&fsm.getResource(), &sys);
  BOOST_CHECK_EQUAL(fsm.getInitialState(), "someState");
  BOOST_CHECK_EQUAL(fsm.getErrorState(), "myErrState");
  BOOST_CHECK_EQUAL( &sys.getObj(fsm.getId()), (Object*) &fsm);
  BOOST_CHECK_EQUAL( fsm.getParticipants().size(), size_t(0));
  
  std::vector<std::string> expectedStates = {"someState", "myErrState"};
  BOOST_CHECK_EQUAL_COLLECTIONS( fsm.getStates().begin(), fsm.getStates().end(), expectedStates.begin(), expectedStates.end() );
}


BOOST_AUTO_TEST_CASE(TestAddState) {
  LOG(kInfo) << "Running SystemStateMachineTestSuite/TestAddState";

  DummyActionableSystem sys("dummySys");
  swatch::core::SystemStateMachine& fsm = sys.registerStateMachine("aFSM", "someState", "myErrState");

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
  LOG(kInfo) << "Running SystemStateMachineTestSuite/TestAddTransition";

  DummyActionableSystem sys("dummySys");  
  swatch::core::SystemStateMachine& fsm = sys.registerStateMachine("aFSM", "state0", "errState");
  fsm.addState("state1");
  
  BOOST_CHECK_EQUAL( fsm.getTransitions("state0").size(), size_t(0) );
  BOOST_CHECK_EQUAL( fsm.getTransitions("state1").size(), size_t(0) );

  // 1) Add transition
  SystemTransition& tA = fsm.addTransition("transitionA", "state0", "state1");

  BOOST_CHECK_EQUAL(tA.getId(), "transitionA");
  BOOST_CHECK_EQUAL(tA.getPath(), fsm.getPath()+".state0.transitionA");
  BOOST_CHECK_EQUAL(tA.getStartState(), "state0");
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


BOOST_FIXTURE_TEST_CASE(TestAddTransitionSteps, SystemStateMachineTestSetup) {
  LOG(kInfo) << "Running SystemStateMachineTestSuite/TestAddTransitionSteps";

  // Confirm expected construction before starting tests
  BOOST_REQUIRE_EQUAL(fsm.getParticipants().size(), size_t(0));
  BOOST_REQUIRE_EQUAL(sysItoA->size(), size_t(0));
  BOOST_REQUIRE_EQUAL(sysAtoB->size(), size_t(0));
  
  std::vector<StateMachine::Transition*> transitions;
  
  // 1) Adding empty step: should throw, leaving transition unchanged
  BOOST_CHECK_THROW( sysItoA->add(transitions), InvalidSystemTransition);
  BOOST_CHECK_EQUAL( sysItoA->size(), size_t(0));
  BOOST_CHECK( sysItoA->begin() == sysItoA->end());
  BOOST_CHECK_EQUAL(fsm.getParticipants().size(), size_t(0));

  // 2) Adding transition vector containing NULL ptr: should throw, leaving transition unchanged
  BOOST_CHECK_THROW( sysItoA->add(std::vector<StateMachine::Transition*>{NULL}), InvalidSystemTransition);
  BOOST_CHECK_EQUAL( sysItoA->size(), size_t(0));
  BOOST_CHECK( sysItoA->begin() == sysItoA->end());
  BOOST_CHECK_EQUAL(fsm.getParticipants().size(), size_t(0));
 
  // 3) Adding step for non-child: should throw & leave transition unchanged
  DummyActionableObject nonChildObj;
  StateMachine& nonChildFSM = nonChildObj.registerStateMachine("someFSM", "state0", "ERROR");
  nonChildFSM.addState("state1");
  transitions = { &nonChildFSM.addTransition("t", "state0", "state1") };
  BOOST_CHECK_THROW( sysItoA->add(transitions), InvalidSystemTransition);
  BOOST_CHECK_EQUAL( sysItoA->size(), size_t(0));
  BOOST_CHECK( sysItoA->begin() == sysItoA->end());
  BOOST_CHECK_EQUAL(fsm.getParticipants().size(), size_t(0));

  // 4) Adding step with two transitions from same child: should throw & leave transition unchanged
  transitions = { child1.ItoA , child1.ItoB };
  BOOST_CHECK_THROW( sysItoA->add(transitions), InvalidSystemTransition);
  BOOST_CHECK_EQUAL( sysItoA->size(), size_t(0));
  BOOST_CHECK( sysItoA->begin() == sysItoA->end());
  BOOST_CHECK_EQUAL(fsm.getParticipants().size(), size_t(0));
  
  // 5) Add valid step (just child1), using vector
  std::set<const StateMachine*> expectedParticipants = {&child1.fsm};
  sysItoA->add( std::vector<StateMachine::Transition*>{child1.ItoA} );
  BOOST_CHECK_EQUAL_COLLECTIONS( fsm.getParticipants().begin(), fsm.getParticipants().end(), expectedParticipants.begin(), expectedParticipants.end());
  BOOST_REQUIRE_EQUAL( sysItoA->size(), size_t(1));
  BOOST_REQUIRE( (sysItoA->begin() + 1) == sysItoA->end() );
  BOOST_REQUIRE_EQUAL( sysItoA->begin()->get().size(), size_t(1));
  BOOST_CHECK_EQUAL( sysItoA->begin()->get().at(0), child1.ItoA);
  BOOST_REQUIRE_EQUAL( sysItoA->begin()->cget().size(), size_t(1));
  BOOST_CHECK_EQUAL( sysItoA->begin()->cget().at(0), child1.ItoA);
  
  // 6) Add another valid step (child2 & child3), using iterators to children this time
  sysItoA->add(children.begin()+1, children.end(), child1.fsm.getId(), childState0, child1.ItoA->getId());
  expectedParticipants = {&child1.fsm, &child2.fsm, &child3.fsm};
  BOOST_CHECK_EQUAL_COLLECTIONS( fsm.getParticipants().begin(), fsm.getParticipants().end(), expectedParticipants.begin(), expectedParticipants.end());
  BOOST_REQUIRE_EQUAL( sysItoA->size(), size_t(2));
  BOOST_REQUIRE_EQUAL( (sysItoA->end() - sysItoA->begin()), 2 );
  SystemTransition::iterator lIt = sysItoA->begin() + 1;
  BOOST_REQUIRE_EQUAL( lIt->get().size(), size_t(2));
  BOOST_CHECK_EQUAL( lIt->get().at(0), child2.ItoA);
  BOOST_CHECK_EQUAL( lIt->get().at(1), child3.ItoA);
  BOOST_REQUIRE_EQUAL( lIt->cget().size(), size_t(2));
  BOOST_CHECK_EQUAL( lIt->cget().at(0), child2.ItoA);
  BOOST_CHECK_EQUAL( lIt->cget().at(1), child3.ItoA);  
  
  // 7) Adding step which is inconsistent with previous steps (start/end states of children don't match)
  //    ... should throw & leave transition unchanged
  BOOST_CHECK_THROW(sysItoA->add(std::vector<StateMachine::Transition*>{child1.ItoB}), InvalidSystemTransition);
  BOOST_CHECK_EQUAL( sysItoA->size(), size_t(2));
  BOOST_CHECK_EQUAL( sysItoA->end() - sysItoA->begin(), 2);

  // 8) Adding step with child transition from different child FSM than used in other steps
  //    ... should throw & leave transition unchanged
  StateMachine& child1otherFSM = child1.obj.registerStateMachine("anotherFSM", childState0, childStateErr);
  child1otherFSM.addState(childStateA);
  StateMachine::Transition& child1otherTransition = child1otherFSM.addTransition(child1.ItoA->getId(), childState0, childStateA);
  SystemTransition& t = fsm.addTransition("freshTransition", sysState0, sysStateA);
  transitions = {&child1otherTransition};
  BOOST_CHECK_THROW( t.add(transitions) , InvalidSystemTransition);
  BOOST_CHECK_EQUAL( sysItoA->size(), size_t(2));
  BOOST_CHECK_EQUAL( sysItoA->end() - sysItoA->begin(), 2);
}



/* ---------------------------------------------------------------- */
/*   PART 2: ENGAGING & DISENGAGING                                 */
/* ---------------------------------------------------------------- */

BOOST_FIXTURE_TEST_CASE(TestEngageFSM, SystemStateMachineTestSetup) {
  LOG(kInfo) << "Running SystemStateMachineTestSuite/TestEngageFSM";

  // Finish off setup: Add child1 & child2 to state machine; child3 is left out
  sysItoA->add(children.begin(), children.end()-1, child1.fsm.getId(), childState0, child1.ItoA->getId());

  // Confirm correct construction before staring tests
  BOOST_REQUIRE_EQUAL( sys->getState().getEngagedFSM(), (const SystemStateMachine*) NULL );
  BOOST_REQUIRE_EQUAL( sys->getState().getState(), "" );
  BOOST_REQUIRE(fsm.getParticipants() == (std::set<const StateMachine*>{&child1.fsm, &child2.fsm}));
  
  // Engaging FSM: should put system & children into FSM's initial state
  BOOST_CHECK_NO_THROW( sys->engageStateMachine(fsm.getId()) );
  BOOST_CHECK_EQUAL( sys->getState().getEngagedFSM(), & fsm );
  BOOST_CHECK_EQUAL( sys->getState().getState(), sysState0 );
  BOOST_CHECK_EQUAL( child1.obj.getState().getEngagedFSM(), &child1.fsm);
  BOOST_CHECK_EQUAL( child1.obj.getState().getState(), childState0);
  BOOST_CHECK_EQUAL( child2.obj.getState().getEngagedFSM(), &child2.fsm);
  BOOST_CHECK_EQUAL( child2.obj.getState().getState(), childState0);
  BOOST_CHECK_EQUAL( child3.obj.getState().getEngagedFSM(), (const StateMachine*) NULL);
  BOOST_CHECK_EQUAL( child3.obj.getState().getState(), "");
  
  // Trying to engage FSM again: should throw, and leave 
  BOOST_CHECK_THROW( sys->engageStateMachine(fsm.getId()), ResourceInWrongStateMachine );
  BOOST_CHECK_EQUAL( sys->getState().getEngagedFSM(), & fsm );
  BOOST_CHECK_EQUAL( sys->getState().getState(), sysState0 );
  BOOST_CHECK_EQUAL( child1.obj.getState().getEngagedFSM(), &child1.fsm);
  BOOST_CHECK_EQUAL( child1.obj.getState().getState(), childState0);
  BOOST_CHECK_EQUAL( child2.obj.getState().getEngagedFSM(), &child2.fsm);
  BOOST_CHECK_EQUAL( child2.obj.getState().getState(), childState0);
  BOOST_CHECK_EQUAL( child3.obj.getState().getEngagedFSM(), (const StateMachine*) NULL);
  BOOST_CHECK_EQUAL( child3.obj.getState().getState(), "");
  
  // Trying to engage another FSM also shouldn't work
  sys->registerStateMachine("anotherFSM", "otherInitialState", "otherErrorState");
  BOOST_CHECK_THROW( sys->engageStateMachine("anotherFSM"), ResourceInWrongStateMachine );
  BOOST_CHECK_EQUAL( sys->getState().getEngagedFSM(), & fsm );
  BOOST_CHECK_EQUAL( sys->getState().getState(), sysState0 );
  BOOST_CHECK_EQUAL( child1.obj.getState().getEngagedFSM(), &child1.fsm);
  BOOST_CHECK_EQUAL( child1.obj.getState().getState(), childState0);
  BOOST_CHECK_EQUAL( child2.obj.getState().getEngagedFSM(), &child2.fsm);
  BOOST_CHECK_EQUAL( child2.obj.getState().getState(), childState0);
  BOOST_CHECK_EQUAL( child3.obj.getState().getEngagedFSM(), (const StateMachine*) NULL);
  BOOST_CHECK_EQUAL( child3.obj.getState().getState(), "");
}


BOOST_FIXTURE_TEST_CASE(TestEngageFSMChildComplication, SystemStateMachineTestSetup) {
  LOG(kInfo) << "Running SystemStateMachineTestSuite/TestEngageFSMChildComplication";

  // Finish off setup: Add child1 & child2 to state machine; child3 is left out
  sysItoA->add(children.begin(), children.end()-1, child1.fsm.getId(), childState0, child1.ItoA->getId());

  // Confirm correct construction before staring tests
  BOOST_REQUIRE_EQUAL( sys->getState().getEngagedFSM(), (const SystemStateMachine*) NULL );
  BOOST_REQUIRE_EQUAL( sys->getState().getState(), "" );
  BOOST_REQUIRE(fsm.getParticipants() == (std::set<const StateMachine*>{&child1.fsm, &child2.fsm}));
  
  // Engaging FSM when a child is already engaged: should throw, and have no effect
  child2.obj.engageStateMachine( child2.fsm.getId() );
  BOOST_CHECK_THROW( sys->engageStateMachine(fsm.getId()), ResourceInWrongStateMachine );
  BOOST_CHECK_EQUAL( sys->getState().getEngagedFSM(), (const SystemStateMachine*) NULL );
  BOOST_CHECK_EQUAL( sys->getState().getState(), "" );
  BOOST_CHECK_EQUAL( child1.obj.getState().getEngagedFSM(), (const StateMachine*) NULL);
  BOOST_CHECK_EQUAL( child1.obj.getState().getState(), "");
  BOOST_CHECK_EQUAL( child2.obj.getState().getEngagedFSM(), &child2.fsm);
  BOOST_CHECK_EQUAL( child2.obj.getState().getState(), childState0);
  BOOST_CHECK_EQUAL( child3.obj.getState().getEngagedFSM(), (const StateMachine*) NULL);
  BOOST_CHECK_EQUAL( child3.obj.getState().getState(), "");

  // Engaging FSM after disengaging child2 & engaging child3 FSM:
  // ... should put system & children into FSM's initial state
  child2.fsm.disengage();
  child3.obj.engageStateMachine(child3.fsm.getId());
  BOOST_CHECK_NO_THROW( sys->engageStateMachine(fsm.getId()) );
  BOOST_CHECK_EQUAL( sys->getState().getEngagedFSM(), & fsm );
  BOOST_CHECK_EQUAL( sys->getState().getState(), sysState0 );
  BOOST_CHECK_EQUAL( child1.obj.getState().getEngagedFSM(), &child1.fsm);
  BOOST_CHECK_EQUAL( child1.obj.getState().getState(), childState0);
  BOOST_CHECK_EQUAL( child2.obj.getState().getEngagedFSM(), &child2.fsm);
  BOOST_CHECK_EQUAL( child2.obj.getState().getState(), childState0);
  BOOST_CHECK_EQUAL( child3.obj.getState().getEngagedFSM(), &child3.fsm);
  BOOST_CHECK_EQUAL( child3.obj.getState().getState(), childState0);
}


BOOST_FIXTURE_TEST_CASE(TestDisengageFSM, SystemStateMachineTestSetup) {
  LOG(kInfo) << "Running SystemStateMachineTestSuite/TestDisengageFSM";

  // Finish off setup: Add child1 & child2 to state machine; child3 is left out
  sysItoA->add(children.begin(), children.end()-1, child1.fsm.getId(), childState0, child1.ItoA->getId());

  // Confirm correct construction before starting tests
  BOOST_REQUIRE_EQUAL( sys->getState().getEngagedFSM(), (const SystemStateMachine*) NULL );
  BOOST_REQUIRE_EQUAL( sys->getState().getState(), "" );
  BOOST_REQUIRE(fsm.getParticipants() == (std::set<const StateMachine*>{&child1.fsm, &child2.fsm}));

  // Disengaging before FSM is engaged: Should throw, and leave state unchanged
  BOOST_CHECK_THROW( fsm.disengage(), ResourceInWrongStateMachine);
  BOOST_CHECK_EQUAL( sys->getState().getEngagedFSM(), (const SystemStateMachine*) NULL);
  BOOST_CHECK_EQUAL( sys->getState().getState(), "");
  BOOST_CHECK_EQUAL( child1.obj.getState().getEngagedFSM(), (const StateMachine*) NULL);
  BOOST_CHECK_EQUAL( child1.obj.getState().getState(), "");
  BOOST_CHECK_EQUAL( child2.obj.getState().getEngagedFSM(), (const StateMachine*) NULL);
  BOOST_CHECK_EQUAL( child2.obj.getState().getState(), "");
  BOOST_CHECK_EQUAL( child3.obj.getState().getEngagedFSM(), (const StateMachine*) NULL);
  BOOST_CHECK_EQUAL( child3.obj.getState().getState(), "");
  
  // Engage FSM before continuing unit tests for disengage
  sys->engageStateMachine(fsm.getId());
  BOOST_REQUIRE_EQUAL( sys->getState().getEngagedFSM(), &fsm);
  BOOST_REQUIRE_EQUAL( sys->getState().getState(), sysState0);
  BOOST_REQUIRE_EQUAL( child1.obj.getState().getEngagedFSM(), &child1.fsm);
  BOOST_REQUIRE_EQUAL( child1.obj.getState().getState(), childState0);
  BOOST_REQUIRE_EQUAL( child2.obj.getState().getEngagedFSM(), &child2.fsm);
  BOOST_REQUIRE_EQUAL( child2.obj.getState().getState(), childState0);
  BOOST_REQUIRE_EQUAL( child3.obj.getState().getEngagedFSM(), (const StateMachine*) NULL);
  BOOST_REQUIRE_EQUAL( child3.obj.getState().getState(), "");

  // Disengaging another FSM: Should throw, and leave state unchanged
  SystemStateMachine& otherFSM = sys->registerStateMachine("anotherFSM", "otherInitialState", "otherErrorState");
  BOOST_CHECK_THROW( otherFSM.disengage(), ResourceInWrongStateMachine );
  BOOST_CHECK_EQUAL( sys->getState().getEngagedFSM(), &fsm);
  BOOST_CHECK_EQUAL( sys->getState().getState(), sysState0);
  BOOST_CHECK_EQUAL( child1.obj.getState().getEngagedFSM(), &child1.fsm);
  BOOST_CHECK_EQUAL( child1.obj.getState().getState(), childState0);
  BOOST_CHECK_EQUAL( child2.obj.getState().getEngagedFSM(), &child2.fsm);
  BOOST_CHECK_EQUAL( child2.obj.getState().getState(), childState0);
  BOOST_CHECK_EQUAL( child3.obj.getState().getEngagedFSM(), (const StateMachine*) NULL);
  BOOST_CHECK_EQUAL( child3.obj.getState().getState(), "");

  // Disengaging the engaged FSM: Should return system & children to constructed state
  BOOST_CHECK_NO_THROW( fsm.disengage() );
  BOOST_CHECK_EQUAL( sys->getState().getEngagedFSM(), (const SystemStateMachine*) NULL);
  BOOST_CHECK_EQUAL( sys->getState().getState(), "");
  BOOST_CHECK_EQUAL( child1.obj.getState().getEngagedFSM(), (const StateMachine*) NULL);
  BOOST_CHECK_EQUAL( child1.obj.getState().getState(), "");
  BOOST_CHECK_EQUAL( child2.obj.getState().getEngagedFSM(), (const StateMachine*) NULL);
  BOOST_CHECK_EQUAL( child2.obj.getState().getState(), "");
  BOOST_CHECK_EQUAL( child3.obj.getState().getEngagedFSM(), (const StateMachine*) NULL);
  BOOST_CHECK_EQUAL( child3.obj.getState().getState(), "");
}


BOOST_FIXTURE_TEST_CASE(TestDisengageFSMChildComplications, SystemStateMachineTestSetup) {
  LOG(kInfo) << "Running SystemStateMachineTestSuite/TestDisengageFSMChildComplications";

  // Finish off setup: Add child1 & child2 to state machine; child3 is left out
  sysItoA->add(children.begin(), children.end()-1, child1.fsm.getId(), childState0, child1.ItoA->getId());

  // EngageFSM & confirm correct state before starting tests
  BOOST_REQUIRE(fsm.getParticipants() == (std::set<const StateMachine*>{&child1.fsm, &child2.fsm}));
  sys->engageStateMachine(fsm.getId());
  BOOST_REQUIRE_EQUAL( sys->getState().getEngagedFSM(), &fsm);
  BOOST_REQUIRE_EQUAL( sys->getState().getState(), sysState0);
  BOOST_REQUIRE_EQUAL( child1.obj.getState().getEngagedFSM(), &child1.fsm);
  BOOST_REQUIRE_EQUAL( child1.obj.getState().getState(), childState0);
  BOOST_REQUIRE_EQUAL( child2.obj.getState().getEngagedFSM(), &child2.fsm);
  BOOST_REQUIRE_EQUAL( child2.obj.getState().getState(), childState0);
  BOOST_REQUIRE_EQUAL( child3.obj.getState().getEngagedFSM(), (const StateMachine*) NULL);
  BOOST_REQUIRE_EQUAL( child3.obj.getState().getState(), "");

  // Disengaging system FSM after already disengaging a child: Should throw, and leave state unchanged
  child2.fsm.disengage();
  BOOST_CHECK_THROW( fsm.disengage(), ResourceInWrongStateMachine);
  BOOST_CHECK_EQUAL( sys->getState().getEngagedFSM(), &fsm);
  BOOST_CHECK_EQUAL( sys->getState().getState(), sysState0);
  BOOST_CHECK_EQUAL( child1.obj.getState().getEngagedFSM(), &child1.fsm);
  BOOST_CHECK_EQUAL( child1.obj.getState().getState(), childState0);
  BOOST_CHECK_EQUAL( child2.obj.getState().getEngagedFSM(), (const StateMachine*) NULL);
  BOOST_CHECK_EQUAL( child2.obj.getState().getState(), "");
  BOOST_CHECK_EQUAL( child3.obj.getState().getEngagedFSM(), (const StateMachine*) NULL);
  BOOST_CHECK_EQUAL( child3.obj.getState().getState(), "");

  // Disengaging system FSM after re-engaging child FSM: Should return system & children to constructed state
  child2.obj.engageStateMachine(child2.fsm.getId());
  BOOST_CHECK_NO_THROW( fsm.disengage() );
  BOOST_CHECK_EQUAL( sys->getState().getEngagedFSM(), (const SystemStateMachine*) NULL);
  BOOST_CHECK_EQUAL( sys->getState().getState(), "");
  BOOST_CHECK_EQUAL( child1.obj.getState().getEngagedFSM(), (const StateMachine*) NULL);
  BOOST_CHECK_EQUAL( child1.obj.getState().getState(), "");
  BOOST_CHECK_EQUAL( child2.obj.getState().getEngagedFSM(), (const StateMachine*) NULL);
  BOOST_CHECK_EQUAL( child2.obj.getState().getState(), "");
  BOOST_CHECK_EQUAL( child3.obj.getState().getEngagedFSM(), (const StateMachine*) NULL);
  BOOST_CHECK_EQUAL( child3.obj.getState().getState(), "");
}



/* ---------------------------------------------------------------- */
/*   PART 3: RUNNING TRANSITIONS                                    */
/* ---------------------------------------------------------------- */


BOOST_FIXTURE_TEST_CASE(TestRunEmptyTransition, SystemStateMachineTestSetup) {
  LOG(kInfo) << "Running SystemStateMachineTestSuite/TestRunEmptyTransition";
  DummyGateKeeper gk;

  // Preparation
  sysAtoB->add(children.begin(), children.end(), child1.fsm.getId(), childStateA, child1.AtoB->getId());
  sys->engageStateMachine(fsm.getId());
  child1.fsm.disengage();
  child2.fsm.disengage();
  child3.fsm.disengage();
  BOOST_REQUIRE_EQUAL(sys->getState().getEngagedFSM(), &fsm);
  BOOST_REQUIRE_EQUAL(sys->getState().getState(), sysState0);
  BOOST_REQUIRE_EQUAL(child1.obj.getState().getEngagedFSM(), (const StateMachine*) NULL);
  BOOST_REQUIRE_EQUAL(child2.obj.getState().getEngagedFSM(), (const StateMachine*) NULL);
  BOOST_REQUIRE_EQUAL(child3.obj.getState().getEngagedFSM(), (const StateMachine*) NULL);
  BOOST_REQUIRE_EQUAL(child1.obj.getState().getState(), "");
  BOOST_REQUIRE_EQUAL(child2.obj.getState().getState(), "");
  BOOST_REQUIRE_EQUAL(child3.obj.getState().getState(), "");
  
  // Running empty transition: Should work, regardless of child state
  BOOST_CHECK_NO_THROW( sysItoA->exec(gk, false) );
  BOOST_CHECK_EQUAL(sys->getState().getEngagedFSM(), &fsm);
  BOOST_CHECK_EQUAL(sys->getState().getState(), sysStateA);
  BOOST_CHECK_EQUAL(child1.obj.getState().getEngagedFSM(), (const StateMachine*) NULL);
  BOOST_CHECK_EQUAL(child2.obj.getState().getEngagedFSM(), (const StateMachine*) NULL);
  BOOST_CHECK_EQUAL(child3.obj.getState().getEngagedFSM(), (const StateMachine*) NULL);
  BOOST_CHECK_EQUAL(child1.obj.getState().getState(), "");
  BOOST_CHECK_EQUAL(child2.obj.getState().getState(), "");
  BOOST_CHECK_EQUAL(child3.obj.getState().getState(), "");
  SystemTransitionStatus s = sysItoA->getStatus();
  BOOST_CHECK_EQUAL( s.getActionPath(), sysItoA->getPath() );
  BOOST_CHECK_EQUAL( s.getState(), Functionoid::kDone );
  BOOST_CHECK_EQUAL( s.getProgress(), 1.0 );
  BOOST_CHECK_GE( s.getRunningTime(), 0.0);
  BOOST_CHECK_EQUAL( s.getStepStatus().size(), size_t(0));
  BOOST_CHECK_EQUAL( s.getNumberOfCompletedSteps(), size_t(0));
  BOOST_CHECK_EQUAL( s.getTotalNumberOfSteps(), size_t(0));
}


BOOST_FIXTURE_TEST_CASE(TestRunTransitionDisengagedFSM, SystemStateMachineTestSetup) {
  LOG(kInfo) << "Running SystemStateMachineTestSuite/TestRunTransitionDisengagedFSM";
  DummyGateKeeper gk;

  // Running transition before FSM is engaged: should throw, and leave state/TransitionStatus unchanged
  BOOST_CHECK_THROW( sysItoA->exec(gk, false), ResourceInWrongStateMachine);
  BOOST_CHECK_EQUAL( sysItoA->getStatus().getState(), Functionoid::kInitial);
  BOOST_CHECK_EQUAL( sys->getState().getEngagedFSM(), (const SystemStateMachine*) NULL);
  BOOST_CHECK_EQUAL( sys->getState().getState(), "");
  BOOST_CHECK_EQUAL( child1.obj.getState().getEngagedFSM(), (const StateMachine*) NULL);
  BOOST_CHECK_EQUAL( child1.obj.getState().getState(), "");
  BOOST_CHECK_EQUAL( child2.obj.getState().getEngagedFSM(), (const StateMachine*) NULL);
  BOOST_CHECK_EQUAL( child2.obj.getState().getState(), "");
  BOOST_CHECK_EQUAL( child3.obj.getState().getEngagedFSM(), (const StateMachine*) NULL);
  BOOST_CHECK_EQUAL( child3.obj.getState().getState(), "");

  // Running transition with other FSM engaged: should throw, and leave state/TransitionStatus unchanged
  const SystemStateMachine& otherFSM = sys->registerStateMachine("anotherFSM", sysState0, sysStateErr);
  sys->engageStateMachine("anotherFSM");
  BOOST_REQUIRE_EQUAL( sys->getState().getEngagedFSM(), & otherFSM );
  BOOST_REQUIRE_EQUAL( sys->getState().getState(), sysState0);
  BOOST_CHECK_THROW( sysItoA->exec(gk, false), ResourceInWrongStateMachine );
  BOOST_CHECK_EQUAL( sysItoA->getStatus().getState(), Functionoid::kInitial);
  BOOST_CHECK_EQUAL( sys->getState().getEngagedFSM(), & otherFSM);
  BOOST_CHECK_EQUAL( sys->getState().getState(), sysState0);
}


BOOST_FIXTURE_TEST_CASE(TestRunTransitionMissingParams, SystemStateMachineTestSetup) {
  LOG(kInfo) << "Running SystemStateMachineTestSuite/TestRunTransitionMissingParams";

  sysItoA->add(std::vector<StateMachine::Transition*>{child1.ItoA, child2.ItoA});
  sysItoA->add(std::vector<StateMachine::Transition*>{child3.ItoA});
  child1.ItoA->add(child1.cmdNormal1);
  child3.ItoA->add(child3.cmdNormal2);

  // Engage FSM, and confirm objects are ready for following tests
  sys->engageStateMachine( fsm.getId() );
  BOOST_REQUIRE_EQUAL( sys->getState().getEngagedFSM(), & fsm);
  BOOST_REQUIRE_EQUAL( sys->getState().getState(), sysState0);
  BOOST_REQUIRE_EQUAL( child1.obj.getState().getEngagedFSM(), & child1.fsm);
  BOOST_REQUIRE_EQUAL( child2.obj.getState().getEngagedFSM(), & child2.fsm);
  BOOST_REQUIRE_EQUAL( child3.obj.getState().getEngagedFSM(), & child3.fsm);
  BOOST_REQUIRE_EQUAL( child1.obj.getState().getState(), childState0);
  BOOST_REQUIRE_EQUAL( child2.obj.getState().getState(), childState0);
  BOOST_REQUIRE_EQUAL( child3.obj.getState().getState(), childState0);
  
  // Running transition without parameter values defined: Should throw, and leave state unchanged
  BOOST_CHECK_THROW( sysItoA->exec(DummyGateKeeper(), false), ParameterNotFound );
  BOOST_CHECK_EQUAL( sysItoA->getStatus().getState(), Functionoid::kInitial );
  BOOST_CHECK_EQUAL( child1.ItoA->getState(), Functionoid::kInitial );
  BOOST_CHECK_EQUAL( child2.ItoA->getState(), Functionoid::kInitial );
  BOOST_CHECK_EQUAL( child3.ItoA->getState(), Functionoid::kInitial );
  BOOST_CHECK_EQUAL( child1.obj.getState().getEngagedFSM(), & child1.fsm);
  BOOST_CHECK_EQUAL( child2.obj.getState().getEngagedFSM(), & child2.fsm);
  BOOST_CHECK_EQUAL( child3.obj.getState().getEngagedFSM(), & child3.fsm);
  BOOST_CHECK_EQUAL( child1.obj.getState().getState(), childState0);
  BOOST_CHECK_EQUAL( child2.obj.getState().getState(), childState0);
  BOOST_CHECK_EQUAL( child3.obj.getState().getState(), childState0);

  // checkMissingParams method should return correct list of missing parameters 
  typedef CommandVec::MissingParam tMissingParam;
  std::vector<tMissingParam> result, expected;
  expected.push_back(tMissingParam("", child1.cmdNormal1.getId(), DummyCommand::paramToDo));
  expected.push_back(tMissingParam("", child1.cmdNormal1.getId(), DummyCommand::paramX));

  std::map<const StateMachine::Transition*, std::vector<tMissingParam> > missingParamMap;
  sysItoA->checkForMissingParameters(DummyGateKeeper(), missingParamMap);
  BOOST_CHECK_EQUAL(missingParamMap.size(), size_t(2));
  BOOST_REQUIRE_EQUAL( missingParamMap.count(child1.ItoA), size_t(1));
  BOOST_CHECK_EQUAL_COLLECTIONS(missingParamMap[child1.ItoA].begin(), missingParamMap[child1.ItoA].end(), expected.begin(), expected.end());

  expected.clear();
  expected.push_back(tMissingParam("", child3.cmdNormal2.getId(), DummyCommand::paramToDo));
  expected.push_back(tMissingParam("", child3.cmdNormal2.getId(), DummyCommand::paramX));
  BOOST_REQUIRE_EQUAL( missingParamMap.count(child3.ItoA), size_t(1));
  BOOST_CHECK_EQUAL_COLLECTIONS(missingParamMap[child3.ItoA].begin(), missingParamMap[child3.ItoA].end(), expected.begin(), expected.end());
}


BOOST_FIXTURE_TEST_CASE(TestRunGoodTransition, SystemStateMachineTestSetup) {
  LOG(kInfo) << "Running SystemStateMachineTestSuite/TestRunGoodTransition";

  // Preparation: Add child transitions & commands, engage FSM; then check state before starting unit tests
  child1.ItoA->add(child1.cmdNormal1);
  child2.ItoA->add(child2.cmdNormal1);
  child2.ItoA->add(child2.cmdNormal2);
  child3.ItoA->add(child3.cmdNormal1);
  child3.ItoA->add(child3.cmdNormal2);
  child3.ItoA->add(child3.cmdNormal1);
  sysItoA->add( std::vector<StateMachine::Transition*>{child1.ItoA, child2.ItoA});
  sysItoA->add( std::vector<StateMachine::Transition*>{child3.ItoA});
  sys->engageStateMachine(fsm.getId());
  BOOST_REQUIRE_EQUAL(sys->getState().getEngagedFSM(), &fsm);
  BOOST_REQUIRE_EQUAL(sys->getState().getState(), sysState0);
  BOOST_REQUIRE_EQUAL(sysItoA->getStatus().getState(), Functionoid::kInitial);
  BOOST_REQUIRE_EQUAL(child1.obj.getState().getEngagedFSM(), &child1.fsm);
  BOOST_REQUIRE_EQUAL(child2.obj.getState().getEngagedFSM(), &child2.fsm);
  BOOST_REQUIRE_EQUAL(child3.obj.getState().getEngagedFSM(), &child3.fsm);
  BOOST_REQUIRE_EQUAL(child1.obj.getState().getState(), childState0);
  BOOST_REQUIRE_EQUAL(child2.obj.getState().getState(), childState0);
  BOOST_REQUIRE_EQUAL(child3.obj.getState().getState(), childState0);
  
  // Running transition: Should work, and move system/children into correct state
  BOOST_CHECK_NO_THROW( sysItoA->exec(gk, false) );
  BOOST_CHECK_EQUAL( sys->getState().getEngagedFSM(), &fsm);
  BOOST_CHECK_EQUAL( sys->getState().getState(), sysStateA );
  BOOST_CHECK_EQUAL( child1.obj.getState().getEngagedFSM(), &child1.fsm );
  BOOST_CHECK_EQUAL( child2.obj.getState().getEngagedFSM(), &child2.fsm );
  BOOST_CHECK_EQUAL( child3.obj.getState().getEngagedFSM(), &child3.fsm );
  BOOST_CHECK_EQUAL( child1.obj.getState().getState(), childStateA );
  BOOST_CHECK_EQUAL( child2.obj.getState().getState(), childStateA );
  BOOST_CHECK_EQUAL( child3.obj.getState().getState(), childStateA );

  // Check the transition status object
  SystemTransitionStatus s = sysItoA->getStatus();
  BOOST_CHECK_EQUAL( s.getActionPath(), sysItoA->getPath() );
  BOOST_CHECK_EQUAL( s.getState(), Functionoid::kDone );
  BOOST_CHECK_EQUAL( s.getProgress(), 1.0 );
  BOOST_CHECK_GT( s.getRunningTime(), 0.0 );
  BOOST_CHECK_EQUAL( s.getNumberOfCompletedSteps(), size_t(2) );
  BOOST_CHECK_EQUAL( s.getTotalNumberOfSteps(), size_t(2) );
  BOOST_REQUIRE_EQUAL( s.getStepStatus().size(), size_t(2) );
  
  // Check status of step 1
  BOOST_REQUIRE_EQUAL( s.getStepStatus().at(0).size(), size_t(2) );
  const StateMachine::TransitionStatus& child1Status = *s.getStepStatus().at(0).at(0);
  BOOST_CHECK_EQUAL( child1Status.getActionPath(), child1.ItoA->getPath() );
  BOOST_CHECK_EQUAL( child1Status.getState(), Functionoid::kDone );
  BOOST_CHECK_EQUAL( child1Status.getProgress(), 1.0 );
  BOOST_CHECK_GT( child1Status.getRunningTime(), 0.0 );
  BOOST_CHECK_EQUAL( child1Status.getNumberOfCompletedCommands(), size_t(1) );
  BOOST_CHECK_EQUAL( child1Status.getTotalNumberOfCommands(), size_t(1) );
  BOOST_CHECK_EQUAL( child1Status.getCommandStatus().size(), size_t(1) );
  BOOST_CHECK_EQUAL( child1Status.getResults().size(), size_t(1) );
  const StateMachine::TransitionStatus& child2Status = *s.getStepStatus().at(0).at(1);
  BOOST_CHECK_EQUAL( child2Status.getActionPath(), child2.ItoA->getPath() );
  BOOST_CHECK_EQUAL( child2Status.getState(), Functionoid::kDone );
  BOOST_CHECK_EQUAL( child2Status.getProgress(), 1.0 );
  BOOST_CHECK_GT( child2Status.getRunningTime(), 0.0 );
  BOOST_CHECK_EQUAL( child2Status.getNumberOfCompletedCommands(), size_t(2) );
  BOOST_CHECK_EQUAL( child2Status.getTotalNumberOfCommands(), size_t(2) );
  BOOST_CHECK_EQUAL( child2Status.getCommandStatus().size(), size_t(2) );
  BOOST_CHECK_EQUAL( child2Status.getResults().size(), size_t(2) );

  // Check status of step 2
  BOOST_REQUIRE_EQUAL( s.getStepStatus().at(1).size(), size_t(1) );
  const StateMachine::TransitionStatus& child3Status = *s.getStepStatus().at(1).at(0);
  BOOST_CHECK_EQUAL( child3Status.getActionPath(), child3.ItoA->getPath() );
  BOOST_CHECK_EQUAL( child3Status.getState(), Functionoid::kDone );
  BOOST_CHECK_EQUAL( child3Status.getProgress(), 1.0 );
  BOOST_CHECK_GT( child3Status.getRunningTime(), 0.0 );
  BOOST_CHECK_EQUAL( child3Status.getNumberOfCompletedCommands(), size_t(3) );
  BOOST_CHECK_EQUAL( child3Status.getTotalNumberOfCommands(), size_t(3) );
  BOOST_CHECK_EQUAL( child3Status.getCommandStatus().size(), size_t(3) );
  BOOST_CHECK_EQUAL( child3Status.getResults().size(), size_t(3) );
}


BOOST_FIXTURE_TEST_CASE(TestRunWarningTransition, SystemStateMachineTestSetup) {
  LOG(kInfo) << "Running SystemStateMachineTestSuite/TestRunWarningTransition";

  // Preparation: Add child transitions & commands, engage FSM; then check state before starting unit tests
  child1.ItoA->add(child1.cmdNormal1);
  child2.ItoA->add(child2.cmdWarning);
  child2.ItoA->add(child2.cmdNormal2);
  child3.ItoA->add(child3.cmdNormal1);
  child3.ItoA->add(child3.cmdNormal2);
  child3.ItoA->add(child3.cmdNormal1);
  sysItoA->add( std::vector<StateMachine::Transition*>{child1.ItoA, child2.ItoA});
  sysItoA->add( std::vector<StateMachine::Transition*>{child3.ItoA});
  sys->engageStateMachine(fsm.getId());
  BOOST_REQUIRE_EQUAL(sys->getState().getEngagedFSM(), &fsm);
  BOOST_REQUIRE_EQUAL(sys->getState().getState(), sysState0);
  BOOST_REQUIRE_EQUAL(sysItoA->getStatus().getState(), Functionoid::kInitial);
  BOOST_REQUIRE_EQUAL(child1.obj.getState().getEngagedFSM(), &child1.fsm);
  BOOST_REQUIRE_EQUAL(child2.obj.getState().getEngagedFSM(), &child2.fsm);
  BOOST_REQUIRE_EQUAL(child3.obj.getState().getEngagedFSM(), &child3.fsm);
  BOOST_REQUIRE_EQUAL(child1.obj.getState().getState(), childState0);
  BOOST_REQUIRE_EQUAL(child2.obj.getState().getState(), childState0);
  BOOST_REQUIRE_EQUAL(child3.obj.getState().getState(), childState0);
  
  // Running transition: Should work, and move system/children into correct state
  BOOST_CHECK_NO_THROW( sysItoA->exec(gk, false) );
  BOOST_CHECK_EQUAL( sys->getState().getEngagedFSM(), &fsm);
  BOOST_CHECK_EQUAL( sys->getState().getState(), sysStateA );
  BOOST_CHECK_EQUAL( child1.obj.getState().getEngagedFSM(), &child1.fsm );
  BOOST_CHECK_EQUAL( child2.obj.getState().getEngagedFSM(), &child2.fsm );
  BOOST_CHECK_EQUAL( child3.obj.getState().getEngagedFSM(), &child3.fsm );
  BOOST_CHECK_EQUAL( child1.obj.getState().getState(), childStateA );
  BOOST_CHECK_EQUAL( child2.obj.getState().getState(), childStateA );
  BOOST_CHECK_EQUAL( child3.obj.getState().getState(), childStateA );

  // Check the transition status object
  SystemTransitionStatus s = sysItoA->getStatus();
  BOOST_CHECK_EQUAL( s.getActionPath(), sysItoA->getPath() );
  BOOST_CHECK_EQUAL( s.getState(), Functionoid::kWarning );
  BOOST_CHECK_EQUAL( s.getProgress(), 1.0 );
  BOOST_CHECK_GT( s.getRunningTime(), 0.0 );
  BOOST_CHECK_EQUAL( s.getNumberOfCompletedSteps(), size_t(2) );
  BOOST_CHECK_EQUAL( s.getTotalNumberOfSteps(), size_t(2) );
  BOOST_REQUIRE_EQUAL( s.getStepStatus().size(), size_t(2) );
  
  // Check status of step 1
  BOOST_REQUIRE_EQUAL( s.getStepStatus().at(0).size(), size_t(2) );
  const StateMachine::TransitionStatus& child1Status = *s.getStepStatus().at(0).at(0);
  BOOST_CHECK_EQUAL( child1Status.getActionPath(), child1.ItoA->getPath() );
  BOOST_CHECK_EQUAL( child1Status.getState(), Functionoid::kDone );
  BOOST_CHECK_EQUAL( child1Status.getProgress(), 1.0 );
  BOOST_CHECK_GT( child1Status.getRunningTime(), 0.0 );
  BOOST_CHECK_EQUAL( child1Status.getNumberOfCompletedCommands(), size_t(1) );
  BOOST_CHECK_EQUAL( child1Status.getTotalNumberOfCommands(), size_t(1) );
  BOOST_CHECK_EQUAL( child1Status.getCommandStatus().size(), size_t(1) );
  BOOST_CHECK_EQUAL( child1Status.getResults().size(), size_t(1) );
  const StateMachine::TransitionStatus& child2Status = *s.getStepStatus().at(0).at(1);
  BOOST_CHECK_EQUAL( child2Status.getActionPath(), child2.ItoA->getPath() );
  BOOST_CHECK_EQUAL( child2Status.getState(), Functionoid::kWarning );
  BOOST_CHECK_EQUAL( child2Status.getProgress(), 1.0 );
  BOOST_CHECK_GT( child2Status.getRunningTime(), 0.0 );
  BOOST_CHECK_EQUAL( child2Status.getNumberOfCompletedCommands(), size_t(2) );
  BOOST_CHECK_EQUAL( child2Status.getTotalNumberOfCommands(), size_t(2) );
  BOOST_CHECK_EQUAL( child2Status.getCommandStatus().size(), size_t(2) );
  BOOST_CHECK_EQUAL( child2Status.getResults().size(), size_t(2) );

  // Check status of step 2
  BOOST_REQUIRE_EQUAL( s.getStepStatus().at(1).size(), size_t(1) );
  const StateMachine::TransitionStatus& child3Status = *s.getStepStatus().at(1).at(0);
  BOOST_CHECK_EQUAL( child3Status.getActionPath(), child3.ItoA->getPath() );
  BOOST_CHECK_EQUAL( child3Status.getState(), Functionoid::kDone );
  BOOST_CHECK_EQUAL( child3Status.getProgress(), 1.0 );
  BOOST_CHECK_GT( child3Status.getRunningTime(), 0.0 );
  BOOST_CHECK_EQUAL( child3Status.getNumberOfCompletedCommands(), size_t(3) );
  BOOST_CHECK_EQUAL( child3Status.getTotalNumberOfCommands(), size_t(3) );
  BOOST_CHECK_EQUAL( child3Status.getCommandStatus().size(), size_t(3) );
  BOOST_CHECK_EQUAL( child3Status.getResults().size(), size_t(3) );
}


BOOST_FIXTURE_TEST_CASE(TestRunErrorTransition, SystemStateMachineTestSetup) {
  LOG(kInfo) << "Running SystemStateMachineTestSuite/TestRunErrorTransition";

  // Preparation: Add child transitions & commands, engage FSM; then check state before starting unit tests
  child1.ItoA->add(child1.cmdNormal1);
  child2.ItoA->add(child2.cmdError);
  child2.ItoA->add(child2.cmdNormal2);
  child3.ItoA->add(child3.cmdNormal1);
  child3.ItoA->add(child3.cmdNormal2);
  child3.ItoA->add(child3.cmdNormal1);
  sysItoA->add( std::vector<StateMachine::Transition*>{child1.ItoA, child2.ItoA});
  sysItoA->add( std::vector<StateMachine::Transition*>{child3.ItoA});
  sys->engageStateMachine(fsm.getId());
  BOOST_REQUIRE_EQUAL(sys->getState().getEngagedFSM(), &fsm);
  BOOST_REQUIRE_EQUAL(sys->getState().getState(), sysState0);
  BOOST_REQUIRE_EQUAL(sysItoA->getStatus().getState(), Functionoid::kInitial);
  BOOST_REQUIRE_EQUAL(child1.obj.getState().getEngagedFSM(), &child1.fsm);
  BOOST_REQUIRE_EQUAL(child2.obj.getState().getEngagedFSM(), &child2.fsm);
  BOOST_REQUIRE_EQUAL(child3.obj.getState().getEngagedFSM(), &child3.fsm);
  BOOST_REQUIRE_EQUAL(child1.obj.getState().getState(), childState0);
  BOOST_REQUIRE_EQUAL(child2.obj.getState().getState(), childState0);
  BOOST_REQUIRE_EQUAL(child3.obj.getState().getState(), childState0);
  
  // Running transition: Should move system & child2 into error state, child1 into A, and not affect child3
  BOOST_CHECK_NO_THROW( sysItoA->exec(gk, false) );
  BOOST_CHECK_EQUAL( sys->getState().getEngagedFSM(), &fsm);
  BOOST_CHECK_EQUAL( sys->getState().getState(), sysStateErr );
  BOOST_CHECK_EQUAL( child1.obj.getState().getEngagedFSM(), &child1.fsm );
  BOOST_CHECK_EQUAL( child2.obj.getState().getEngagedFSM(), &child2.fsm );
  BOOST_CHECK_EQUAL( child3.obj.getState().getEngagedFSM(), &child3.fsm );
  BOOST_CHECK_EQUAL( child1.obj.getState().getState(), childStateA );
  BOOST_CHECK_EQUAL( child2.obj.getState().getState(), childStateErr );
  BOOST_CHECK_EQUAL( child3.obj.getState().getState(), childState0 );

  // Check the transition status object
  SystemTransitionStatus s = sysItoA->getStatus();
  BOOST_CHECK_EQUAL( s.getActionPath(), sysItoA->getPath() );
  BOOST_CHECK_EQUAL( s.getState(), Functionoid::kError );
  BOOST_CHECK_EQUAL( s.getProgress(), DummyErrorCommand::finalProgress / float(4) );
  BOOST_CHECK_GT( s.getRunningTime(), 0.0 );
  BOOST_CHECK_EQUAL( s.getNumberOfCompletedSteps(), size_t(1) );
  BOOST_CHECK_EQUAL( s.getTotalNumberOfSteps(), size_t(2) );
  BOOST_REQUIRE_EQUAL( s.getStepStatus().size(), size_t(1) );
  
  // Check status of step 1
  BOOST_REQUIRE_EQUAL( s.getStepStatus().at(0).size(), size_t(2) );
  const StateMachine::TransitionStatus& child1Status = *s.getStepStatus().at(0).at(0);
  BOOST_CHECK_EQUAL( child1Status.getActionPath(), child1.ItoA->getPath() );
  BOOST_CHECK_EQUAL( child1Status.getState(), Functionoid::kDone );
  BOOST_CHECK_EQUAL( child1Status.getProgress(), 1.0 );
  BOOST_CHECK_GT( child1Status.getRunningTime(), 0.0 );
  BOOST_CHECK_EQUAL( child1Status.getNumberOfCompletedCommands(), size_t(1) );
  BOOST_CHECK_EQUAL( child1Status.getTotalNumberOfCommands(), size_t(1) );
  BOOST_CHECK_EQUAL( child1Status.getCommandStatus().size(), size_t(1) );
  BOOST_CHECK_EQUAL( child1Status.getResults().size(), size_t(1) );
  const StateMachine::TransitionStatus& child2Status = *s.getStepStatus().at(0).at(1);
  BOOST_CHECK_EQUAL( child2Status.getActionPath(), child2.ItoA->getPath() );
  BOOST_CHECK_EQUAL( child2Status.getState(), Functionoid::kError );
  BOOST_CHECK_EQUAL( child2Status.getProgress(), DummyErrorCommand::finalProgress / float(2) );
  BOOST_CHECK_GT( child2Status.getRunningTime(), 0.0 );
  BOOST_CHECK_EQUAL( child2Status.getNumberOfCompletedCommands(), size_t(1) );
  BOOST_CHECK_EQUAL( child2Status.getTotalNumberOfCommands(), size_t(2) );
  BOOST_CHECK_EQUAL( child2Status.getCommandStatus().size(), size_t(1) );
  BOOST_CHECK_EQUAL( child2Status.getResults().size(), size_t(1) );
  BOOST_CHECK_EQUAL( child2.cmdNormal1.getStatus().getState(), Functionoid::kInitial );
  BOOST_CHECK_EQUAL( child2.cmdNormal2.getStatus().getState(), Functionoid::kInitial );
  
  // Check that stopped before step 2 - i.e. none of child3's commands were run
  BOOST_CHECK_EQUAL( child3.ItoA->getStatus().getState(), Functionoid::kInitial );
  BOOST_CHECK_EQUAL( child3.cmdNormal1.getStatus().getState(), Functionoid::kInitial );
  BOOST_CHECK_EQUAL( child3.cmdNormal2.getStatus().getState(), Functionoid::kInitial );
}


BOOST_FIXTURE_TEST_CASE(TestRunTransitionWrongSystemState, SystemStateMachineTestSetup) {
  LOG(kInfo) << "Running SystemStateMachineTestSuite/TestRunTransitionWrongSystemState";

  // Preparation: Engage FSM, check state before starting unit tests
  sys->engageStateMachine(fsm.getId());
  BOOST_REQUIRE_EQUAL(sys->getState().getEngagedFSM(), &fsm);
  BOOST_REQUIRE_EQUAL(sys->getState().getState(), sysState0);
  BOOST_REQUIRE_EQUAL(sysItoA->getStatus().getState(), Functionoid::kInitial);
  BOOST_REQUIRE_EQUAL(sysAtoB->getStatus().getState(), Functionoid::kInitial);

  // Running transition A->B from initial state: Should throw, and change nothing
  BOOST_CHECK_THROW( sysAtoB->exec(DummyGateKeeper(), false), ResourceInWrongState );
  BOOST_CHECK_EQUAL(sysAtoB->getStatus().getState(), Functionoid::kInitial);
  BOOST_CHECK_EQUAL(sys->getState().getEngagedFSM(), &fsm);
  BOOST_CHECK_EQUAL(sys->getState().getState(), sysState0);

  // Run transition I->A to prepare for 2nd test
  sysItoA->exec(DummyGateKeeper(), false);
  BOOST_REQUIRE_EQUAL(sys->getState().getEngagedFSM(), &fsm);
  BOOST_REQUIRE_EQUAL(sys->getState().getState(), sysStateA);
  BOOST_REQUIRE_EQUAL(sysAtoB->getStatus().getState(), Functionoid::kInitial);

  // Running transition I->A from state A: Should throw, and change nothing
  BOOST_CHECK_THROW( sysItoA->exec(DummyGateKeeper(), false), ResourceInWrongState);
  BOOST_CHECK_EQUAL(sysAtoB->getStatus().getState(), Functionoid::kInitial);
  BOOST_CHECK_EQUAL(sys->getState().getEngagedFSM(), &fsm);
  BOOST_CHECK_EQUAL(sys->getState().getState(), sysStateA);
}


BOOST_FIXTURE_TEST_CASE(TestRunTransitionWrongChildState, SystemStateMachineTestSetup) {
  LOG(kInfo) << "Running SystemStateMachineTestSuite/TestRunTransitionWrongChildState";

  // Preparation: Add children to transitions, engage FSM, run child1 I->A already, and check state before starting unit tests
  sysItoA->add(children.begin(), children.end(), child1.fsm.getId(), childState0, child1.ItoA->getId());
  sysItoA->add(std::vector<StateMachine::Transition*>{&child1.fsm.addTransition("tmp_AtoA", childStateA, childStateA)});
  sys->engageStateMachine(fsm.getId());
  child1.ItoA->exec(gk, false);
  BOOST_REQUIRE_EQUAL(sys->getState().getEngagedFSM(), &fsm);
  BOOST_REQUIRE_EQUAL(sys->getState().getState(), sysState0);
  BOOST_REQUIRE_EQUAL(sysItoA->getStatus().getState(), Functionoid::kInitial);
  BOOST_REQUIRE_EQUAL(sysAtoB->getStatus().getState(), Functionoid::kInitial);
  BOOST_REQUIRE_EQUAL(child1.obj.getState().getEngagedFSM(), &child1.fsm);
  BOOST_REQUIRE_EQUAL(child2.obj.getState().getEngagedFSM(), &child2.fsm);
  BOOST_REQUIRE_EQUAL(child3.obj.getState().getEngagedFSM(), &child3.fsm);
  BOOST_REQUIRE_EQUAL(child1.obj.getState().getState(), childStateA);
  BOOST_REQUIRE_EQUAL(child2.obj.getState().getState(), childState0);
  BOOST_REQUIRE_EQUAL(child3.obj.getState().getState(), childState0);

  // Running transition I->A with child1 in state A: Should throw, and change nothing
  BOOST_CHECK_THROW( sysItoA->exec(DummyGateKeeper()), ResourceInWrongState );
  do{} while (sysItoA->getStatus().getState() == Functionoid::kScheduled || sysItoA->getStatus().getState() == Functionoid::kRunning);
  BOOST_CHECK_EQUAL(sys->getState().getEngagedFSM(), &fsm);
  BOOST_CHECK_EQUAL(sys->getState().getState(), sysState0);
  BOOST_CHECK_EQUAL(sysItoA->getStatus().getState(), Functionoid::kInitial);
  BOOST_CHECK_EQUAL(sysAtoB->getStatus().getState(), Functionoid::kInitial);
  BOOST_CHECK_EQUAL(child1.obj.getState().getEngagedFSM(), &child1.fsm);
  BOOST_CHECK_EQUAL(child2.obj.getState().getEngagedFSM(), &child2.fsm);
  BOOST_CHECK_EQUAL(child3.obj.getState().getEngagedFSM(), &child3.fsm);
  BOOST_CHECK_EQUAL(child1.obj.getState().getState(), childStateA);
  BOOST_CHECK_EQUAL(child2.obj.getState().getState(), childState0);
  BOOST_CHECK_EQUAL(child3.obj.getState().getState(), childState0);

  // Reset child1 & run transition I->A to prepare for 2nd test
  child1.fsm.reset();
  sysItoA->exec(gk, false);
  sysAtoB->add(std::vector<StateMachine::Transition*>{child1.AtoB, child2.AtoB});
  sysAtoB->add(std::vector<StateMachine::Transition*>{child3.AtoB});
  child3.fsm.reset();
  BOOST_REQUIRE_EQUAL(sys->getState().getEngagedFSM(), &fsm);
  BOOST_REQUIRE_EQUAL(sys->getState().getState(), sysStateA);
  BOOST_REQUIRE_EQUAL(sysItoA->getStatus().getState(), Functionoid::kDone);
  BOOST_REQUIRE_EQUAL(sysAtoB->getStatus().getState(), Functionoid::kInitial);
  BOOST_REQUIRE_EQUAL(child1.obj.getState().getEngagedFSM(), &child1.fsm);
  BOOST_REQUIRE_EQUAL(child2.obj.getState().getEngagedFSM(), &child2.fsm);
  BOOST_REQUIRE_EQUAL(child3.obj.getState().getEngagedFSM(), &child3.fsm);
  BOOST_REQUIRE_EQUAL(child1.obj.getState().getState(), childStateA);
  BOOST_REQUIRE_EQUAL(child2.obj.getState().getState(), childStateA);
  BOOST_REQUIRE_EQUAL(child3.obj.getState().getState(), childState0);

  // Running transition A->B with child3 (2nd step) in state I: Should throw, and change nothing
  BOOST_CHECK_THROW( sysAtoB->exec(DummyGateKeeper()), ResourceInWrongState);
  do{} while (sysAtoB->getStatus().getState() == Functionoid::kScheduled || sysAtoB->getStatus().getState() == Functionoid::kRunning);
  BOOST_CHECK_EQUAL(sys->getState().getEngagedFSM(), &fsm);
  BOOST_CHECK_EQUAL(sys->getState().getState(), sysStateA);
  BOOST_CHECK_EQUAL(sysItoA->getStatus().getState(), Functionoid::kDone);
  BOOST_CHECK_EQUAL(sysAtoB->getStatus().getState(), Functionoid::kInitial);
  BOOST_CHECK_EQUAL(child1.obj.getState().getEngagedFSM(), &child1.fsm);
  BOOST_CHECK_EQUAL(child2.obj.getState().getEngagedFSM(), &child2.fsm);
  BOOST_CHECK_EQUAL(child3.obj.getState().getEngagedFSM(), &child3.fsm);
  BOOST_CHECK_EQUAL(child1.obj.getState().getState(), childStateA);
  BOOST_CHECK_EQUAL(child2.obj.getState().getState(), childStateA);
  BOOST_CHECK_EQUAL(child3.obj.getState().getState(), childState0);
}


BOOST_FIXTURE_TEST_CASE(TestRunTransitionDisengagedChildFSM, SystemStateMachineTestSetup) {
  LOG(kInfo) << "Running SystemStateMachineTestSuite/TestRunTransitionDisengagedChildFSM";
  DummyGateKeeper gk;

  // Preparation: 
  //   * Define transitions: I->A = child1 & child2
  //                         A->B = all threee children
  //   * Engage system FSM & then engage child1 in another FSM w/ same initial state
  //   * Require that state is correct before testing
  sysItoA->add(children.begin(), children.end()-1, child1.fsm.getId(), childState0, child1.ItoA->getId());
  sysAtoB->add(children.begin(), children.end(), child1.fsm.getId(), childStateA, child1.AtoB->getId());
  sys->engageStateMachine(fsm.getId());
  child1.fsm.disengage();
  StateMachine& child1OtherFSM = child1.obj.registerStateMachine("anotherFSM", childState0, childStateErr);
  child1.obj.engageStateMachine(child1OtherFSM.getId());
  BOOST_REQUIRE( fsm.getParticipants() == (std::set<const StateMachine*>{&child1.fsm, &child2.fsm, &child3.fsm}) );
  BOOST_REQUIRE_EQUAL(sys->getState().getEngagedFSM(), &fsm);
  BOOST_REQUIRE_EQUAL(sys->getState().getState(), sysState0);
  BOOST_REQUIRE_EQUAL(child1.obj.getState().getEngagedFSM(), &child1OtherFSM);
  BOOST_REQUIRE_EQUAL(child2.obj.getState().getEngagedFSM(), &child2.fsm);
  BOOST_REQUIRE_EQUAL(child3.obj.getState().getEngagedFSM(), &child3.fsm);
  BOOST_REQUIRE_EQUAL(child1.obj.getState().getState(), childState0);
  BOOST_REQUIRE_EQUAL(child2.obj.getState().getState(), childState0);
  BOOST_REQUIRE_EQUAL(child3.obj.getState().getState(), childState0);

  // Running transition after child FSM has been disengaged: should throw, and leave state/TransitionStatus unchanged
  BOOST_CHECK_THROW( sysItoA->exec(gk, false), ResourceInWrongStateMachine);
  BOOST_CHECK_EQUAL( sysItoA->getStatus().getState(), Functionoid::kInitial);
  BOOST_CHECK_EQUAL( sys->getState().getEngagedFSM(), &fsm);
  BOOST_CHECK_EQUAL( sys->getState().getState(), sysState0);
  BOOST_CHECK_EQUAL( child1.obj.getState().getEngagedFSM(), &child1OtherFSM);
  BOOST_CHECK_EQUAL( child1.obj.getState().getState(), childState0);
  BOOST_CHECK_EQUAL( child2.obj.getState().getEngagedFSM(), &child2.fsm);
  BOOST_CHECK_EQUAL( child2.obj.getState().getState(), childState0);
  BOOST_CHECK_EQUAL( child3.obj.getState().getEngagedFSM(), &child3.fsm);
  BOOST_CHECK_EQUAL( child3.obj.getState().getState(), childState0);

  // Running transition after child1 engaged in correct FSM again: 
  // ... should work, even with child3 in wrong FSM
  child1OtherFSM.disengage();
  child1.obj.engageStateMachine(child1.fsm.getId());
  child3.fsm.disengage();
  BOOST_CHECK_NO_THROW( sysItoA->exec(gk, false));
  BOOST_CHECK_EQUAL( sys->getState().getEngagedFSM(), & fsm );
  BOOST_CHECK_EQUAL( sys->getState().getState(), sysStateA);
  BOOST_CHECK_EQUAL( child1.obj.getState().getEngagedFSM(), &child1.fsm);
  BOOST_CHECK_EQUAL( child1.obj.getState().getState(), childStateA);
  BOOST_CHECK_EQUAL( child2.obj.getState().getEngagedFSM(), &child2.fsm);
  BOOST_CHECK_EQUAL( child2.obj.getState().getState(), childStateA);
  BOOST_CHECK_EQUAL( child3.obj.getState().getEngagedFSM(), (const StateMachine*) NULL);
  BOOST_CHECK_EQUAL( child3.obj.getState().getState(), "");
  SystemTransitionStatus s = sysItoA->getStatus();
  BOOST_CHECK_EQUAL( s.getActionPath(), sysItoA->getPath());
  BOOST_CHECK_EQUAL( s.getState(), Functionoid::kDone);
  BOOST_CHECK_EQUAL( s.getProgress(), 1.0);
  BOOST_CHECK_GE( s.getRunningTime(), 0.0);
  BOOST_REQUIRE_EQUAL( s.getStepStatus().size(), size_t(1));
  BOOST_CHECK_EQUAL( s.getStepStatus().at(0).size(), size_t(2));
  BOOST_CHECK_EQUAL( s.getNumberOfCompletedSteps(), size_t(1));
  BOOST_CHECK_EQUAL( s.getTotalNumberOfSteps(), size_t(1));
}



/* ---------------------------------------------------------------- */
/*   PART 4: RESETTING                                              */
/* ---------------------------------------------------------------- */

BOOST_FIXTURE_TEST_CASE(TestResetFSM, SystemStateMachineTestSetup) {
  LOG(kInfo) << "Running SystemStateMachineTestSuite/TestResetFSMNoChildren";
  
  // Add children to system FSM, engage, and confirm initial state to prepare for following tests
  sysItoA->add(children.begin(), children.end()-1, child1.fsm.getId(), childState0, child1.ItoA->getId());
  BOOST_REQUIRE( fsm.getParticipants() == (std::set<const StateMachine*>{&child1.fsm, &child2.fsm}));
  BOOST_REQUIRE_NO_THROW( sys->engageStateMachine(fsm.getId()) );
  BOOST_REQUIRE_EQUAL( sys->getState().getEngagedFSM(), & fsm );
  BOOST_REQUIRE_EQUAL( sys->getState().getState(), sysState0 );
  BOOST_REQUIRE_EQUAL( child1.obj.getState().getEngagedFSM(), &child1.fsm );
  BOOST_REQUIRE_EQUAL( child2.obj.getState().getEngagedFSM(), &child2.fsm );
  BOOST_REQUIRE_EQUAL( child3.obj.getState().getEngagedFSM(), (const StateMachine*) NULL );
  BOOST_REQUIRE_EQUAL( child1.obj.getState().getState(), childState0 );
  BOOST_REQUIRE_EQUAL( child2.obj.getState().getState(), childState0 );
  BOOST_REQUIRE_EQUAL( child3.obj.getState().getState(), "" );
  
  // Resetting state machine from initial state: should work, but do nothing
  BOOST_CHECK_NO_THROW( fsm.reset() );
  BOOST_CHECK_EQUAL( sys->getState().getEngagedFSM(), & fsm );
  BOOST_CHECK_EQUAL( sys->getState().getState(), sysState0 );
  BOOST_CHECK_EQUAL( child1.obj.getState().getEngagedFSM(), &child1.fsm );
  BOOST_CHECK_EQUAL( child2.obj.getState().getEngagedFSM(), &child2.fsm );
  BOOST_CHECK_EQUAL( child3.obj.getState().getEngagedFSM(), (const StateMachine*) NULL );
  BOOST_CHECK_EQUAL( child1.obj.getState().getState(), childState0 );
  BOOST_CHECK_EQUAL( child2.obj.getState().getState(), childState0 );
  BOOST_CHECK_EQUAL( child3.obj.getState().getState(), "" );
  
  // Run a transition to prepare for following test
  BOOST_REQUIRE_NO_THROW( sysItoA->exec(DummyGateKeeper(), false) );
  BOOST_REQUIRE_EQUAL( sysItoA->getStatus().getState(), Functionoid::kDone );
  BOOST_REQUIRE_EQUAL( sys->getState().getEngagedFSM(), & fsm );
  BOOST_REQUIRE_EQUAL( sys->getState().getState(), sysStateA );
  BOOST_REQUIRE_EQUAL( child1.ItoA->getStatus().getState(), Functionoid::kDone );
  BOOST_REQUIRE_EQUAL( child2.ItoA->getStatus().getState(), Functionoid::kDone );
  BOOST_REQUIRE_EQUAL( child1.obj.getState().getEngagedFSM(), &child1.fsm );
  BOOST_REQUIRE_EQUAL( child2.obj.getState().getEngagedFSM(), &child2.fsm );
  BOOST_REQUIRE_EQUAL( child3.obj.getState().getEngagedFSM(), (const StateMachine*) NULL );
  BOOST_REQUIRE_EQUAL( child1.obj.getState().getState(), childStateA );
  BOOST_REQUIRE_EQUAL( child2.obj.getState().getState(), childStateA );
  BOOST_REQUIRE_EQUAL( child3.obj.getState().getState(), "" );
    
  // Resetting state machine: should reset system & children to initial state
  BOOST_CHECK_NO_THROW( fsm.reset() );
  BOOST_CHECK_EQUAL( sys->getState().getEngagedFSM(), & fsm );
  BOOST_CHECK_EQUAL( sys->getState().getState(), sysState0 );
  BOOST_CHECK_EQUAL( child1.obj.getState().getEngagedFSM(), &child1.fsm );
  BOOST_CHECK_EQUAL( child2.obj.getState().getEngagedFSM(), &child2.fsm );
  BOOST_CHECK_EQUAL( child3.obj.getState().getEngagedFSM(), (const StateMachine*) NULL );
  BOOST_CHECK_EQUAL( child1.obj.getState().getState(), childState0 );
  BOOST_CHECK_EQUAL( child2.obj.getState().getState(), childState0 );
  BOOST_CHECK_EQUAL( child3.obj.getState().getState(), "" );
}


BOOST_FIXTURE_TEST_CASE(TestResetDisengagedFSM, SystemStateMachineTestSetup) {
  LOG(kInfo) << "Running SystemStateMachineTestSuite/TestResetDisengagedFSM";

  // Resetting FSM before engaged: Should throw, and leave state unchanged
  BOOST_CHECK_THROW( fsm.reset(), ResourceInWrongStateMachine);
  BOOST_CHECK_EQUAL( sys->getState().getEngagedFSM(), (const SystemStateMachine*) NULL  );
  BOOST_CHECK_EQUAL( sys->getState().getState(), "" );
  BOOST_CHECK_EQUAL( child1.obj.getState().getEngagedFSM(), (const StateMachine*) NULL );
  BOOST_CHECK_EQUAL( child2.obj.getState().getEngagedFSM(), (const StateMachine*) NULL);
  BOOST_CHECK_EQUAL( child3.obj.getState().getEngagedFSM(), (const StateMachine*) NULL );
  BOOST_CHECK_EQUAL( child1.obj.getState().getState(), "" );
  BOOST_CHECK_EQUAL( child2.obj.getState().getState(), "" );
  BOOST_CHECK_EQUAL( child3.obj.getState().getState(), "" );
}


BOOST_FIXTURE_TEST_CASE(TestResetFSMChildDisengaged, SystemStateMachineTestSetup) {
  LOG(kInfo) << "Running SystemStateMachineTestSuite/TestResetFSMChildDisengaged";

  // Preparation: add children to system FSM, engage, run "I->A" transition & disengage 1 child 
  sysItoA->add(children.begin(), children.end(), child1.fsm.getId(), childState0, child1.ItoA->getId());
  sys->engageStateMachine(fsm.getId());
  sysItoA->exec(DummyGateKeeper(), false);
  child1.fsm.disengage();
  BOOST_REQUIRE_EQUAL(sysItoA->getStatus().getState(), Functionoid::kDone);
  BOOST_REQUIRE_EQUAL(sys->getState().getEngagedFSM(), &fsm);
  BOOST_REQUIRE_EQUAL(sys->getState().getState(), sysStateA);
  BOOST_REQUIRE_EQUAL(child1.ItoA->getStatus().getState(), Functionoid::kDone);
  BOOST_REQUIRE_EQUAL(child2.ItoA->getStatus().getState(), Functionoid::kDone);
  BOOST_REQUIRE_EQUAL(child3.ItoA->getStatus().getState(), Functionoid::kDone);
  BOOST_REQUIRE_EQUAL(child1.obj.getState().getEngagedFSM(), (const StateMachine*) NULL);
  BOOST_REQUIRE_EQUAL(child2.obj.getState().getEngagedFSM(), &child2.fsm);
  BOOST_REQUIRE_EQUAL(child3.obj.getState().getEngagedFSM(), &child3.fsm);
  BOOST_REQUIRE_EQUAL(child1.obj.getState().getState(), "");
  BOOST_REQUIRE_EQUAL(child2.obj.getState().getState(), childStateA);
  BOOST_REQUIRE_EQUAL(child3.obj.getState().getState(), childStateA);

  // Resetting FSM when child disengaged: Should throw, and leave state unchanged
  BOOST_CHECK_THROW( fsm.reset(), ResourceInWrongStateMachine);
  BOOST_REQUIRE_EQUAL(sys->getState().getEngagedFSM(), &fsm);
  BOOST_REQUIRE_EQUAL(sys->getState().getState(), sysStateA);
  BOOST_REQUIRE_EQUAL(child1.obj.getState().getEngagedFSM(), (const StateMachine*) NULL);
  BOOST_REQUIRE_EQUAL(child2.obj.getState().getEngagedFSM(), &child2.fsm);
  BOOST_REQUIRE_EQUAL(child3.obj.getState().getEngagedFSM(), &child3.fsm);
  BOOST_REQUIRE_EQUAL(child1.obj.getState().getState(), "");
  BOOST_REQUIRE_EQUAL(child2.obj.getState().getState(), childStateA);
  BOOST_REQUIRE_EQUAL(child3.obj.getState().getState(), childStateA);  
}

BOOST_AUTO_TEST_SUITE_END() // SystemStateMachineTestSuite


} /* namespace test */
} /* namespace core */
} /* namespace swatch */

