/**
 * @file    MP7Operations.hpp
 * @author  Alessandro Thea
 * @data    MArch 2015
 * 
 */

#include "swatch/hardware/MP7Operations.hpp"

// Swatch headers
#include "swatch/hardware/MP7Processor.hpp"
#include "swatch/hardware/MP7Commands.hpp"

namespace swatch {
namespace hardware {

MP7Configure::MP7Configure(swatch::core::ActionHandler* resource) : swatch::core::Operation(resource) {

  std::string halted("Halted");
  std::string configured("Configured");
  std::string enabled("Enabled");
  std::string suspended("Suspended");
  std::string failed("Failed");

  fsm_->addState(halted);
  fsm_->addState(configured);
  fsm_->addState(enabled);
  fsm_->addState(suspended);
  fsm_->addState(failed);
  fsm_->setInitialState(halted);
  fsm_->reset();

  fsm_->addTransition<MP7Configure>(halted, halted, "coldReset", this, &MP7Configure::pass, &MP7Configure::coldReset);
  fsm_->addTransition<MP7Configure>(halted, configured, "configure", this, &MP7Configure::pass, &MP7Configure::configure);
  fsm_->addTransition<MP7Configure>(configured, enabled, "enable", this, &MP7Configure::pass, &MP7Configure::enable);
  fsm_->addTransition<MP7Configure>(enabled, suspended, "suspend", this, &MP7Configure::pass, &MP7Configure::suspend);
  fsm_->addTransition<MP7Configure>(enabled, configured, "stop", this, &MP7Configure::pass, &MP7Configure::stop);
  fsm_->addTransition<MP7Configure>(suspended, enabled, "enable", this, &MP7Configure::pass, &MP7Configure::enable);
  fsm_->addTransition<MP7Configure>(suspended, configured, "stop", this, &MP7Configure::pass, &MP7Configure::stop);
}



//---
MP7Configure::~MP7Configure() {

}

bool
MP7Configure::pass(){
  return true;
}

//---
void
MP7Configure::coldReset() {
  MP7Processor* res = getResource<MP7Processor>();

  swatch::core::Command* reset = res->getCommand("reset");
  reset->exec();
}

//---
void
MP7Configure::configure() {
  MP7Processor* res = getResource<MP7Processor>();
}

//---
void
MP7Configure::enable() {
  MP7Processor* res = getResource<MP7Processor>();
}

//---
void
MP7Configure::suspend() {
  MP7Processor* res = getResource<MP7Processor>();
}

//---
void
MP7Configure::stop() {
  MP7Processor* res = getResource<MP7Processor>();
}

} /* namespace hardware */
} /* namespace swatch */ 