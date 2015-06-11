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
#include "swatch/core/test/DummyCommand.hpp"
#include "swatch/core/test/DummyHandler.hpp"
//xdaq headers
#include <xdata/Integer.h>
#include <xdata/String.h>

using namespace swatch::logger;
using namespace swatch::core;

namespace swatch {
namespace core {
namespace test {

struct CommandTestSetup {
  CommandTestSetup():
  handler(),
  print(),
  error(),
  nothing(),
  params(){
    handler.Register<DummyCommand>("dummy_print");//, new DummyCommand(&handler, xdata::Integer(-33)));
    handler.Register<DummyCommand>("dummy_error");//, new DummyCommand(&handler, xdata::Integer(-33)));
    handler.Register<DummyCommand>("dummy_nada");//, new DummyCommand(&handler, xdata::Integer(-33)));

    print = handler.getCommand("dummy_print");
    error = handler.getCommand("dummy_error");
    nothing = handler.getCommand("dummy_nada");

    ((DummyCommand*) print)->registerParameter("todo", xdata::String("print"));
    ((DummyCommand*) error)->registerParameter("todo", xdata::String("error"));
    ((DummyCommand*) nothing)->registerParameter("todo", xdata::String("nothing"));
  }
  ~CommandTestSetup(){
  }

  DummyHandler handler;
  Command* print, *error, *nothing;
  XParameterSet params;
};

BOOST_AUTO_TEST_SUITE( CommandTestSuite)
// although commands will be created via a factory in the end,
// we want to make sure that the factory can use this
BOOST_AUTO_TEST_CASE(TestConstructor) {
  DummyHandler handler = DummyHandler();
  swatch::core::Command* test = handler.Register<DummyCommand>("Test");
  
  BOOST_CHECK(test->getDefaultParams().get<xdata::Integer>("aa").equals(xdata::Integer(15)));
}

BOOST_FIXTURE_TEST_CASE(TestTodo,  CommandTestSetup) {
  LOG(kInfo) << "Running CommandTestSuite/TestTodo";
  BOOST_CHECK(print->getDefaultParams().get<xdata::String>("todo") == "print");
  BOOST_CHECK(error->getDefaultParams().get<xdata::String>("todo") == "error");
  BOOST_CHECK(nothing->getDefaultParams().get<xdata::String>("todo") == "nothing");
}

BOOST_FIXTURE_TEST_CASE(TestRunPrint,  CommandTestSetup) {
  LOG(kInfo) << "Running CommandTestSuite/TestRunPrint";
  print->exec( params , false );

  BOOST_CHECK_EQUAL(print->getProgress(), 100.0);
  BOOST_CHECK_EQUAL(print->getStatus(), Command::kDone);
  BOOST_CHECK_EQUAL(print->getResult().type(), "int");
  BOOST_CHECK(print->getResult<xdata::Integer>().equals(xdata::Integer(99)));
}

BOOST_FIXTURE_TEST_CASE(TestRunNothing,  CommandTestSetup) {
  LOG(kInfo) << "Running CommandTestSuite/TestRunNothing";
  nothing->exec( params , false );

  BOOST_CHECK_EQUAL(nothing->getProgress(), 0);
  BOOST_CHECK_EQUAL(nothing->getStatus(), Command::kWarning);
  BOOST_CHECK_EQUAL(nothing->getStatusMsg(), "Nothing was done");
}

BOOST_FIXTURE_TEST_CASE(TestRunError,  CommandTestSetup) {
  LOG(kInfo) << "Running CommandTestSuite/TestRunError";
  error->exec( params , false );

  BOOST_CHECK_CLOSE(error->getProgress(), 50.49, 0.1);
  BOOST_CHECK_EQUAL(error->getStatus(), Command::kError);
  BOOST_CHECK_EQUAL(error->getStatusMsg(), "But ended up in error");
}

BOOST_AUTO_TEST_SUITE_END() // CommandTestSuite

} /* namespace test */
} /* namespace core */
} /* namespace swatch */

