
#include <boost/test/unit_test.hpp>


// SWATCH headers
#include "swatch/core/NewOperation.hpp"
#include "swatch/core/test/DummyActionableObject.hpp"
#include "swatch/core/test/DummyCommand.hpp"
#include "swatch/logger/Log.hpp"


using namespace swatch::logger;
namespace swatch {
namespace core {
namespace test {

  struct OperationTestSetup {
    OperationTestSetup() :
      obj(new DummyActionableObject("parent"), ActionableObject::Deleter()),
      childA1( obj->add(new DummyActionableObject("childA1"), ActionableObject::Deleter()) ),
      childA2( obj->add(new DummyActionableObject("childA2"), ActionableObject::Deleter()) ),
      childB( obj->add(new DummyActionableObject("childB"), ActionableObject::Deleter()) )
    {
      std::vector<DummyActionableObject*> children;
      children.push_back(&childA1);
      children.push_back(&childA2);
      children.push_back(&childB);
      
      for(std::vector<DummyActionableObject*>::const_iterator it=children.begin(); it!=children.end(); it++)
      {
        (*it)->registerFunctionoid<DummyCommand>(commandIdNormal1);
        (*it)->registerFunctionoid<DummyCommand>(commandIdNormal2);
        (*it)->registerFunctionoid<DummyWarningCommand>(commandIdWarning);
        (*it)->registerFunctionoid<DummyErrorCommand>(commandIdError);
        (*it)->registerFunctionoid<DummyThrowCommand>(commandIdThrow);
      }
    }
    
    ~OperationTestSetup() {}
    
    boost::shared_ptr<DummyActionableObject> obj;
    DummyActionableObject& childA1;
    DummyActionableObject& childA2;
    DummyActionableObject& childB;
    static const std::string commandIdNormal1, commandIdNormal2, commandIdWarning, commandIdError, commandIdThrow;
    static const std::string cmdSeqId;
  };
  
  const std::string OperationTestSetup::commandIdNormal1 = "cmdNormal1";
  const std::string OperationTestSetup::commandIdNormal2 = "cmdNormal2";
  const std::string OperationTestSetup::commandIdWarning = "cmdWarning";
  const std::string OperationTestSetup::commandIdError = "cmdError";
  const std::string OperationTestSetup::commandIdThrow = "cmdThrow";
  
  const std::string OperationTestSetup::cmdSeqId = "cmdSeqA";
//  struct OperationTestSetup {
//  OperationTestSetup():
//    handler(), 
//    common( handler.registerFunctionoid<DummyOperation>("common") ),
//    custom( handler.registerFunctionoid<DummyOperation>("custom") ),
//    test( handler.registerFunctionoid<DummyOperation>("test") )
//  {
//  }
//
//  ~OperationTestSetup(){
//  }
//
//  DummyActionableObject handler;
//  swatch::core::Operation& common;
//  swatch::core::Operation& custom;
//  swatch::core::Operation& test;
//
//};

BOOST_AUTO_TEST_SUITE( OperationTestSuite )


/*
BOOST_AUTO_TEST_CASE(TestAddState) {
  LOG(kInfo) << "Running OperationTestSuite/TestAddState";
  
  swatch::core::NewOperation op("anOp", "someState");
  
  BOOST_CHECK_EQUAL(op.getCurrentState(), "someState");
  
  std::vector<std::string> expectedStates;
  expectedStates.push_back("someState");
  BOOST_CHECK_EQUAL_COLLECTIONS( op.getStates().begin(), op.getStates().end(), expectedStates.begin(), expectedStates.end() );
  
  // addState should throw if state already defined
  BOOST_CHECK_THROW(op.addState("someState"), swatch::core::exception);
  
  BOOST_CHECK_NO_THROW(op.addState("anotherState"));
  expectedStates.push_back("anotherState");
  BOOST_CHECK_EQUAL_COLLECTIONS( op.getStates().begin(), op.getStates().end(), expectedStates.begin(), expectedStates.end() );
}


BOOST_AUTO_TEST_CASE(TestAddTransition) {
  LOG(kInfo) << "Running OperationTestSuite/TestAddTransition";
  
  swatch::core::NewOperation op("anOp", "state0");
  op.addState("state1");
  
  // 1) Add transition
  BOOST_CHECK_NO_THROW(op.addTransition("transitionA", "state0", "state1"));
  BOOST_CHECK_EQUAL(op.getTransition("transitionA").getId(), "transitionA");
  BOOST_CHECK_EQUAL(&op.getTransition("transitionA").getOperation(), &op);
  
  std::map<std::string, std::map<std::string, NewOperation::Transition*> > expectedMap;
  expectedMap["state0"]["transitionA"] = &op.getTransition("transitionA");
  expectedMap["state1"] = std::map<std::string, NewOperation::Transition*>();

  BOOST_CHECK_EQUAL_COLLECTIONS(op.getStateTransitions("state0").begin(), op.getStateTransitions("state0").end(), expectedMap["state0"].begin(), expectedMap["state1"].end());
  BOOST_CHECK_EQUAL_COLLECTIONS(op.getStateTransitions("state1").begin(), op.getStateTransitions("state1").end(), expectedMap["state1"].begin(), expectedMap["state1"].end());

  // 
  swatch::core::NewOperation& tA = op.getStateTransitions("state0")["transitionA"];
  BOOST_CHECK_EQUAL(tA.size(), 0);
  BOOST_CHECK(tA.begin() == tA.end() );
}
*/



BOOST_AUTO_TEST_SUITE_END() // OperationTestSuite
} /* namespace test */
} /* namespace core */
} /* namespace swatch */
