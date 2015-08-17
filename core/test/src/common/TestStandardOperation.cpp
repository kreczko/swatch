/*
 * StandardOperation_test.cpp
 *
 *  Created on: 16 Feb 2015
 *      Author: kreczko
 */
#include <boost/test/unit_test.hpp>

// SWATCH headers
#include "swatch/core/test/DummyActionableObject.hpp"
#include "swatch/core/test/DummyStandardOperation.hpp"
#include "swatch/logger/Log.hpp"

#include <xdata/Integer.h>
#include <xdata/String.h>


using namespace swatch::logger;
namespace swatch {
namespace core {
namespace test {
struct StandardOperationTestSetup {
  StandardOperationTestSetup():
    handler(),
    common( handler.registerFunctionoid<DummyStandardOperation>("common") ),
    custom( handler.registerFunctionoid<DummyStandardOperation>("custom") ),
    test( handler.registerFunctionoid<DummyStandardOperation>("test") )
  {
  }

  ~StandardOperationTestSetup(){
  }

  DummyActionableObject handler;
  swatch::core::FSM fsm;
  swatch::core::Operation& common;
  swatch::core::Operation& custom;
  swatch::core::Operation& test;
};

BOOST_AUTO_TEST_SUITE( StandardOperationTestSuite )

BOOST_AUTO_TEST_CASE(TestConstructor) {
  LOG(kInfo) << "Running StandardOperationTestSuite/TestConstructor";
  DummyActionableObject handler;
  swatch::core::StandardOperation& common = handler.registerFunctionoid<DummyStandardOperation>("Common");
  BOOST_CHECK_EQUAL(common.getCurrentState(), "HALTED");
}

// BOOST_FIXTURE_TEST_CASE(TestTodo,  StandardOperationTestSetup) {
//   LOG(kInfo) << "Running StandardOperationTestSuite/TestTodo";
//   BOOST_CHECK(common->getDefaultParams().get<xdata::String>("todo") == "common");
//   BOOST_CHECK(custom->getDefaultParams().get<xdata::String>("todo") == "custom");
//   BOOST_CHECK(test->getDefaultParams().get<xdata::String>("todo") == "test");
// }


BOOST_FIXTURE_TEST_CASE(TestColdReset,  StandardOperationTestSetup) {
  LOG(kInfo) << "Running StandardOperationTestSuite/TestColdReset";
  BOOST_CHECK_EQUAL(common.getCurrentState(), "HALTED");
  common.executeTransition("COLDRESET");
  BOOST_CHECK_EQUAL(common.getCurrentState(), "HALTED");
  BOOST_CHECK_EQUAL(handler.something(), "I am cold...");
}

BOOST_FIXTURE_TEST_CASE(TestConfigure,  StandardOperationTestSetup) {
  LOG(kInfo) << "Running StandardOperationTestSuite/TestConfigure";
  BOOST_CHECK_EQUAL(common.getCurrentState(), "HALTED");
  common.executeTransition("CONFIGURE");
  BOOST_CHECK_EQUAL(common.getCurrentState(), "CONFIGURED");
  BOOST_CHECK_EQUAL(handler.getNumber(), uint32_t(42));
  BOOST_CHECK_EQUAL(handler.something(), "I have been configured");
}

BOOST_FIXTURE_TEST_CASE(TestInvalidEnable,  StandardOperationTestSetup) {
  LOG(kInfo) << "Running StandardOperationTestSuite/TestInvalidEnable";
  BOOST_CHECK_EQUAL(common.getCurrentState(), "HALTED");
  // invalid
  BOOST_CHECK_THROW(common.executeTransition("enable"), swatch::core::exception);
}

BOOST_FIXTURE_TEST_CASE(TestWalkThrough,  StandardOperationTestSetup) {
  LOG(kInfo) << "Running StandardOperationTestSuite/TestWalkThrough";
  BOOST_CHECK_EQUAL(common.getCurrentState(), "HALTED");
  // configure
  common.executeTransition("CONFIGURE");
  BOOST_CHECK_EQUAL(common.getCurrentState(), "CONFIGURED");
  // enable
  common.executeTransition("ENABLE");
  BOOST_CHECK_EQUAL(common.getCurrentState(), "ENABLED");
  BOOST_CHECK_EQUAL(handler.something(), "I am enabled...");
  // stop
  common.executeTransition("STOP");
  BOOST_CHECK_EQUAL(common.getCurrentState(), "CONFIGURED");
  // enable
  common.executeTransition("ENABLE");
  BOOST_CHECK_EQUAL(common.getCurrentState(), "ENABLED");
  // suspend
  common.executeTransition("SUSPEND");
  BOOST_CHECK_EQUAL(common.getCurrentState(), "SUSPENDED");
  // stop
  common.executeTransition("STOP");
  BOOST_CHECK_EQUAL(common.getCurrentState(), "CONFIGURED");
}

BOOST_FIXTURE_TEST_CASE(TestFailWithException,  StandardOperationTestSetup) {
  LOG(kInfo) << "Running StandardOperationTestSuite/TestFailWithException";
  BOOST_CHECK_EQUAL(common.getCurrentState(), "HALTED");
//   invalid
  BOOST_CHECK_THROW(common.executeTransition("failWithException"), swatch::core::exception);
//  BOOST_CHECK_THROW(common->executeTransition("fail"), swatch::core::exception);
}


BOOST_AUTO_TEST_SUITE_END() // StandardOperationTestSuite
} /* namespace test */
} /* namespace core */
} /* namespace swatch */
