#include "swatch/logger/Log.hpp"
#include "swatch/core/xoperators.hpp"
#include "swatch/core/test/DummyHandler.hpp"
#include "swatch/processor/test/DummyProcessor.hpp"

#include <xdata/Integer.h>
#include <xdata/String.h>
#include <boost/foreach.hpp>
#include "../../include/swatch/processor/test/DummyProcessorOperation.hpp"
#include "swatch/core/FSM.hpp"

namespace swatch {
namespace processor {
namespace test {
DummyProcessorOperation::DummyProcessorOperation(const std::string& aId) :
        core::Operation(aId) {
  getParams().adopt("aa", new xdata::Integer(15));
  getParams().adopt("todo", new xdata::String(""));
  std::string halted("HALTED");
  std::string tested("TESTED");

  fsm_->addState(halted);
  fsm_->addState(tested);
  fsm_->setInitialState(halted);
  fsm_->reset();

  fsm_->addStateTransition<DummyProcessorOperation>(halted, tested, "test", this, &DummyProcessorOperation::test);
}

DummyProcessorOperation::~DummyProcessorOperation() {
//  delete dummy_proc_;
}

void DummyProcessorOperation::test(toolbox::Event::Reference event) {
  DummyProcessor* res = getParent<DummyProcessor>();
  res->test1();
  res->test2();
}

} // namespace test
} // namespace processor
} // namespace swatch
