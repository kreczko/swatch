#include "swatch/system/test/DummyAMC13ManagerCommands.hpp"

// boost headers
#include "boost/thread/v2/thread.hpp"

// XDAQ headers
#include "xdata/Boolean.h"
#include "xdata/UnsignedInteger.h"

// SWATCH headers
#include "swatch/system/test/DummyAMC13Manager.hpp"
#include "swatch/system/test/DummyAMC13Driver.hpp"


namespace swatch {
namespace system {
namespace test {


///////////////////////////////
/*  DummyAMC13RebootCommand  */

DummyAMC13RebootCommand::DummyAMC13RebootCommand(const std::string& aId) : 
  Command(aId, xdata::Boolean(true))
{
  registerParameter("cmdDuration", xdata::UnsignedInteger(10));
}

DummyAMC13RebootCommand::~DummyAMC13RebootCommand()
{
}

core::Command::State DummyAMC13RebootCommand::code(const swatch::core::XParameterSet& aParams)
{ 
  boost::this_thread::sleep_for(boost::chrono::seconds(aParams.get<xdata::UnsignedInteger>("cmdDuration").value_));

  DummyAMC13Driver& driver = getParent<DummyAMC13Manager>()->getDriver();
  
  driver.reboot();
  
  return kDone;
}


//////////////////////////////
/*  DummyAMC13ResetCommand  */

DummyAMC13ResetCommand::DummyAMC13ResetCommand(const std::string& aId) : 
  Command(aId, xdata::Boolean(true))
{
  registerParameter("clkErrorTimeout", xdata::UnsignedInteger(60));
  registerParameter("cmdDuration", xdata::UnsignedInteger(10));
}

DummyAMC13ResetCommand::~DummyAMC13ResetCommand()
{
}

core::Command::State DummyAMC13ResetCommand::code(const swatch::core::XParameterSet& aParams)
{
  boost::this_thread::sleep_for(boost::chrono::seconds(aParams.get<xdata::UnsignedInteger>("cmdDuration").value_));

  DummyAMC13Driver& driver = getParent<DummyAMC13Manager>()->getDriver();
  
  size_t timeout = aParams.get<xdata::UnsignedInteger>("clkErrorTimeout").value_;

  driver.reset(timeout);
  
  return kDone;
}


////////////////////////////////
/*  DummyConfigureDaqCommand  */

DummyAMC13ConfigureDaqCommand::DummyAMC13ConfigureDaqCommand(const std::string& aId) : 
  Command(aId, xdata::Boolean(true))
{
  registerParameter("cmdDuration", xdata::UnsignedInteger(10));
}

DummyAMC13ConfigureDaqCommand::~DummyAMC13ConfigureDaqCommand()
{
}

core::Command::State DummyAMC13ConfigureDaqCommand::code(const swatch::core::XParameterSet& aParams)
{
  boost::this_thread::sleep_for(boost::chrono::seconds(aParams.get<xdata::UnsignedInteger>("cmdDuration").value_));

  DummyAMC13Manager* mgr = getParent<DummyAMC13Manager>();

  mgr->getDriver().configureDaq(mgr->getStub().fedId);
  
  return kDone;
}


/////////////////////////////
/*  DummyEnableDaqCommand  */

DummyAMC13EnableDaqCommand::DummyAMC13EnableDaqCommand(const std::string& aId) : 
  Command(aId, xdata::Boolean(true))
{
  registerParameter("daqErrorTimeout", xdata::UnsignedInteger(60));
  registerParameter("cmdDuration", xdata::UnsignedInteger(10));
}

DummyAMC13EnableDaqCommand::~DummyAMC13EnableDaqCommand()
{
}

core::Command::State DummyAMC13EnableDaqCommand::code(const swatch::core::XParameterSet& aParams)
{
  boost::this_thread::sleep_for(boost::chrono::seconds(aParams.get<xdata::UnsignedInteger>("cmdDuration").value_));

  DummyAMC13Driver& driver = getParent<DummyAMC13Manager>()->getDriver();
  
  size_t timeout = aParams.get<xdata::UnsignedInteger>("daqErrorTimeout").value_;

  driver.enableDaq(timeout);
  
  return kDone;
}


} // namespace test
} // namespace processor
} // namespace swatch
