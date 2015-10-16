
#include "swatch/dummy/DummySystem.hpp"


#include "swatch/core/Factory.hpp"
#include "swatch/core/SystemStateMachine.hpp"
#include "swatch/processor/Processor.hpp"
#include "swatch/dtm/DaqTTCManager.hpp"


SWATCH_REGISTER_CLASS(swatch::dummy::DummySystem)


namespace swatch {
namespace dummy {

DummySystem::DummySystem(const swatch::core::AbstractStub& aStub) : 
  swatch::system::System(aStub)
{
  typedef processor::RunControlFSM tProcFSM;
  typedef dtm::RunControlFSM tDaqTTCFSM;
  
  system::RunControlFSM& fsm = getRunControlFSM();
  fsm.coldReset.add(getDaqTTC(), tDaqTTCFSM::kStateInitial, tDaqTTCFSM::kTrColdReset)
               .add(getProcessors(), tProcFSM::kStateInitial, tProcFSM::kTrColdReset);

  fsm.setup.add(getDaqTTC(), tDaqTTCFSM::kStateInitial, tDaqTTCFSM::kTrClockSetup)
           .add(getProcessors(), tProcFSM::kStateInitial, tProcFSM::kTrSetup)
           .add(getDaqTTC(), tDaqTTCFSM::kStateClockOK, tDaqTTCFSM::kTrCfgDaq);

  fsm.preconfigure.add(getProcessors(), tProcFSM::kStateSync, tProcFSM::kTrPreCfg);

  fsm.connect.add(getProcessors(), tProcFSM::kStatePreCfg, tProcFSM::kTrConnect);

  fsm.start.add(getDaqTTC(), tDaqTTCFSM::kStateCfg, tDaqTTCFSM::kTrStart);

  fsm.pause.add(getDaqTTC(), tDaqTTCFSM::kStateRunning, tDaqTTCFSM::kTrPause);

  fsm.resume.add(getDaqTTC(), tDaqTTCFSM::kStatePaused, tDaqTTCFSM::kTrResume);

  fsm.stopFromPaused.add(getDaqTTC(), tDaqTTCFSM::kStatePaused, tDaqTTCFSM::kTrStop);

  fsm.stopFromRunning.add(getDaqTTC(), tDaqTTCFSM::kStateRunning, tDaqTTCFSM::kTrStop);
}


DummySystem::~DummySystem()
{
}


} // namespace dummy
} // namespace swatch
