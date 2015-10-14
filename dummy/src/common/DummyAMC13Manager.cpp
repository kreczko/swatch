
#include "swatch/dummy/DummyAMC13Manager.hpp"


// boost headers
#include "boost/foreach.hpp"

// SWATCH headers
#include "swatch/logger/Log.hpp"
#include "swatch/core/Factory.hpp"
#include "swatch/core/StateMachine.hpp"
#include "swatch/dtm/DaqTTCStub.hpp"
#include "swatch/dummy/DummyAMC13Driver.hpp"
#include "swatch/dummy/DummyAMC13ManagerCommands.hpp"


SWATCH_REGISTER_CLASS(swatch::dummy::DummyAMC13Manager);


namespace swatch {
namespace dummy {


DummyAMC13Manager::DummyAMC13Manager( const swatch::core::AbstractStub& aStub ) : 
  dtm::DaqTTCManager(aStub),
  driver_(new DummyAMC13Driver())
{
  // 1) Commands
  core::Command& reboot = registerFunctionoid<DummyAMC13RebootCommand>("reboot");
  core::Command& reset = registerFunctionoid<DummyAMC13ResetCommand>("reset");
  core::Command& cfgDaq = registerFunctionoid<DummyAMC13ConfigureDaqCommand>("configureDaq");
  core::Command& startDaq = registerFunctionoid<DummyAMC13StartDaqCommand>("startDaq");
  core::Command& stopDaq = registerFunctionoid<DummyAMC13StopDaqCommand>("stopDaq");
  
  // 2) Command sequences
  //registerFunctionoid<DaqTTCMgrCommandSequence>("resetAndConfigure").run(reset).then(configureDaq);
  
  
  dtm::RunControlFSM& lFSM = getRunControlFSM();
  lFSM.coldReset.add(reboot);
  lFSM.clockSetup.add(reset);
  lFSM.cfgDaq.add(cfgDaq);
  lFSM.start.add(startDaq);
  //lFSM.pause;
  //lFSM.resume;
  lFSM.stopFromPaused.add(stopDaq);
  lFSM.stopFromRunning.add(stopDaq);
}


DummyAMC13Manager::~DummyAMC13Manager() {
}




void DummyAMC13Manager::retrieveMetricValues() {
  DummyAMC13Driver::TTCStatus s = driver_->readTTCStatus();

  setMetricValue<uint16_t>(daqMetricFedId_, driver_->readFedId());
}


} // namespace dummy
} // namespace swatch
