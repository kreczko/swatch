#include <boost/foreach.hpp>
#include <xdata/Integer.h>

#include "swatch/core/Command.hpp"
#include "swatch/core/ActionableObject.hpp"
#include "swatch/logger/Log.hpp"
#include "swatch/core/xoperators.hpp"
#include "swatch/core/test/DummyCommand.hpp"
#include "swatch/core/test/DummyHandler.hpp"

namespace swlo = swatch::logger;
namespace swco = swatch::core;
namespace swcot = swatch::core::test;

int main(int argc, char const *argv[]) {
  swcot::DummyHandler handler;
  swco::XParameterSet params;

  LOG(swlo::kInfo) << "resource : smthg = " << handler.something() << " - num = " << handler.number(); 

  handler.Register<swcot::DummyCommand>("dummy");

  LOG(swlo::kNotice) << "Registered commands ";
  BOOST_FOREACH( const std::string& cmdname, handler.getCommands() ) {
    LOG(swlo::kInfo) << " * " << cmdname << ": result type = " << handler.getCommand("dummy")->getResult().type();
  }
  
  LOG(swlo::kInfo) << "dummy: " << handler.getCommand("dummy");

  swco::Command* cmd = handler.getCommand("dummy");

  LOG(swlo::kNotice) << " --- --- --- ";
  cmd->exec(params);


  LOG(swlo::kInfo) << "progress: " << cmd->getProgress() << " - " << cmd->getProgressMsg();
  LOG(swlo::kInfo) << "status: " << cmd->getStatus() << " - " << cmd->getStatusMsg();
  LOG(swlo::kInfo) << "result: " << cmd->getResult<xdata::Integer>();

  LOG(swlo::kInfo) << "resource : smthg = " << handler.something() << " - num = " << handler.number();

  LOG(swlo::kNotice) << " --- --- --- ";  
  cmd->reset();
  params.add("todo", xdata::String("print"));

  cmd->exec(params);

  LOG(swlo::kInfo) << "progress: " << cmd->getProgress() << " - " << cmd->getProgressMsg();
  LOG(swlo::kInfo) << "status: " << cmd->getStatus() << " - " << cmd->getStatusMsg();
  LOG(swlo::kInfo) << "result: " << cmd->getResult<xdata::Integer>();
  
  LOG(swlo::kInfo) << "resource : smthg = " << handler.something() << " - num = " << handler.number();

  LOG(swlo::kNotice) << " --- --- --- ";
  cmd->reset();
  params["todo"] = xdata::String("error");
  cmd->exec(params);

  LOG(swlo::kInfo) << "progress: " << cmd->getProgress() << " - " << cmd->getProgressMsg();
  LOG(swlo::kInfo) << "status: " << cmd->getStatus() << " - " << cmd->getStatusMsg();
  LOG(swlo::kInfo) << "result: " << cmd->getResult<xdata::Integer>();
  
  LOG(swlo::kInfo) << "resource : smthg = " << handler.something() << " - num = " << handler.number();

  /* code */
  return 0;
}
