/*
 * TestCommand.cpp
 *
 *  Created on: August 2015
 *      Author: Tom Williams
 */

#include <boost/test/unit_test.hpp>
#include <boost/foreach.hpp>
// swatch headers
#include "swatch/core/Command.hpp"
#include "swatch/logger/Log.hpp"
#include "swatch/core/test/DummyCommand.hpp"
#include "swatch/core/test/DummyActionableObject.hpp"
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
    obj( new DummyActionableObject(), ActionableObject::Deleter() ),
    cmd( obj->registerFunctionoid<DummyCommand>("dummy_cmd") ),
    warning_cmd( obj->registerFunctionoid<DummyWarningCommand>("dummy_warning_cmd") ),
    error_cmd( obj->registerFunctionoid<DummyErrorCommand>("dummy_error_cmd") ),
    throw_cmd( obj->registerFunctionoid<DummyThrowCommand>("dummy_throw_cmd") )
  {
  }
  
  ~CommandTestSetup(){
  }

  boost::shared_ptr<DummyActionableObject> obj;
  Command& cmd;
  Command& warning_cmd;
  Command& error_cmd;
  Command& throw_cmd;
  ReadWriteXParameterSet params;
};

BOOST_AUTO_TEST_SUITE( CommandTestSuite )


BOOST_FIXTURE_TEST_CASE(TestDefaultParams, CommandTestSetup) {
  LOG(kInfo) << "Running CommandTestSuite/TestDefaultParams";

  BOOST_CHECK_EQUAL(cmd.getDefaultParams().get<xdata::Integer>("x").value_ , 15);
  BOOST_CHECK_EQUAL(cmd.getDefaultParams().get<xdata::String>("todo").value_ , "");
}



BOOST_FIXTURE_TEST_CASE(TestCommandInitialState,  CommandTestSetup) {
  LOG(kInfo) << "Running CommandTestSuite/TestCommandInitialState";

  const std::vector<const Command*> cmds = { &cmd, &warning_cmd, &error_cmd, &throw_cmd};
  
  for(auto it=cmds.begin(); it != cmds.end(); it++)
  {
    BOOST_CHECK_EQUAL( (*it)->getState(), ActionStatus::kInitial );
    
    CommandStatus s = (*it)->getStatus();
    BOOST_CHECK_EQUAL(s.getState(), ActionStatus::kInitial);
    BOOST_CHECK_EQUAL(s.getProgress(), 0.0);
    BOOST_CHECK_EQUAL(s.getStatusMsg(), "initialised");
    BOOST_CHECK_EQUAL(s.getParameters().size(), size_t(0));
    BOOST_CHECK_EQUAL(s.getResult(), (const xdata::Serializable*)NULL);
  }
}


BOOST_FIXTURE_TEST_CASE(TestSuccessfulCommand,  CommandTestSetup) {
  LOG(kInfo) << "Running CommandTestSuite/TestSuccessfulCommand";
  BOOST_CHECK_NE(obj->getNumber(), size_t(54));
  
  params.add("todo", xdata::String("useResource"));
  params.add("x", xdata::Integer(42));
  BOOST_CHECK_NE(params.get<xdata::Integer>("x").value_, DummyCommand::defaultResult.value_);
  cmd.exec(params);
  do {
  } while ( (cmd.getState() == ActionStatus::kScheduled) || (cmd.getState() == ActionStatus::kRunning) );

  BOOST_CHECK_EQUAL(obj->getNumber(), uint32_t(54));
  
  CommandStatus s = cmd.getStatus();
  BOOST_CHECK_EQUAL(s.getState(), ActionStatus::kDone);
  BOOST_CHECK_EQUAL(s.getProgress(), 1.0);
  BOOST_CHECK_EQUAL(s.getStatusMsg(), DummyCommand::finalMsgUseResource);
  BOOST_CHECK_EQUAL(s.getParameters().size(), size_t(2));
  BOOST_CHECK_EQUAL(s.getParameters().get<xdata::Integer>("x").value_, 42);
  BOOST_CHECK_EQUAL(s.getParameters().get<xdata::String>("todo").value_, "useResource");
  BOOST_REQUIRE(s.getResult() != NULL);
  BOOST_CHECK_EQUAL(s.getResultAsString(), params.get("x").toString());  
}


BOOST_FIXTURE_TEST_CASE(TestCommandWarning,  CommandTestSetup) {
  LOG(kInfo) << "Running CommandTestSuite/TestCommandWarning";

  warning_cmd.exec(params);
  
  do {
  } while ( (warning_cmd.getState() == ActionStatus::kScheduled) || (warning_cmd.getState() == ActionStatus::kRunning) );
  
  CommandStatus s = warning_cmd.getStatus();
  BOOST_CHECK_EQUAL(s.getState(), ActionStatus::kWarning);
  BOOST_CHECK_EQUAL(s.getProgress(), 1.0);
  BOOST_CHECK_EQUAL(s.getStatusMsg(), DummyWarningCommand::finalMsg);
  //BOOST_CHECK_EQUAL(s.getParameters().size(), size_t(0));
  BOOST_REQUIRE(s.getResult() != NULL);
  BOOST_CHECK_EQUAL(s.getResultAsString(), boost::lexical_cast<std::string>(DummyWarningCommand::defaultResult.value_));
}


BOOST_FIXTURE_TEST_CASE(TestCommandError,  CommandTestSetup) {
  LOG(kInfo) << "Running CommandTestSuite/TestCommandError";

  error_cmd.exec(params);
  
  do {
  } while ( (error_cmd.getState() == ActionStatus::kScheduled) || (error_cmd.getState() == ActionStatus::kRunning) );
  
  CommandStatus s = error_cmd.getStatus();
  BOOST_CHECK_EQUAL(s.getState(), ActionStatus::kError);
  BOOST_CHECK_EQUAL(s.getProgress(), DummyErrorCommand::finalProgress);
  BOOST_CHECK_EQUAL(s.getStatusMsg(), DummyErrorCommand::finalMsg);
  BOOST_CHECK_EQUAL(s.getParameters().size(), size_t(0));
  BOOST_REQUIRE(s.getResult() != NULL);
  BOOST_CHECK_EQUAL(s.getResultAsString(), boost::lexical_cast<std::string>(DummyErrorCommand::defaultResult.value_));
}


BOOST_FIXTURE_TEST_CASE(TestThrowingCommand,  CommandTestSetup) {
  LOG(kInfo) << "Running CommandTestSuite/TestThrowingCommand";

  throw_cmd.exec(params);
  
  do {
  } while ( (throw_cmd.getState() == ActionStatus::kScheduled) || (throw_cmd.getState() == ActionStatus::kRunning) );
  
  CommandStatus s = throw_cmd.getStatus();
  BOOST_CHECK_EQUAL(s.getState(), ActionStatus::kError);
  BOOST_CHECK_EQUAL(s.getProgress(), DummyThrowCommand::finalProgress);
  BOOST_CHECK_EQUAL(s.getStatusMsg(), "An exception of type 'std::runtime_error' was thrown in Command::code(): " + DummyThrowCommand::exceptionMsg);
  BOOST_CHECK_EQUAL(s.getParameters().size(), size_t(0));
  BOOST_REQUIRE(s.getResult() != NULL);
  BOOST_CHECK_EQUAL(s.getResultAsString(), boost::lexical_cast<std::string>(DummyThrowCommand::defaultResult.value_));
}


BOOST_FIXTURE_TEST_CASE(TestNullCommand,  CommandTestSetup) {
  
}


BOOST_FIXTURE_TEST_CASE(TestCommandResetCorrectly,  CommandTestSetup) {

}


//BOOST_FIXTURE_TEST_CASE(TestRunPrint,  CommandTestSetup) {
//  LOG(kInfo) << "Running CommandTestSuite/TestRunPrint";
//
//  cmd.exec( params );
//  do {
//  } while ( (print.getState() == swatch::core::Command::kScheduled) || (print.getState() == swatch::core::Command::kRunning) );
//
//
//  CommandStatus status = print.getStatus();
//  BOOST_CHECK_EQUAL(status.getProgress(), 1.0);
//  BOOST_REQUIRE_EQUAL(status.getState(), Command::kDone);
//  BOOST_CHECK_EQUAL(status.getResult()->type(), "int");
//  xdata::Integer result(*dynamic_cast<const xdata::Integer*>(status.getResult()));
//  BOOST_CHECK(result.equals(xdata::Integer(99)));
//}
//
//BOOST_FIXTURE_TEST_CASE(TestRunNothing,  CommandTestSetup) {
//  LOG(kInfo) << "Running CommandTestSuite/TestRunNothing";
//  nothing.exec( params );
//
//  do {
//  } while ( (nothing.getState() == swatch::core::Command::kScheduled) || (nothing.getState() == swatch::core::Command::kRunning) );
//
//  CommandStatus status = nothing.getStatus();
//  BOOST_CHECK_EQUAL(status.getProgress(), 1.0);
//  BOOST_CHECK_EQUAL(status.getState(), Command::kWarning);
//  BOOST_CHECK_EQUAL(status.getStatusMsg(), "Nothing was done");
//}
//
//BOOST_FIXTURE_TEST_CASE(TestRunError,  CommandTestSetup) {
//  LOG(kInfo) << "Running CommandTestSuite/TestRunError";
//  error.exec( params );
//
//  do {
//  } while ( (error.getState() == swatch::core::Command::kScheduled) || (error.getState() == swatch::core::Command::kRunning) );
//
//  CommandStatus status = error.getStatus();
//  BOOST_CHECK_CLOSE(status.getProgress(), 0.5049, 0.0001);
//  BOOST_CHECK_EQUAL(status.getState(), Command::kError);
//  BOOST_CHECK_EQUAL(status.getStatusMsg(), "But ended up in error");
//}

BOOST_AUTO_TEST_SUITE_END() // CommandTestSuite

} /* namespace test */
} /* namespace core */
} /* namespace swatch */

