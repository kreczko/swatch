/*
 * Command_test.cpp
 *
 *  Created on: 16 Feb 2015
 *      Author: kreczko
 */
#include <boost/test/unit_test.hpp>
#include <boost/foreach.hpp>
// swatch headers
#include "swatch/core/ActionHandler.hpp"
#include "swatch/logger/Log.hpp"
#include "swatch/core/test/DummyCommand.hpp"
#include "swatch/core/test/DummyHandler.hpp"
#include "swatch/core/test/DummyOperation.hpp"

using namespace swatch::logger;
using namespace swatch::core;
namespace swatch {
namespace core {
namespace test {

struct ActionHandlerTestSetup {
  ActionHandlerTestSetup():
  handler(){
    handler.registerCommand<DummyCommand>("dummy_1");//, new DummyCommand(&handler));
    handler.registerCommand<DummyCommand>("dummy_2");//, new DummyCommand(&handler));
    handler.registerCommand<DummyCommand>("dummy_3");//, new DummyCommand(&handler));
    handler.registerOperation<DummyOperation>("test_1");
    handler.registerOperation<DummyOperation>("test_2");
  }
  ~ActionHandlerTestSetup(){
  }

  DummyHandler handler;

};

BOOST_AUTO_TEST_SUITE( ActionHandlerTestSuite)

BOOST_FIXTURE_TEST_CASE(TestRegisterCommand,  ActionHandlerTestSetup) {
  LOG(kInfo) << "Running ActionHandlerTestSuite/TestRegisterCommand";
  auto n_commands = handler.getCommands().size();
  handler.registerCommand<DummyCommand>("dummy_5000");
  auto n_commands_after = handler.getCommands().size();
  BOOST_CHECK_EQUAL(n_commands_after, n_commands + 1);
}

BOOST_FIXTURE_TEST_CASE(TestGetCommand,  ActionHandlerTestSetup) {
  LOG(kInfo) << "Running ActionHandlerTestSuite/TestGetCommand";
  Command* dummy_1 = handler.getCommand("dummy_1");
  BOOST_CHECK_EQUAL(dummy_1->status(), 0);
  // do not delete commands, they are owned by ActionHandler
  // memory access violation at address: 0x00000081: no mapping at fault address
  // maybe we should return smart pointers (boost::shared_ptr)?
  //  delete dummy_1;
}

BOOST_FIXTURE_TEST_CASE(TestRegisterOperation,  ActionHandlerTestSetup) {
  LOG(kInfo) << "Running ActionHandlerTestSuite/TestRegisterOperation";
  auto n_ctrl = handler.getOperations().size();
  handler.registerOperation<DummyOperation>("dummy_5000");
  auto n_ctrl_after = handler.getOperations().size();
  BOOST_CHECK_EQUAL(n_ctrl_after, n_ctrl + 1);
}

BOOST_FIXTURE_TEST_CASE(TestGetOperation,  ActionHandlerTestSetup) {
  LOG(kInfo) << "Running ActionHandlerTestSuite/TestGetOperation";
  Operation* test_1 = handler.getOperation("test_1");
  BOOST_CHECK_EQUAL(test_1->getCurrentState(), "HALTED");
}

BOOST_AUTO_TEST_SUITE_END() // ProcessorTestSuite

} /* namespace test */
} /* namespace core */
} /* namespace swatch */

