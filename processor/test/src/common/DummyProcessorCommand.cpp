#include "swatch/logger/Log.hpp"
#include "swatch/core/xoperators.hpp"
#include "swatch/core/test/DummyHandler.hpp"
#include "swatch/processor/test/DummyProcessorCommand.hpp"
#include "swatch/processor/test/DummyProcessor.hpp"

#include <xdata/Integer.h>
#include <xdata/String.h>
#include <boost/foreach.hpp>

namespace swatch {
namespace processor {
namespace test {
DummyProcessorCommand::DummyProcessorCommand(const std::string aId) :
        core::Command(aId, xdata::String("")) {
  getParams().adopt("aa", new xdata::Integer(15));
  getParams().adopt("todo", new xdata::String(""));
}

DummyProcessorCommand::~DummyProcessorCommand() {
//  delete dummy_proc_;
}

void DummyProcessorCommand::code() {
  DummyProcessor* res = getParent<DummyProcessor>();

  std::string todo = getParams().get<xdata::String>("todo");
  if (todo == "getCrateId") {
    setProgress(90.49, "Dummy command did something");
    setResult(xdata::String(res->getCrateId()));
    setDone("Dummy command successfully completed");

  } else {
    //      setProgress(0.0);
    setProgress(0.0, "Not even started");
    setWarning("Nothing was done");

  }
}
} // namespace test
} // namespace processor
} // namespace swatch
