#include "swatch/processor/test/DummyProcessorCommand.hpp"


#include "xdata/Integer.h"
#include "xdata/String.h"

#include "swatch/logger/Log.hpp"
#include "swatch/processor/test/DummyProcessor.hpp"


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
}

core::Command::State DummyProcessorCommand::code(const swco::XParameterSet& params)
{
  DummyProcessor* res = getParent<DummyProcessor>();

  std::string todo = params.get<xdata::String>("todo").value_;
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
