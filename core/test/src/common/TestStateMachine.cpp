
#include <boost/test/unit_test.hpp>


// SWATCH headers
#include "swatch/core/CommandSequence.hpp"
#include "swatch/core/StateMachine.hpp"
#include "swatch/core/test/DummyActionableObject.hpp"
#include "swatch/core/test/DummyCommand.hpp"
#include "swatch/logger/Log.hpp"


using namespace swatch::logger;
namespace swatch {
namespace core {
namespace test {

  struct OperationTestSetup {
    OperationTestSetup() :
      obj(new DummyActionableObject("dummyObj"), ActionableObject::Deleter())
    {
        obj->registerFunctionoid<DummyCommand>(commandIdNormal1);
        obj->registerFunctionoid<DummyCommand>(commandIdNormal2);
        obj->registerFunctionoid<DummyWarningCommand>(commandIdWarning);
        obj->registerFunctionoid<DummyErrorCommand>(commandIdError);
        obj->registerFunctionoid<DummyThrowCommand>(commandIdThrow);

        obj->registerCommandSequence(cmdSeqId, commandIdNormal1).then(commandIdNormal2);
    }
    
    ~OperationTestSetup() {}
    
    boost::shared_ptr<DummyActionableObject> obj;
    static const std::string commandIdNormal1, commandIdNormal2, commandIdWarning, commandIdError, commandIdThrow;
    static const std::string cmdSeqId;
  };
  
  const std::string OperationTestSetup::commandIdNormal1 = "cmdNormal1";
  const std::string OperationTestSetup::commandIdNormal2 = "cmdNormal2";
  const std::string OperationTestSetup::commandIdWarning = "cmdWarning";
  const std::string OperationTestSetup::commandIdError = "cmdError";
  const std::string OperationTestSetup::commandIdThrow = "cmdThrow";
  
  const std::string OperationTestSetup::cmdSeqId = "cmdSeqA";



BOOST_AUTO_TEST_SUITE( NewOperationTestSuite )


BOOST_AUTO_TEST_CASE(TestAddState) {
  LOG(kInfo) << "Running NewOperationTestSuite/TestAddState";

  DummyActionableObject obj("dummy");  
  swatch::core::StateMachine& op = * new StateMachine("anOp", obj, "someState", "myErrState");
  
  BOOST_CHECK_EQUAL(op.getInitialState(), "someState");
  BOOST_CHECK_EQUAL(op.getErrorState(), "myErrState");
  BOOST_CHECK_EQUAL( &obj.getObj(op.getId()), (Object*) &op);
  
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
  LOG(kInfo) << "Running NewOperationTestSuite/TestAddTransition";

  DummyActionableObject obj("dummy");  
  swatch::core::StateMachine& op = * new StateMachine("anOp", obj, "state0", "errState");
  op.addState("state1");
  
  BOOST_CHECK_EQUAL( op.getTransitions("state0").size(), size_t(0) );
  BOOST_CHECK_EQUAL( op.getTransitions("state1").size(), size_t(0) );

  // 1) Add transition
  StateMachine::Transition& tA = op.addTransition("transitionA", "state0", "state1");

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


BOOST_AUTO_TEST_CASE(TestAddTransitionSteps) {
  LOG(kInfo) << "Running NewOperationTestSuite/TestAddTransitionSteps";

  DummyActionableObject obj("dummy");
  Command& cmd = obj.registerFunctionoid<DummyCommand>("aCmd");
  CommandSequence& cmdSeq = obj.registerCommandSequence("aCmdSeq", cmd);
  swatch::core::StateMachine& op = * new StateMachine("anOp", obj, "state0", "errState");
  op.addState("state1");
  
  // 1) Add transition, no steps yet
  StateMachine::Transition& tA = op.addTransition("transitionA", "state0", "state1");

  BOOST_CHECK_EQUAL(tA.size(), size_t(0));
  BOOST_CHECK(tA.begin() == tA.end());
  
  // 2) Add command to transition
  BOOST_CHECK_EQUAL(&(tA.add(cmd)), &tA);
  BOOST_CHECK_EQUAL(tA.size(), size_t(1));
  BOOST_CHECK_EQUAL(&tA.begin()->get(), &cmd);
  BOOST_CHECK_EQUAL( tA.begin()->getAlias(), "");
  BOOST_CHECK_EQUAL( tA.begin()->getNamespace(), "");

  // 3) Add aliased command to transition
  BOOST_CHECK_EQUAL(&(tA.add(cmd, "dummyAlias")), &tA);
  BOOST_CHECK_EQUAL(tA.size(), size_t(2));
  BOOST_CHECK_EQUAL(&(tA.begin()+1)->get(), &cmd);
  BOOST_CHECK_EQUAL( (tA.begin()+1)->getAlias(), "dummyAlias");
  BOOST_CHECK_EQUAL( (tA.begin()+1)->getNamespace(), "");

  // 3) Add command sequence to transition
  BOOST_CHECK_EQUAL(&(tA.add(cmdSeq)), &tA);
  BOOST_CHECK_EQUAL(tA.size(), size_t(2 + cmdSeq.size()));
  CommandVec::const_iterator lIt1 = cmdSeq.begin();
  for(auto lIt2=(tA.begin()+2); lIt1!=cmdSeq.end(); lIt1++, lIt2++)
  {
    BOOST_CHECK_EQUAL(&lIt1->get(), &lIt2->get());
    BOOST_CHECK_EQUAL(lIt1->getAlias(), lIt2->getAlias());
    BOOST_CHECK_EQUAL(lIt1->getNamespace(), lIt2->getNamespace());
  }
}


BOOST_AUTO_TEST_SUITE_END() // OperationTestSuite
} /* namespace test */
} /* namespace core */
} /* namespace swatch */
