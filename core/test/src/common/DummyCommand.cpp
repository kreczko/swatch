#include "swatch/logger/Log.hpp"
#include "swatch/core/xoperators.hpp"
#include "swatch/core/test/DummyCommand.hpp"
#include "swatch/core/test/DummyHandler.hpp"

#include <xdata/Integer.h>
#include <xdata/String.h>
#include <boost/foreach.hpp>
#include <boost/chrono.hpp>
#include <unistd.h>

using namespace swatch;
namespace swatch {
namespace core {
namespace test {

DummyCommand::DummyCommand(const std::string& aId) :
        core::Command(aId, xdata::Integer(-33)) {

  registerParameter("aa", xdata::Integer(15));
  registerParameter("todo", xdata::String(""));
}

DummyCommand::~DummyCommand() {
//  delete dummy_proc_;
}

Command::State DummyCommand::code(XParameterSet& params) ///Should take const reference but xdata::serializable is const-correctness broken
{
  DummyHandler* res = getParent<DummyHandler>();

  BOOST_FOREACH(const std::string& n, params.keys()) {
    LOG(logger::kInfo) << n << " : " << getDefaultParams()[n];
  }

  std::string todo = params.get<xdata::String>("todo");
  if (todo == "print") {

    res->setSomething("|12345|");
    res->setNumber(54);
    setProgress(0.1);
    
    setResult(xdata::Integer(99));
    setProgress(0.99, "doing stuff");
    
    setStatusMsg("Dummy command successfully completed");
    return kDone;

  } 
  else if (todo == "error") 
  {
    setProgress(0.5049, "Dummy command did something");

    setStatusMsg("But ended up in error");
    return kError;
  }
  else if (todo == "thread")
  {
    setProgress(0.01, "Dummy command just started");
    unsigned int milliseconds(params.get<xdata::Integer>("milliseconds"));
    for (unsigned int i = 0; i < milliseconds; ++i) {
      boost::this_thread::sleep_for(boost::chrono::milliseconds(1));
//      usleep(1); // takes microseconds
      setProgress(0.01 + 0.99 * float(i) / milliseconds, "Dummy command progressed");
    }
    setStatusMsg("Thread command " + id() + " completed");
    return kDone;
  }
  else
  {
    setProgress(0.0, "Not even started");

    setStatusMsg("Nothing was done");
    return kWarning;
  }
}

} /* namespace test */
} /* namespace core */
} /* namespace swatch */
