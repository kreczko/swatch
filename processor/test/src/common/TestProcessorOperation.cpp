/*
 * Operation_test.cpp
 *
 *  Created on: 16 Feb 2015
 *      Author: kreczko
 */
#include <boost/test/unit_test.hpp>

// XDAQ headers
#include "xdata/Integer.h"
#include "xdata/String.h"

// SWATCH headers
#include "swatch/core/Operation.hpp"
#include "swatch/logger/Log.hpp"
#include "swatch/processor/test/DummyProcessor.hpp"
#include "swatch/processor/test/DummyProcessorOperation.hpp"


using namespace swatch::logger;
namespace swct = swatch::core::test;

namespace swatch {
namespace processor {
namespace test {
struct ProcessorOperationTestSetup {
  ProcessorOperationTestSetup():
    resource(DummyProcessor::generateParams("BigDummy")),
    testing(resource.getOperation("testing"))
  {
  }
  
  ~ProcessorOperationTestSetup() {}

  DummyProcessor resource;
  swatch::core::Operation& testing;
};

BOOST_AUTO_TEST_SUITE( ProcessorOperationTestSuite)

// although commands will be created via a factory in the end,
// we want to make sure that the factory can use this
// BOOST_FIXTURE_TEST_CASE(TestTodo,  ProcessorOperationTestSetup) {
//   LOG(kInfo) << "Running ProcessorOperationTestSuite/TestTodo";
//   BOOST_CHECK(testing->getDefaultParams().get<xdata::String>("todo") == "test");
// }

BOOST_FIXTURE_TEST_CASE(TestOperation,  ProcessorOperationTestSetup) {
  LOG(kInfo) << "Running ProcessorOperationTestSuite/TestOperation";
  BOOST_CHECK_EQUAL(testing.getCurrentState(), "HALTED");
  // test
  testing.executeTransition("test");
  BOOST_CHECK_EQUAL(testing.getCurrentState(), "TESTED");
  BOOST_CHECK_EQUAL(resource.ranTests().size(), size_t(2));
}


BOOST_AUTO_TEST_SUITE_END() // ProcessorOperationTestSuite

} /* namespace test */
} /* namespace core */
} /* namespace swatch */
