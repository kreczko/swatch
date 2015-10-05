
#include "swatch/dummy/DummyProcessorCommands.hpp"


#include "xdata/Boolean.h"
#include "xdata/UnsignedInteger.h"

#include "swatch/dummy/DummyProcessor.hpp"
#include "swatch/dummy/DummyProcDriver.hpp"


namespace swco = swatch::core;

namespace swatch {
namespace dummy {


////////////////////////
/*  DummyProcCommand  */

DummyProcCommand::DummyProcCommand(const std::string& aId) : 
  Command(aId, xdata::Boolean(true))
{
  registerParameter("cmdDuration", xdata::UnsignedInteger(10));
}

DummyProcCommand::~DummyProcCommand() {}


void DummyProcCommand::sleep(const core::XParameterSet& aParams)
{  
  const size_t nrSeconds = aParams.get<xdata::UnsignedInteger>("cmdDuration").value_;

  for(size_t i=0; i<(nrSeconds*4); i++)
  {
    std::ostringstream oss;
    oss << "Done " << i << " of " << (nrSeconds*4) << " things";
    setProgress(float(i)/float(nrSeconds*4), oss.str());
    boost::this_thread::sleep_for(boost::chrono::milliseconds(250));
  }
}


//////////////////////////
/*  DummyRebootCommand  */

DummyRebootCommand::DummyRebootCommand(const std::string& aId) : 
  DummyProcCommand(aId)
{
}

DummyRebootCommand::~DummyRebootCommand()
{
}

core::Command::State DummyRebootCommand::code(const swatch::core::XParameterSet& aParams)
{
  sleep( aParams );
  
  DummyProcDriver& driver = getParent<DummyProcessor>()->getDriver();
  
  driver.reboot();
  
  return kDone;
}


/////////////////////////
/*  DummyResetCommand  */

DummyResetCommand::DummyResetCommand(const std::string& aId) : 
  DummyProcCommand(aId)
{
  registerParameter("clkErrorTimeout", xdata::UnsignedInteger(60));
}

DummyResetCommand::~DummyResetCommand()
{
}

core::Command::State DummyResetCommand::code(const swatch::core::XParameterSet& aParams)
{
  sleep( aParams );
  
  DummyProcDriver& driver = getParent<DummyProcessor>()->getDriver();
  
  size_t timeout = aParams.get<xdata::UnsignedInteger>("clkErrorTimeout").value_;

  driver.reset(timeout);
  
  return kDone;
}


///////////////////////////////
/*  DummyConfigureTxCommand  */

DummyConfigureTxCommand::DummyConfigureTxCommand(const std::string& aId) : 
  DummyProcCommand(aId)
{
  registerParameter("txErrorTimeout", xdata::UnsignedInteger(60));
}

DummyConfigureTxCommand::~DummyConfigureTxCommand()
{
}

core::Command::State DummyConfigureTxCommand::code(const swatch::core::XParameterSet& aParams)
{
  sleep( aParams );

  DummyProcDriver& driver = getParent<DummyProcessor>()->getDriver();
  
  size_t timeout = aParams.get<xdata::UnsignedInteger>("txErrorTimeout").value_;

  driver.configureTxPorts(timeout);
  
  return kDone;
}


///////////////////////////////
/*  DummyConfigureRxCommand  */

DummyConfigureRxCommand::DummyConfigureRxCommand(const std::string& aId) : 
  DummyProcCommand(aId)
{
  registerParameter("rxErrorTimeout", xdata::UnsignedInteger(60));
}

DummyConfigureRxCommand::~DummyConfigureRxCommand()
{
}

core::Command::State DummyConfigureRxCommand::code(const swatch::core::XParameterSet& aParams)
{
  sleep( aParams );
  
  DummyProcDriver& driver = getParent<DummyProcessor>()->getDriver();
  
  size_t timeout = aParams.get<xdata::UnsignedInteger>("rxErrorTimeout").value_;

  driver.configureRxPorts(timeout);
  
  return kDone;
}


////////////////////////////////
/*  DummyConfigureDaqCommand  */

DummyConfigureDaqCommand::DummyConfigureDaqCommand(const std::string& aId) : 
  DummyProcCommand(aId)
{
  registerParameter("daqErrorTimeout", xdata::UnsignedInteger(60));
}

DummyConfigureDaqCommand::~DummyConfigureDaqCommand()
{
}

core::Command::State DummyConfigureDaqCommand::code(const swatch::core::XParameterSet& aParams)
{
  sleep( aParams );
  
  DummyProcDriver& driver = getParent<DummyProcessor>()->getDriver();
  
  size_t timeout = aParams.get<xdata::UnsignedInteger>("daqErrorTimeout").value_;

  driver.configureReadout(timeout);
  
  return kDone;
}


/////////////////////////////////
/*  DummyConfigureAlgoCommand  */

DummyConfigureAlgoCommand::DummyConfigureAlgoCommand(const std::string& aId) : 
  DummyProcCommand(aId)
{
  registerParameter("algoErrorTimeout", xdata::UnsignedInteger(60));
}

DummyConfigureAlgoCommand::~DummyConfigureAlgoCommand()
{
}

core::Command::State DummyConfigureAlgoCommand::code(const swatch::core::XParameterSet& aParams)
{
  sleep( aParams );
  
  DummyProcDriver& driver = getParent<DummyProcessor>()->getDriver();
  
  size_t timeout = aParams.get<xdata::UnsignedInteger>("algoErrorTimeout").value_;

  driver.configureAlgo(timeout);
  
  return kDone;
}


} // namespace dummy
} // namespace swatch
