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
#include "swatch/core/test/DummyCommand.hpp"
#include "swatch/core/test/DummySleepCommand.hpp"
#include "swatch/core/test/DummyActionableObject.hpp"
#include "swatch/core/test/DummyOperation.hpp"


using namespace swatch::logger;
using namespace swatch::core;
namespace swatch {
namespace core {
namespace test {

struct ActionableObjectTestSetup {
  ActionableObjectTestSetup():
  handler( new DummyActionableObject() ),
  deleter( new ActionableObject::Deleter() )
  { 
    handler->registerFunctionoid<DummyCommand>("dummy_1");//, new DummyCommand(&handler));
    handler->registerFunctionoid<DummyCommand>("dummy_2");//, new DummyCommand(&handler));
    handler->registerFunctionoid<DummyCommand>("dummy_3");//, new DummyCommand(&handler));
    handler->registerFunctionoid<DummySleepCommand>("sleep");
    handler->registerFunctionoid<DummyOperation>("test_1");
    handler->registerFunctionoid<DummyOperation>("test_2");
  }
  
  ~ActionableObjectTestSetup(){
    if (deleter.get() != NULL)
      (*deleter)(handler);
    else
      delete handler;
  }

  DummyActionableObject* handler;
  boost::scoped_ptr<Object::Deleter> deleter;
};


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
  BOOST_CHECK_EQUAL(dummy_1.getState(), 0);
}


BOOST_FIXTURE_TEST_CASE(TestRegisterOperation,  ActionableObjectTestSetup) {
  LOG(kInfo) << "Running ActionableObjectTestSuite/TestRegisterOperation";
  size_t n_ctrl = handler->getOperations().size();
  handler->registerFunctionoid<DummyOperation>("dummy_5000");
  size_t n_ctrl_after = handler->getOperations().size();
  BOOST_CHECK_EQUAL(n_ctrl_after, n_ctrl + 1);
}


BOOST_FIXTURE_TEST_CASE(TestGetOperation,  ActionableObjectTestSetup) {
  LOG(kInfo) << "Running ActionableObjectTestSuite/TestGetOperation";
  Operation& test_1 = handler->getOperation("test_1");
  BOOST_CHECK_EQUAL(test_1.getCurrentState(), "HALTED");
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

