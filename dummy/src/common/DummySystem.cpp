
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
  typedef processor::RunControlFSM ProcFSM_t;
  typedef dtm::RunControlFSM DaqTTCFSM_t;
  
  system::RunControlFSM& fsm = getRunControlFSM();
  fsm.coldReset.add(getDaqTTCs(), DaqTTCFSM_t::kStateInitial, DaqTTCFSM_t::kTrColdReset)
               .add(getProcessors(), ProcFSM_t::kStateInitial, ProcFSM_t::kTrColdReset);

  fsm.setup.add(getDaqTTCs(), DaqTTCFSM_t::kStateInitial, DaqTTCFSM_t::kTrClockSetup)
           .add(getProcessors(), ProcFSM_t::kStateInitial, ProcFSM_t::kTrSetup)
           .add(getDaqTTCs(), DaqTTCFSM_t::kStateClockOK, DaqTTCFSM_t::kTrCfgDaq);

  fsm.configure.add(getProcessors(), ProcFSM_t::kStateSync, ProcFSM_t::kTrConfigure);

  fsm.align.add(getProcessors(), ProcFSM_t::kStateConfigured, ProcFSM_t::kTrAlign);

  fsm.start.add(getDaqTTCs(), DaqTTCFSM_t::kStateConfigured, DaqTTCFSM_t::kTrStart);

  fsm.pause.add(getDaqTTCs(), DaqTTCFSM_t::kStateRunning, DaqTTCFSM_t::kTrPause);

  fsm.resume.add(getDaqTTCs(), DaqTTCFSM_t::kStatePaused, DaqTTCFSM_t::kTrResume);

  fsm.stopFromPaused.add(getDaqTTCs(), DaqTTCFSM_t::kStatePaused, DaqTTCFSM_t::kTrStop)
                    .add(getProcessors(), ProcFSM_t::kStateAligned, ProcFSM_t::kTrStop);

  fsm.stopFromRunning.add(getDaqTTCs(), DaqTTCFSM_t::kStateRunning, DaqTTCFSM_t::kTrStop)
                     .add(getProcessors(), ProcFSM_t::kStateAligned, ProcFSM_t::kTrStop);
}


DummySystem::~DummySystem()
{
}


} // namespace dummy
} // namespace swatch
