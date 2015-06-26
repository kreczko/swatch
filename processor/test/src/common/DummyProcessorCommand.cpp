#include "swatch/logger/Log.hpp"
#include "swatch/core/xoperators.hpp"
#include "swatch/core/test/DummyHandler.hpp"
#include "swatch/processor/test/DummyProcessorCommand.hpp"
#include "swatch/processor/test/DummyProcessor.hpp"

#include <xdata/Integer.h>
#include <xdata/String.h>
#include <boost/foreach.hpp>

namespace swco = swatch::core;

namespace swatch {
namespace processor {
namespace test {
DummyProcessorCommand::DummyProcessorCommand(const std::string& aId) :
        DummyCommand(aId, xdata::String("")) {
  registerParameter("aa", xdata::Integer(15));
  registerParameter("todo", xdata::String(""));
}

DummyProcessorCommand::~DummyProcessorCommand() {
//  delete dummy_proc_;
}

core::Command::State DummyProcessorCommand::code(swco::XParameterSet& params) ///Should take const reference but xdata::serializable is const-correctness broken
{
  DummyProcessor* res = getParent<DummyProcessor>();

  std::string todo = params.get<xdata::String>("todo");
  if (todo == "getCrateId") {
    setProgress(0.9049, "Dummy command did something");
    setResult(xdata::String(res->getCrateId()));
    
    setStatusMsg("Dummy command successfully completed");
    return kDone;
  } else {
    //      setProgress(0.0);
    setProgress(0.0, "Not even started");
    
    setStatusMsg("Nothing was done");
    return kWarning;
  }
}
} // namespace test
} // namespace processor
} // namespace swatch
