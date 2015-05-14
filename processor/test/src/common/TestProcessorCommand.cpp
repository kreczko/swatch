/*
 * Command_test.cpp
 *
 *  Created on: 16 Feb 2015
 *      Author: kreczko
 */
#include <boost/test/unit_test.hpp>
#include <boost/foreach.hpp>
// swatch headers
#include "swatch/core/Command.hpp"
#include "swatch/logger/Log.hpp"
#include "swatch/processor/test/DummyProcessor.hpp"
#include "swatch/processor/test/DummyProcessorCommand.hpp"
#include "swatch/core/test/DummyHandler.hpp"
//xdaq headers
#include <xdata/Integer.h>
#include <xdata/String.h>

using namespace swatch::logger;

namespace swatch {
namespace processor {
namespace test {

struct ProcessorCommandTestSetup {
  ProcessorCommandTestSetup():
  handler("BigDummy", DummyProcessor::generateParams()){
    handler.Register<DummyProcessorCommand>("dummy_getcrate");

    get_crate = handler.getCommand("dummy_getcrate");

    get_crate->getParams().get<xdata::String>("todo") = "getCrateId";
  }
  ~ProcessorCommandTestSetup(){
  }

  DummyProcessor handler;
  swatch::core::Command* get_crate;

};

BOOST_AUTO_TEST_SUITE( ProcessorCommandTestSuite)
// although commands will be created via a factory in the end,
// we want to make sure that the factory can use this
BOOST_FIXTURE_TEST_CASE(TestTodo,  ProcessorCommandTestSetup) {
  LOG(kInfo) << "Running ProcessorCommandTestSuite/TestTodo";
  BOOST_CHECK_EQUAL(get_crate->getParams().get<xdata::String>("todo").toString(), "getCrateId");
}

BOOST_FIXTURE_TEST_CASE(TestRunGetCrate,  ProcessorCommandTestSetup) {
  LOG(kInfo) << "Running ProcessorCommandTestSuite/TestRunGetCrate";
  get_crate->exec();

  BOOST_CHECK_EQUAL(get_crate->getProgress(), 100.0);
  BOOST_CHECK_EQUAL(get_crate->getStatus(), swatch::core::Command::kDone);
  BOOST_CHECK_EQUAL(get_crate->getResult().type(), "string");
  BOOST_CHECK_EQUAL(get_crate->getStatusMsg(), "Dummy command successfully completed");
  BOOST_CHECK(get_crate->getResult<xdata::String>().equals(xdata::String("s2g20-10")));
  BOOST_CHECK_EQUAL(get_crate->getResult<xdata::String>().toString(), "s2g20-10");
}


BOOST_AUTO_TEST_SUITE_END() // ProcessorCommandTestSuite

} /* namespace test */
} /* namespace core */
} /* namespace swatch */
