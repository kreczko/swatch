/*
 * Operation_test.cpp
 *
 *  Created on: 16 Feb 2015
 *      Author: kreczko
 */
#include <boost/test/unit_test.hpp>

// swatch headers
#include "swatch/core/test/DummyHandler.hpp"
#include "swatch/logger/Log.hpp"

#include <xdata/Integer.h>
#include <xdata/String.h>
#include "../../../include/swatch/core/Operation.hpp"
#include "../../include/swatch/core/test/DummyOperation.hpp"

using namespace swatch::logger;
namespace swatch {
namespace core {
namespace test {
struct OperationTestSetup {
  OperationTestSetup():
  handler(){
    handler.Register<DummyOperation>("common");
    handler.Register<DummyOperation>("custom");
    handler.Register<DummyOperation>("test");

    common = handler.getOperation("common");
    custom = handler.getOperation("custom");
    test = handler.getOperation("test");

    common->getParams().get<xdata::String>("todo") = "common";
    custom->getParams().get<xdata::String>("todo") = "custom";
    test->getParams().get<xdata::String>("todo") = "test";
  }
  ~OperationTestSetup(){
  }

  DummyHandler handler;
  swatch::core::FSM fsm;
  swatch::core::Operation* common, *custom, *test;

};

BOOST_AUTO_TEST_SUITE( OperationTestSuite )

BOOST_AUTO_TEST_CASE(TestConstructor) {
  LOG(kInfo) << "Running OperationTestSuite/TestConstructor";
  DummyHandler handler = DummyHandler();
  swatch::core::Operation* common = handler.Register<DummyOperation>("Common");
  BOOST_CHECK_EQUAL(common->getCurrentState(), "HALTED");
}

BOOST_FIXTURE_TEST_CASE(TestTodo,  OperationTestSetup) {
  LOG(kInfo) << "Running OperationTestSuite/TestTodo";
  BOOST_CHECK(common->getParams().get<xdata::String>("todo") == "common");
  BOOST_CHECK(custom->getParams().get<xdata::String>("todo") == "custom");
  BOOST_CHECK(test->getParams().get<xdata::String>("todo") == "test");
}


BOOST_FIXTURE_TEST_CASE(TestColdReset,  OperationTestSetup) {
  LOG(kInfo) << "Running OperationTestSuite/TestColdReset";
  BOOST_CHECK_EQUAL(common->getCurrentState(), "HALTED");
  common->executeTransition("coldReset");
  BOOST_CHECK_EQUAL(common->getCurrentState(), "HALTED");
  BOOST_CHECK_EQUAL(handler.something(), "I am cold...");
}

BOOST_FIXTURE_TEST_CASE(TestConfigure,  OperationTestSetup) {
  LOG(kInfo) << "Running OperationTestSuite/TestConfigure";
  BOOST_CHECK_EQUAL(common->getCurrentState(), "HALTED");
  common->executeTransition("configure");
  BOOST_CHECK_EQUAL(common->getCurrentState(), "CONFIGURED");
  BOOST_CHECK_EQUAL(handler.number(), int(42));
  BOOST_CHECK_EQUAL(handler.something(), "I have been configured");
}

BOOST_FIXTURE_TEST_CASE(TestInvalidEnable,  OperationTestSetup) {
  LOG(kInfo) << "Running OperationTestSuite/TestInvalidEnable";
  BOOST_CHECK_EQUAL(common->getCurrentState(), "HALTED");
  // invalid
  BOOST_CHECK_THROW(common->executeTransition("enable"), swatch::core::exception);
}

BOOST_FIXTURE_TEST_CASE(TestWalkThrough,  OperationTestSetup) {
  LOG(kInfo) << "Running OperationTestSuite/TestWalkThrough";
  BOOST_CHECK_EQUAL(common->getCurrentState(), "HALTED");
  // configure
  common->executeTransition("configure");
  BOOST_CHECK_EQUAL(common->getCurrentState(), "CONFIGURED");
  // enable
  common->executeTransition("enable");
  BOOST_CHECK_EQUAL(common->getCurrentState(), "ENABLED");
  BOOST_CHECK_EQUAL(handler.something(), "I am enabled...");
  // stop
  common->executeTransition("stop");
  BOOST_CHECK_EQUAL(common->getCurrentState(), "CONFIGURED");
  // enable
  common->executeTransition("enable");
  BOOST_CHECK_EQUAL(common->getCurrentState(), "ENABLED");
  // suspend
  common->executeTransition("suspend");
  BOOST_CHECK_EQUAL(common->getCurrentState(), "SUSPENDED");
  // stop
  common->executeTransition("stop");
  BOOST_CHECK_EQUAL(common->getCurrentState(), "CONFIGURED");
}

BOOST_FIXTURE_TEST_CASE(TestFailWithException,  OperationTestSetup) {
  LOG(kInfo) << "Running OperationTestSuite/TestFailWithException";
  BOOST_CHECK_EQUAL(common->getCurrentState(), "HALTED");
//   invalid
  BOOST_CHECK_THROW(common->executeTransition("failWithException"), swatch::core::exception);
//  BOOST_CHECK_THROW(common->executeTransition("fail"), swatch::core::exception);
}


BOOST_AUTO_TEST_SUITE_END() // OperationTestSuite
} /* namespace test */
} /* namespace core */
} /* namespace swatch */
