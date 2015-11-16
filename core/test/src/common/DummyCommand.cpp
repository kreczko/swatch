#include "swatch/core/test/DummyCommand.hpp"


#include "boost/foreach.hpp"
#include "boost/chrono.hpp"

#include "xdata/Integer.h"
#include "xdata/String.h"

#include "swatch/core/xoperators.hpp"
#include "swatch/core/test/DummyActionableObject.hpp"
#include "swatch/logger/Log.hpp"


namespace swatch {
namespace core {
namespace test {

const xdata::Integer DummyCommand::defaultResult(-1);

const std::string DummyCommand::paramX("x");
const std::string DummyCommand::paramToDo("todo");

const std::string DummyCommand::finalMsgUseResource("Dummy command successfully used resource");
const std::string DummyCommand::finalMsgSleep("Dummy command finished sleeping");
 
  
DummyCommand::DummyCommand(const std::string& aId, ActionableObject& aActionable) :
  Command(aId, aActionable, defaultResult) {
  registerParameter(paramX, xdata::Integer(15));
  registerParameter(paramToDo, xdata::String(""));
}


DummyCommand::~DummyCommand() {
}


Command::State DummyCommand::code(const XParameterSet& params)
{
  DummyActionableObject& res = getActionable<DummyActionableObject>();

  std::string todo = params.parameterAsString("todo");
  setResult(params.get("x"));

  if (todo == "useResource") {

    res.setNumber(54);
    setProgress(0.1);
    
    setProgress(0.99, finalMsgUseResource);
    return State::kDone;
  } 
  if (todo == "sleep")
  {
    setProgress(0.01, "Dummy command just started");
    unsigned int milliseconds(params.get<xdata::Integer>("milliseconds").value_);

    for (unsigned int i = 0; i < milliseconds; ++i) {
      boost::this_thread::sleep_for(boost::chrono::milliseconds(1));
      setProgress(0.01 + 0.99 * float(i) / milliseconds, "Dummy command progressed");
    }

    setStatusMsg(finalMsgSleep);
    return State::kDone;
  }
  else
  {
    return State::kDone;
  }
}


//-----------------------//
/*  DummyWarningCommand  */

const xdata::Integer DummyWarningCommand::defaultResult(-1);
const std::string DummyWarningCommand::finalMsg("Dummy command did something, but ended up in warning");

DummyWarningCommand::DummyWarningCommand(const std::string& aId, ActionableObject& aActionable) :
  Command(aId, aActionable, defaultResult)
{
}


DummyWarningCommand::~DummyWarningCommand()
{
}


Command::State DummyWarningCommand::code(const XParameterSet& params)
{
  setProgress(0.5049, finalMsg);
  return State::kWarning;
}



//---------------------//
/*  DummyErrorCommand  */

const xdata::Integer DummyErrorCommand::defaultResult(-1);
const std::string DummyErrorCommand::finalMsg("Dummy command did something, but ended up in error");
const float DummyErrorCommand::finalProgress(0.5049);

DummyErrorCommand::DummyErrorCommand(const std::string& aId, ActionableObject& aActionable) :
  Command(aId, aActionable, defaultResult)
{
}

DummyErrorCommand::~DummyErrorCommand()
{
}

Command::State DummyErrorCommand::code(const XParameterSet& params)
{
  setProgress(finalProgress, finalMsg);
  return State::kError;
}


//---------------------//
/*  DummyThrowCommand  */

const xdata::Integer DummyThrowCommand::defaultResult(-1);
const std::string DummyThrowCommand::exceptionMsg("My test exception message");
const float DummyThrowCommand::finalProgress(0.4032);  

DummyThrowCommand::DummyThrowCommand(const std::string& aId, ActionableObject& aActionable) :
  Command(aId, aActionable, defaultResult)
{
}

DummyThrowCommand::~DummyThrowCommand()
{
}

Command::State DummyThrowCommand::code(const XParameterSet& params)
{
  setProgress(finalProgress);
  throw std::runtime_error(exceptionMsg);
}


} /* namespace test */
} /* namespace core */
} /* namespace swatch */
