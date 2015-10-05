
#include "swatch/dummy/DummySystem.hpp"


#include "swatch/core/Factory.hpp"
#include "swatch/core/SystemStateMachine.hpp"
#include "swatch/processor/Processor.hpp"
#include "swatch/system/DaqTTCManager.hpp"


SWATCH_REGISTER_CLASS(swatch::dummy::DummySystem)


namespace swatch {
namespace dummy {

DummySystem::DummySystem(const swatch::core::AbstractStub& aStub) : 
  swatch::system::System(aStub)
{
  typedef processor::Processor::RunControlFSM tProcFSM;
  typedef system::RunControlFSM tDaqTTCFSM;
  
  system::SysRunControlFSM& fsm = mRunControl;
  fsm.coldReset.add(getDaqTTC().begin(), getDaqTTC().end(), tDaqTTCFSM::kStateInitial, tDaqTTCFSM::kTrColdReset)
               .add(getProcessors().begin(), getProcessors().end(), tProcFSM::kStateInitial, tProcFSM::kTrColdReset);

  fsm.setup.add(getDaqTTC().begin(), getDaqTTC().end(), tDaqTTCFSM::kStateInitial, tDaqTTCFSM::kTrClockSetup)
           .add(getProcessors().begin(), getProcessors().end(), tProcFSM::kStateInitial, tProcFSM::kTrSetup)
           .add(getDaqTTC().begin(), getDaqTTC().end(), tDaqTTCFSM::kStateClockOK, tDaqTTCFSM::kTrCfgDaq);

  fsm.preconfigure.add(getProcessors().begin(), getProcessors().end(), tProcFSM::kStateSync, tProcFSM::kTrPreCfg);

  fsm.connect.add(getProcessors().begin(), getProcessors().end(), tProcFSM::kStatePreCfg, tProcFSM::kTrConnect);

  fsm.start.add(getDaqTTC().begin(), getDaqTTC().end(), tDaqTTCFSM::kStateCfg, tDaqTTCFSM::kTrStart);

  fsm.pause.add(getDaqTTC().begin(), getDaqTTC().end(), tDaqTTCFSM::kStateRunning, tDaqTTCFSM::kTrPause);

  fsm.resume.add(getDaqTTC().begin(), getDaqTTC().end(), tDaqTTCFSM::kStatePaused, tDaqTTCFSM::kTrResume);

  fsm.stopFromPaused.add(getDaqTTC().begin(), getDaqTTC().end(), tDaqTTCFSM::kStatePaused, tDaqTTCFSM::kTrStop);

  fsm.stopFromRunning.add(getDaqTTC().begin(), getDaqTTC().end(), tDaqTTCFSM::kStateRunning, tDaqTTCFSM::kTrStop);
}


DummySystem::~DummySystem()
{
}


} // namespace dummy
} // namespace swatch
