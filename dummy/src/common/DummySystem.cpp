
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
  fsm.coldReset.add(getDaqTTCs(), tDaqTTCFSM::kStateInitial, tDaqTTCFSM::kTrColdReset)
               .add(getProcessors(), tProcFSM::kStateInitial, tProcFSM::kTrColdReset);

  fsm.setup.add(getDaqTTCs(), tDaqTTCFSM::kStateInitial, tDaqTTCFSM::kTrClockSetup)
           .add(getProcessors(), tProcFSM::kStateInitial, tProcFSM::kTrSetup)
           .add(getDaqTTCs(), tDaqTTCFSM::kStateClockOK, tDaqTTCFSM::kTrCfgDaq);

  fsm.configure.add(getProcessors(), tProcFSM::kStateSync, tProcFSM::kTrConfigure);

  fsm.align.add(getProcessors(), tProcFSM::kStateConfigured, tProcFSM::kTrAlign);

  fsm.start.add(getDaqTTCs(), tDaqTTCFSM::kStateConfigured, tDaqTTCFSM::kTrStart);

  fsm.pause.add(getDaqTTCs(), tDaqTTCFSM::kStateRunning, tDaqTTCFSM::kTrPause);

  fsm.resume.add(getDaqTTCs(), tDaqTTCFSM::kStatePaused, tDaqTTCFSM::kTrResume);

  fsm.stopFromPaused.add(getDaqTTCs(), tDaqTTCFSM::kStatePaused, tDaqTTCFSM::kTrStop);

  fsm.stopFromRunning.add(getDaqTTCs(), tDaqTTCFSM::kStateRunning, tDaqTTCFSM::kTrStop);
}


DummySystem::~DummySystem()
{
}


} // namespace dummy
} // namespace swatch
