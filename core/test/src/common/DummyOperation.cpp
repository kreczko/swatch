#include "swatch/logger/Log.hpp"
#include "swatch/core/xoperators.hpp"
#include "swatch/core/test/DummyActionableObject.hpp"

#include "swatch/core/test/DummyOperation.hpp"
#include "swatch/core/exception.hpp"
#include "toolbox/fsm/exception/Exception.h"

// C++ Headers
#include <string>

// Boosr Headers
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

// XDAQ Headers
#include <xdata/Integer.h>
#include <xdata/String.h>

using namespace swatch;
namespace swatch {
namespace core {
namespace test {

DummyOperation::DummyOperation(  const std::string& aId ) :
    swatch::core::Operation( aId ) {
//   assert(handler != 0);

  std::string halted("HALTED");
  std::string configured("CONFIGURED");
  std::string enabled("ENABLED");
  std::string suspended("SUSPENDED");
  std::string failed("FAILED");

  fsm_->addState(halted);
  fsm_->addState(configured);
  fsm_->addState(enabled);
  fsm_->addState(suspended);
  fsm_->addState(failed);
  fsm_->setInitialState(halted);
  fsm_->reset();

  fsm_->addTransition<DummyOperation>(halted, halted, "coldReset", this, &DummyOperation::check_true_example, &DummyOperation::coldReset);
  fsm_->addTransition<DummyOperation>(halted, configured, "configure", this,      &DummyOperation::check_true_example, &DummyOperation::configure);
  fsm_->addTransition<DummyOperation>(configured, enabled, "enable", this, &DummyOperation::check_true_example, &DummyOperation::enable);
  fsm_->addTransition<DummyOperation>(enabled, suspended, "suspend", this, &DummyOperation::check_true_example, &DummyOperation::suspend);
  fsm_->addTransition<DummyOperation>(enabled, configured, "stop", this, &DummyOperation::check_true_example, &DummyOperation::stop);
  fsm_->addTransition<DummyOperation>(suspended, enabled, "enable", this, &DummyOperation::check_true_example, &DummyOperation::enable);
  fsm_->addTransition<DummyOperation>(suspended, configured, "stop", this, &DummyOperation::check_true_example, &DummyOperation::stop);
  // special ones
//  fsm_->addTransition<DummyOperation>(halted, failed, "fail", this, &DummyOperation::fail);
  fsm_->addTransition<DummyOperation>(halted, failed, "failWithException", this,
      &DummyOperation::check_true_example, &DummyOperation::failWithException);
}

DummyOperation::~DummyOperation() {
//  delete dummy_proc_;
}

void DummyOperation::configure() {
  DummyActionableObject* res = getParent<DummyActionableObject>();
  res->setNumber(42);
  res->setSomething("I have been configured");
}

void DummyOperation::coldReset() {
  DummyActionableObject* res = getParent<DummyActionableObject>();
  res->setSomething("I am cold...");
}

void DummyOperation::enable() {
  DummyActionableObject* res = getParent<DummyActionableObject>();
  res->setSomething("I am enabled...");
}

void DummyOperation::stop() {
  DummyActionableObject* res = getParent<DummyActionableObject>();
  res->setSomething("somebody stop me...");
}

void DummyOperation::suspend() {
  DummyActionableObject* res = getParent<DummyActionableObject>();
  res->setSomething("I have been suspended!");
}

void DummyOperation::fail() {
  DummyActionableObject* res = getParent<DummyActionableObject>();
  res->fail();
}

void DummyOperation::failWithException() {
  std::string line = boost::lexical_cast<std::string>(static_cast<long long>(__LINE__));
  throw swatch::core::exception("as expected, I failed." + std::string(__FILE__) + line + std::string(__FUNCTION__) );
}

bool DummyOperation::check_true_example(){
  return true;
}

bool DummyOperation::check_false_example(){
  return false;
}

} /* namespace test */
} /* namespace core */
} /* namespace swatch */
