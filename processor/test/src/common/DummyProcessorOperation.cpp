
#include "swatch/processor/test/DummyProcessorOperation.hpp"


#include <xdata/Integer.h>
#include <xdata/String.h>

#include "swatch/logger/Log.hpp"
#include "swatch/core/FSM.hpp"
#include "swatch/processor/test/DummyProcessor.hpp"


namespace swatch {
namespace processor {
namespace test {

DummyProcessorOperation::DummyProcessorOperation(const std::string& aId) :
  DummyOperation(aId) {
  // since DummyOperation sets states, lets clear it all up
  fsm_->clear();
  std::string halted("HALTED");
  std::string tested("TESTED");

  fsm_->addState(halted);
  fsm_->addState(tested);
  fsm_->setInitialState(halted);
  fsm_->reset();

  fsm_->addStateTransition<DummyProcessorOperation>(halted, tested, "test", this, &DummyProcessorOperation::test);
}


DummyProcessorOperation::~DummyProcessorOperation() {
}


void DummyProcessorOperation::test(toolbox::Event::Reference event) {
  DummyProcessor* res = getParent<DummyProcessor>();
  res->test1();
  res->test2();
}

} // namespace test
} // namespace processor
} // namespace swatch
