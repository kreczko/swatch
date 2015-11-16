
#include "swatch/dummy/DummyAMC13ManagerCommands.hpp"


// boost headers
#include "boost/thread/v2/thread.hpp"

// XDAQ headers
#include "xdata/Boolean.h"
#include "xdata/UnsignedInteger.h"

// SWATCH headers
#include "swatch/dummy/DummyAMC13Manager.hpp"
#include "swatch/dummy/DummyAMC13Driver.hpp"



namespace swatch {
namespace dummy {


/////////////////////////
/*  DummyAMC13Command  */

DummyAMC13Command::DummyAMC13Command(const std::string& aId) : 
  Command(aId, xdata::Boolean(true))
{
  registerParameter("cmdDuration", xdata::UnsignedInteger(10));
}

DummyAMC13Command::~DummyAMC13Command()
{
}

void DummyAMC13Command::sleep(const core::XParameterSet& aParams)
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


///////////////////////////////
/*  DummyAMC13RebootCommand  */

DummyAMC13RebootCommand::DummyAMC13RebootCommand(const std::string& aId) : 
  DummyAMC13Command(aId)
{
}

DummyAMC13RebootCommand::~DummyAMC13RebootCommand()
{
}

core::Command::State DummyAMC13RebootCommand::code(const swatch::core::XParameterSet& aParams)
{ 
  sleep(aParams);

  DummyAMC13Driver& driver = getActionable<DummyAMC13Manager>()->getDriver();
  
  driver.reboot();
  
  return State::kDone;
}


//////////////////////////////
/*  DummyAMC13ResetCommand  */

DummyAMC13ResetCommand::DummyAMC13ResetCommand(const std::string& aId) : 
  DummyAMC13Command(aId)
{
  registerParameter("clkErrTimeout", xdata::UnsignedInteger(60));
  registerParameter("clkWrnTimeout", xdata::UnsignedInteger(45));
}

DummyAMC13ResetCommand::~DummyAMC13ResetCommand()
{
}

core::Command::State DummyAMC13ResetCommand::code(const swatch::core::XParameterSet& aParams)
{
  sleep(aParams);

  DummyAMC13Driver& driver = getActionable<DummyAMC13Manager>()->getDriver();
  
  size_t lWrnTimeout = aParams.get<xdata::UnsignedInteger>("clkWrnTimeout").value_;
  size_t lErrTimeout = aParams.get<xdata::UnsignedInteger>("clkErrTimeout").value_;

  driver.reset(lWrnTimeout, lErrTimeout);
  
  return State::kDone;
}


////////////////////////////////
/*  DummyConfigureDaqCommand  */

DummyAMC13ConfigureDaqCommand::DummyAMC13ConfigureDaqCommand(const std::string& aId) : 
  DummyAMC13Command(aId)
{
}

DummyAMC13ConfigureDaqCommand::~DummyAMC13ConfigureDaqCommand()
{
}

core::Command::State DummyAMC13ConfigureDaqCommand::code(const swatch::core::XParameterSet& aParams)
{
  sleep(aParams);
  
  DummyAMC13Manager* mgr = getActionable<DummyAMC13Manager>();

  mgr->getDriver().configureDaq(mgr->getStub().fedId);
  
  return State::kDone;
}


////////////////////////////
/*  DummyStartDaqCommand  */

DummyAMC13StartDaqCommand::DummyAMC13StartDaqCommand(const std::string& aId) : 
  DummyAMC13Command(aId)
{
  registerParameter("daqErrTimeout", xdata::UnsignedInteger(60));
  registerParameter("daqWrnTimeout", xdata::UnsignedInteger(45));

}

DummyAMC13StartDaqCommand::~DummyAMC13StartDaqCommand()
{
}

core::Command::State DummyAMC13StartDaqCommand::code(const swatch::core::XParameterSet& aParams)
{
  sleep(aParams);

  DummyAMC13Driver& driver = getActionable<DummyAMC13Manager>()->getDriver();
  
  size_t lWrnTimeout = aParams.get<xdata::UnsignedInteger>("daqWrnTimeout").value_;
  size_t lErrTimeout = aParams.get<xdata::UnsignedInteger>("daqErrTimeout").value_;

  driver.startDaq(lWrnTimeout, lErrTimeout);
  
  return State::kDone;
}


////////////////////////////
/*  DummyStartDaqCommand  */

DummyAMC13StopDaqCommand::DummyAMC13StopDaqCommand(const std::string& aId) : 
  DummyAMC13Command(aId)
{
}

DummyAMC13StopDaqCommand::~DummyAMC13StopDaqCommand()
{
}

core::Command::State DummyAMC13StopDaqCommand::code(const swatch::core::XParameterSet& aParams)
{
  sleep(aParams);

  return State::kDone;
}


} // namespace dummy
} // namespace swatch
