
#include "swatch/dummy/DummyAMC13Manager.hpp"


// boost headers
#include "boost/foreach.hpp"

// SWATCH headers
#include "swatch/logger/Log.hpp"
#include "swatch/core/Factory.hpp"
#include "swatch/core/StateMachine.hpp"
#include "swatch/dtm/DaqTTCStub.hpp"
#include "swatch/dummy/DummyAMC13Driver.hpp"
#include "swatch/dummy/DummyAMC13Interfaces.hpp"
#include "swatch/dummy/DummyAMC13ManagerCommands.hpp"
#include "swatch/dtm/AMCPortCollection.hpp"


SWATCH_REGISTER_CLASS(swatch::dummy::DummyAMC13Manager);


namespace swatch {
namespace dummy {


DummyAMC13Manager::DummyAMC13Manager( const swatch::core::AbstractStub& aStub ) : 
  dtm::DaqTTCManager(aStub),
  mDriver(new DummyAMC13Driver())
{
  // 0) Monitoring interfaces
  registerInterface( new AMC13TTC(*mDriver) );
  registerInterface( new AMC13SLinkExpress(0, *mDriver) );
  registerInterface( new dtm::AMCPortCollection() );
  for(std::vector<uint32_t>::const_iterator lIt=getStub().amcSlots.begin(); lIt!=getStub().amcSlots.end(); lIt++)
    getAMCPorts().addPort(new AMC13BackplaneDaqPort(*lIt, *mDriver));
  registerInterface( new AMC13EventBuilder(*mDriver));
  
  // 1) Commands
  core::Command& reboot = registerCommand<DummyAMC13RebootCommand>("reboot");
  core::Command& reset = registerCommand<DummyAMC13ResetCommand>("reset");
  core::Command& cfgDaq = registerCommand<DummyAMC13ConfigureDaqCommand>("configureDaq");
  core::Command& startDaq = registerCommand<DummyAMC13StartDaqCommand>("startDaq");
  core::Command& stopDaq = registerCommand<DummyAMC13StopDaqCommand>("stopDaq");
  
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
  DummyAMC13Driver::TTCStatus s = mDriver->readTTCStatus();

  setMetricValue<uint16_t>(daqMetricFedId_, mDriver->readFedId());
}


} // namespace dummy
} // namespace swatch
