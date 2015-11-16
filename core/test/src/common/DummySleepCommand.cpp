
#include "swatch/core/test/DummySleepCommand.hpp"


#include "boost/chrono.hpp"

#include "xdata/UnsignedInteger.h"

#include "swatch/core/test/DummyActionableObject.hpp"
#include "swatch/logger/Log.hpp"


using namespace swatch;
namespace swatch {
namespace core {
namespace test {

DummySleepCommand::DummySleepCommand(const std::string& aId) :
  core::Command(aId, xdata::Integer(-33)) {
  registerParameter("n", xdata::UnsignedInteger(50));
  registerParameter("millisecPerSleep", xdata::UnsignedInteger(100));
}


DummySleepCommand::~DummySleepCommand() {
}


Command::State DummySleepCommand::code(const XParameterSet& params)
{
  DummyActionableObject* res = getParent<DummyActionableObject>();

  setStatusMsg("Dummy sleep command just started");

  unsigned int n(params.get<xdata::UnsignedInteger>("n").value_);
  unsigned int millisecPerSleep(params.get<xdata::UnsignedInteger>("millisecPerSleep").value_);
  
  for (unsigned int i = 0; i < n; i++) {
    boost::this_thread::sleep_for(boost::chrono::milliseconds(millisecPerSleep));
    
    std::ostringstream oss;
    oss << "Dummy sleep command for object '" << res->getPath() << "' progressed. " << i << " of " << n << " sleeps done; " << (n - i) * millisecPerSleep << " milli sec remain";
    setProgress( float(i) / n, oss.str());
  }
  
  setStatusMsg("Dummy sleep command for object '" + res->getPath() + "' completed");
    return State::kDone;
}

} /* namespace test */
} /* namespace core */
} /* namespace swatch */
