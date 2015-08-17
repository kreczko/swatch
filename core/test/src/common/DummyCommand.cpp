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

const std::string DummyCommand::finalMsgUseResource("Dummy command successfully used resource");
const std::string DummyCommand::finalMsgSleep("Dummy command finished sleeping");
const std::string DummyCommand::finalMsgWarning("Dummy command did something, but ended up in warning");
const std::string DummyCommand::finalMsgError("Dummy command did something, but ended up in error");
const std::string DummyCommand::exceptionMsg("My test exception message");
const float DummyCommand::finalProgressError(0.5049);
const float DummyCommand::finalProgressThrow(0.4032);  

  
  
DummyCommand::DummyCommand(const std::string& aId) :
  Command(aId, defaultResult) {
  registerParameter("x", xdata::Integer(15));
  registerParameter("todo", xdata::String(""));
}


DummyCommand::~DummyCommand() {
}


Command::State DummyCommand::code(const XParameterSet& params)
{
  DummyActionableObject* res = getParent<DummyActionableObject>();

  std::string todo = params.parameterAsString("todo");
  if (todo == "useResource") {

    res->setNumber(54);
    setProgress(0.1);
    
    setResult(params.get("x"));
    setProgress(0.99, finalMsgUseResource);
    return kDone;
  } 
  if (todo == "sleep")
  {
    setProgress(0.01, "Dummy command just started");
    unsigned int milliseconds(params.get<xdata::Integer>("milliseconds").value_);

    for (unsigned int i = 0; i < milliseconds; ++i) {
      boost::this_thread::sleep_for(boost::chrono::milliseconds(1));
      setProgress(0.01 + 0.99 * float(i) / milliseconds, "Dummy command progressed");
    }

    setResult(params.get("x"));
    setStatusMsg(finalMsgSleep);
    return kDone;
  }
  else if (todo == "warning") 
  {
    setProgress(0.5049, finalMsgWarning);
    return kWarning;
  }
  else if (todo == "error") 
  {
    setProgress(finalProgressError, finalMsgError);
    return kError;
  }
  else if (todo == "throw")
  {
    setProgress(finalProgressThrow);
    throw std::runtime_error(exceptionMsg);
  }
  else
  {
    return kDone;
  }
}

} /* namespace test */
} /* namespace core */
} /* namespace swatch */
