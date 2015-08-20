#include "swatch/processor/test/DummyProcessorCommands.hpp"


#include "xdata/Boolean.h"
#include "xdata/UnsignedInteger.h"

#include "swatch/processor/test/DummyProcessor.hpp"
#include "swatch/processor/test/DummyDriver.hpp"


namespace swco = swatch::core;

namespace swatch {
namespace processor {
namespace test {


//////////////////////////
/*  DummyRebootCommand  */

DummyRebootCommand::DummyRebootCommand(const std::string& aId) : 
  Command(aId, xdata::Boolean(true))
{
  registerParameter("cmdDuration", xdata::UnsignedInteger(10));
}

DummyRebootCommand::~DummyRebootCommand()
{
}

core::Command::State DummyRebootCommand::code(const swatch::core::XParameterSet& aParams)
{
  boost::this_thread::sleep_for(boost::chrono::seconds(aParams.get<xdata::UnsignedInteger>("cmdDuration").value_));

  DummyDriver& driver = getParent<DummyProcessor>()->getDriver();
  
  driver.reboot();
  
  return kDone;
}


/////////////////////////
/*  DummyResetCommand  */

DummyResetCommand::DummyResetCommand(const std::string& aId) : 
  Command(aId, xdata::Boolean(true))
{
  registerParameter("clkErrorTimeout", xdata::UnsignedInteger(60));
  registerParameter("cmdDuration", xdata::UnsignedInteger(10));
}

DummyResetCommand::~DummyResetCommand()
{
}

core::Command::State DummyResetCommand::code(const swatch::core::XParameterSet& aParams)
{
  boost::this_thread::sleep_for(boost::chrono::seconds(aParams.get<xdata::UnsignedInteger>("cmdDuration").value_));

  DummyDriver& driver = getParent<DummyProcessor>()->getDriver();
  
  size_t timeout = aParams.get<xdata::UnsignedInteger>("clkErrorTimeout").value_;

  driver.reset(timeout);
  
  return kDone;
}


///////////////////////////////
/*  DummyConfigureTxCommand  */

DummyConfigureTxCommand::DummyConfigureTxCommand(const std::string& aId) : 
  Command(aId, xdata::Boolean(true))
{
  registerParameter("txErrorTimeout", xdata::UnsignedInteger(60));
  registerParameter("cmdDuration", xdata::UnsignedInteger(10));
}

DummyConfigureTxCommand::~DummyConfigureTxCommand()
{
}

core::Command::State DummyConfigureTxCommand::code(const swatch::core::XParameterSet& aParams)
{
  boost::this_thread::sleep_for(boost::chrono::seconds(aParams.get<xdata::UnsignedInteger>("cmdDuration").value_));

  DummyDriver& driver = getParent<DummyProcessor>()->getDriver();
  
  size_t timeout = aParams.get<xdata::UnsignedInteger>("txErrorTimeout").value_;

  driver.configureTxPorts(timeout);
  
  return kDone;
}


///////////////////////////////
/*  DummyConfigureRxCommand  */

DummyConfigureRxCommand::DummyConfigureRxCommand(const std::string& aId) : 
  Command(aId, xdata::Boolean(true))
{
  registerParameter("rxErrorTimeout", xdata::UnsignedInteger(60));
  registerParameter("cmdDuration", xdata::UnsignedInteger(10));
}

DummyConfigureRxCommand::~DummyConfigureRxCommand()
{
}

core::Command::State DummyConfigureRxCommand::code(const swatch::core::XParameterSet& aParams)
{
  boost::this_thread::sleep_for(boost::chrono::seconds(aParams.get<xdata::UnsignedInteger>("cmdDuration").value_));

  DummyDriver& driver = getParent<DummyProcessor>()->getDriver();
  
  size_t timeout = aParams.get<xdata::UnsignedInteger>("rxErrorTimeout").value_;

  driver.configureRxPorts(timeout);
  
  return kDone;
}


////////////////////////////////
/*  DummyConfigureDaqCommand  */

DummyConfigureDaqCommand::DummyConfigureDaqCommand(const std::string& aId) : 
  Command(aId, xdata::Boolean(true))
{
  registerParameter("daqErrorTimeout", xdata::UnsignedInteger(60));
  registerParameter("cmdDuration", xdata::UnsignedInteger(10));
}

DummyConfigureDaqCommand::~DummyConfigureDaqCommand()
{
}

core::Command::State DummyConfigureDaqCommand::code(const swatch::core::XParameterSet& aParams)
{
  boost::this_thread::sleep_for(boost::chrono::seconds(aParams.get<xdata::UnsignedInteger>("cmdDuration").value_));

  DummyDriver& driver = getParent<DummyProcessor>()->getDriver();
  
  size_t timeout = aParams.get<xdata::UnsignedInteger>("daqErrorTimeout").value_;

  driver.configureReadout(timeout);
  
  return kDone;
}


/////////////////////////////////
/*  DummyConfigureAlgoCommand  */

DummyConfigureAlgoCommand::DummyConfigureAlgoCommand(const std::string& aId) : 
  Command(aId, xdata::Boolean(true))
{
  registerParameter("algoErrorTimeout", xdata::UnsignedInteger(60));
  registerParameter("cmdDuration", xdata::UnsignedInteger(10));
}

DummyConfigureAlgoCommand::~DummyConfigureAlgoCommand()
{
}

core::Command::State DummyConfigureAlgoCommand::code(const swatch::core::XParameterSet& aParams)
{
  boost::this_thread::sleep_for(boost::chrono::seconds(aParams.get<xdata::UnsignedInteger>("cmdDuration").value_));

  DummyDriver& driver = getParent<DummyProcessor>()->getDriver();
  
  size_t timeout = aParams.get<xdata::UnsignedInteger>("algoErrorTimeout").value_;

  driver.configureAlgo(timeout);
  
  return kDone;
}


} // namespace test
} // namespace processor
} // namespace swatch
