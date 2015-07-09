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
  ReadWriteXParameterSet params;
};

BOOST_AUTO_TEST_SUITE( CommandTestSuite)
// although commands will be created via a factory in the end,
// we want to make sure that the factory can use this
BOOST_AUTO_TEST_CASE(TestConstructor) {
  DummyHandler handler = DummyHandler();
  swatch::core::Command* test = handler.Register<DummyCommand>("Test");
  
  BOOST_CHECK_EQUAL(test->getDefaultParams().get<xdata::Integer>("aa").value_ , 15);
}

BOOST_FIXTURE_TEST_CASE(TestTodo,  CommandTestSetup) {
  LOG(kInfo) << "Running CommandTestSuite/TestTodo";
  BOOST_CHECK_EQUAL(print->getDefaultParams().get<xdata::String>("todo").value_, "print");
  BOOST_CHECK_EQUAL(error->getDefaultParams().get<xdata::String>("todo").value_, "error");
  BOOST_CHECK_EQUAL(nothing->getDefaultParams().get<xdata::String>("todo").value_,  "nothing");
}

BOOST_FIXTURE_TEST_CASE(TestRunPrint,  CommandTestSetup) {
  LOG(kInfo) << "Running CommandTestSuite/TestRunPrint";
  print->exec( params );

  do {
  } while ( (print->getState() == swatch::core::Command::kScheduled) || (print->getState() == swatch::core::Command::kRunning) );


  CommandStatus status = print->getStatus();
  BOOST_CHECK_EQUAL(status.getProgress(), 1.0);
  BOOST_REQUIRE_EQUAL(status.getState(), Command::kDone);
  BOOST_CHECK_EQUAL(status.getResult()->type(), "int");
  xdata::Integer result(*dynamic_cast<const xdata::Integer*>(status.getResult()));
  BOOST_CHECK(result.equals(xdata::Integer(99)));
}

BOOST_FIXTURE_TEST_CASE(TestRunNothing,  CommandTestSetup) {
  LOG(kInfo) << "Running CommandTestSuite/TestRunNothing";
  nothing->exec( params );

  do {
  } while ( (nothing->getState() == swatch::core::Command::kScheduled) || (nothing->getState() == swatch::core::Command::kRunning) );

  CommandStatus status = nothing->getStatus();
  BOOST_CHECK_EQUAL(status.getProgress(), 1.0);
  BOOST_CHECK_EQUAL(status.getState(), Command::kWarning);
  BOOST_CHECK_EQUAL(status.getStatusMsg(), "Nothing was done");
}

BOOST_FIXTURE_TEST_CASE(TestRunError,  CommandTestSetup) {
  LOG(kInfo) << "Running CommandTestSuite/TestRunError";
  error->exec( params );

  do {
  } while ( (error->getState() == swatch::core::Command::kScheduled) || (error->getState() == swatch::core::Command::kRunning) );

  CommandStatus status = error->getStatus();
  BOOST_CHECK_CLOSE(status.getProgress(), 0.5049, 0.0001);
  BOOST_CHECK_EQUAL(status.getState(), Command::kError);
  BOOST_CHECK_EQUAL(status.getStatusMsg(), "But ended up in error");
}

BOOST_AUTO_TEST_SUITE_END() // CommandTestSuite

} /* namespace test */
} /* namespace core */
} /* namespace swatch */

