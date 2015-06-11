#include "swatch/logger/Log.hpp"
#include "swatch/core/xoperators.hpp"
#include "swatch/core/test/DummyHandler.hpp"

#include "swatch/core/test/DummyStandardOperation.hpp"
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

DummyStandardOperation::DummyStandardOperation(  const std::string& aId ) :
    swatch::core::StandardOperation( aId ) {

  fsm_->addTransition(Halted,     Halted,     ColdReset , this, &DummyStandardOperation::check_true_example, &DummyStandardOperation::coldReset);
  fsm_->addTransition(Halted,     Configured, Configure , this, &DummyStandardOperation::check_true_example, &DummyStandardOperation::configure);
  fsm_->addTransition(Configured, Enabled,    Enable ,    this, &DummyStandardOperation::check_true_example, &DummyStandardOperation::enable);
  fsm_->addTransition(Enabled,    Suspended,  Suspend,   this, &DummyStandardOperation::check_true_example, &DummyStandardOperation::suspend);
  fsm_->addTransition(Enabled,    Configured, Stop,      this, &DummyStandardOperation::check_true_example, &DummyStandardOperation::stop);
  fsm_->addTransition(Suspended,  Enabled,    Enable,    this, &DummyStandardOperation::check_true_example, &DummyStandardOperation::enable);
  fsm_->addTransition(Suspended,  Configured, Stop,      this, &DummyStandardOperation::check_true_example, &DummyStandardOperation::stop);
  // special ones
//  fsm_->addTransition(halted, failed, "fail", this, &DummyStandardOperation::fail);
  fsm_->addTransition( Halted, Failed , "FailWithException" , this,  &DummyStandardOperation::check_true_example, &DummyStandardOperation::failWithException);
}

DummyStandardOperation::~DummyStandardOperation() {
//  delete dummy_proc_;
}

void DummyStandardOperation::configure() {
  DummyHandler* res = getParent<DummyHandler>();
  res->setNumber(42);
  res->setSomething("I have been configured");
}

void DummyStandardOperation::coldReset() {
  DummyHandler* res = getParent<DummyHandler>();
  res->setSomething("I am cold...");
}

void DummyStandardOperation::enable() {
  DummyHandler* res = getParent<DummyHandler>();
  res->setSomething("I am enabled...");
}

void DummyStandardOperation::stop() {
  DummyHandler* res = getParent<DummyHandler>();
  res->setSomething("somebody stop me...");
}

void DummyStandardOperation::suspend() {
  DummyHandler* res = getParent<DummyHandler>();
  res->setSomething("I have been suspended!");
}

void DummyStandardOperation::fail() {
  DummyHandler* res = getParent<DummyHandler>();
  res->fail();
}

void DummyStandardOperation::failWithException() {
  std::string line = boost::lexical_cast<std::string>(static_cast<long long>(__LINE__));
  throw swatch::core::exception("as expected, I failed." + std::string(__FILE__) + line + std::string(__FUNCTION__) );
}

bool DummyStandardOperation::check_true_example(){
  return true;
}

bool DummyStandardOperation::check_false_example(){
  return false;
}

} /* namespace test */
} /* namespace core */
} /* namespace swatch */
