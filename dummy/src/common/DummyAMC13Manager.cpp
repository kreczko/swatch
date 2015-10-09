
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
  
  
  
  mRunControl.coldReset.add(reboot);
  mRunControl.clockSetup.add(reset);
  mRunControl.cfgDaq.add(cfgDaq);
  mRunControl.start.add(startDaq);
  //mRunControl.pause;
  //mRunControl.resume;
  mRunControl.stopFromPaused.add(stopDaq);
  mRunControl.stopFromRunning.add(stopDaq);
}


DummyAMC13Manager::~DummyAMC13Manager() {
}




void DummyAMC13Manager::retrieveMetricValues() {
  DummyAMC13Driver::TTCStatus s = driver_->readTTCStatus();
  
  setMetricValue<double>(ttcMetricClockFreq_, s.clockFreq);
  setMetricValue<uint32_t>(ttcMetricBC0Counter_, s.bc0Counter);
  setMetricValue<uint32_t>(ttcMetricBC0Errors_, s.errCountBC0);
  setMetricValue<uint32_t>(ttcMetricSingleBitErrors_, s.errCountSingleBit);
  setMetricValue<uint32_t>(ttcMetricDoubleBitErrors_, s.errCountDoubleBit);
  
  setMetricValue<uint16_t>(daqMetricFedId_, driver_->readFedId());
}


} // namespace dummy
} // namespace swatch
