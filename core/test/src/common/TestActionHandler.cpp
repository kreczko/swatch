/*
 * Command_test.cpp
 *
 *  Created on: 16 Feb 2015
 *      Author: kreczko
 */
#include <boost/test/unit_test.hpp>
#include <boost/foreach.hpp>
// swatch headers
#include "swatch/core/ActionableObject.hpp"
#include "swatch/logger/Log.hpp"
#include "swatch/core/test/DummyCommand.hpp"
#include "swatch/core/test/DummyHandler.hpp"
#include "swatch/core/test/DummyOperation.hpp"

using namespace swatch::logger;
using namespace swatch::core;
namespace swatch {
namespace core {
namespace test {

struct ActionableObjectTestSetup {
  ActionableObjectTestSetup():
  handler(){
    handler.Register<DummyCommand>("dummy_1");//, new DummyCommand(&handler));
    handler.Register<DummyCommand>("dummy_2");//, new DummyCommand(&handler));
    handler.Register<DummyCommand>("dummy_3");//, new DummyCommand(&handler));
    handler.Register<DummyOperation>("test_1");
    handler.Register<DummyOperation>("test_2");
  }
  ~ActionableObjectTestSetup(){
  }

  DummyHandler handler;

};

BOOST_AUTO_TEST_SUITE( ActionableObjectTestSuite)

BOOST_FIXTURE_TEST_CASE(TestRegisterCommand,  ActionableObjectTestSetup) {
  LOG(kInfo) << "Running ActionableObjectTestSuite/TestRegisterCommand";
  size_t n_commands = handler.getCommands().size();
  handler.Register<DummyCommand>("dummy_5000");
  size_t n_commands_after = handler.getCommands().size();
  BOOST_CHECK_EQUAL(n_commands_after, n_commands + 1);
}

BOOST_FIXTURE_TEST_CASE(TestGetCommand,  ActionableObjectTestSetup) {
  LOG(kInfo) << "Running ActionableObjectTestSuite/TestGetCommand";
  Command* dummy_1 = handler.getCommand("dummy_1");
  BOOST_CHECK_EQUAL(dummy_1->getState(), 0);
  // do not delete commands, they are owned by ActionableObject
  // memory access violation at address: 0x00000081: no mapping at fault address
  // maybe we should return smart pointers (boost::shared_ptr)?
  //  delete dummy_1;
}

BOOST_FIXTURE_TEST_CASE(TestRegisterOperation,  ActionableObjectTestSetup) {
  LOG(kInfo) << "Running ActionableObjectTestSuite/TestRegisterOperation";
  size_t n_ctrl = handler.getOperations().size();
  handler.Register<DummyOperation>("dummy_5000");
  size_t n_ctrl_after = handler.getOperations().size();
  BOOST_CHECK_EQUAL(n_ctrl_after, n_ctrl + 1);
}

BOOST_FIXTURE_TEST_CASE(TestGetOperation,  ActionableObjectTestSetup) {
  LOG(kInfo) << "Running ActionableObjectTestSuite/TestGetOperation";
  Operation* test_1 = handler.getOperation("test_1");
  BOOST_CHECK_EQUAL(test_1->getCurrentState(), "HALTED");
}

BOOST_AUTO_TEST_SUITE_END() // ProcessorTestSuite

} /* namespace test */
} /* namespace core */
} /* namespace swatch */

