
#include "swatch/dummy/DummyProcessor.hpp"

// SWATCH headers
#include "swatch/logger/Log.hpp"
#include "swatch/core/CommandSequence.hpp"
#include "swatch/core/Factory.hpp"
#include "swatch/core/StateMachine.hpp"
#include "swatch/processor/PortCollection.hpp"
#include "swatch/processor/ProcessorStub.hpp"
#include "swatch/dummy/DummyAlgo.hpp"
#include "swatch/dummy/DummyProcDriver.hpp"
#include "swatch/dummy/DummyProcessorCommands.hpp"
#include "swatch/dummy/DummyReadout.hpp"
#include "swatch/dummy/DummyRxPort.hpp"
#include "swatch/dummy/DummyTxPort.hpp"
#include "swatch/dummy/DummyTTC.hpp"

// XDAQ Headers
#include "xdata/String.h"
#include "xdata/Vector.h"

// Boost Headers
#include <boost/assign.hpp>
#include <boost/foreach.hpp>

// C++ Headers
#include <iomanip>


SWATCH_REGISTER_CLASS(swatch::dummy::DummyProcessor);


namespace swatch {
namespace dummy {


DummyProcessor::DummyProcessor(const swatch::core::AbstractStub& aStub) :
  Processor(aStub),
  driver_(new DummyProcDriver())
{
  // 1) Interfaces
  registerInterface( new DummyTTC(*driver_) );
  registerInterface( new DummyReadoutInterface(*driver_) );
  registerInterface( new DummyAlgo(*driver_) );
  registerInterface( new processor::PortCollection() );
  
  const processor::ProcessorStub& stub = getStub();
  
  for(auto it = stub.rxPorts.begin(); it != stub.rxPorts.end(); it++)
    getPorts().addInput(new DummyRxPort(it->id, it->number, *driver_));
  for(auto it = stub.txPorts.begin(); it != stub.txPorts.end(); it++)
    getPorts().addOutput(new DummyTxPort(it->id, it->number, *driver_));

  // 2) Commands
  core::Command& reboot = registerFunctionoid<DummyResetCommand>("reboot");
  core::Command& reset = registerFunctionoid<DummyResetCommand>("reset");
  core::Command& cfgTx = registerFunctionoid<DummyConfigureTxCommand>("configureTx");
  core::Command& cfgRx = registerFunctionoid<DummyConfigureRxCommand>("configureRx");
  core::Command& cfgDaq = registerFunctionoid<DummyConfigureDaqCommand>("configureDaq");
  core::Command& cfgAlgo = registerFunctionoid<DummyConfigureAlgoCommand>("configureAlgo");

  // 3) Command sequences
  core::CommandSequence& cfgSeq = registerCommandSequence("configSeq1",reboot).then(reset).then(cfgDaq).then(cfgTx);
  core::CommandSequence& cfgRxSeq = registerCommandSequence("configRxSeq", cfgRx);
  core::CommandSequence& cfgAlgoSeq = registerCommandSequence("configAlgoSeq", cfgAlgo);

  // 4) State machines
  processor::RunControlFSM& lFSM = getRunControlFSM();
  lFSM.coldReset.add(reboot);
  lFSM.setup.add(cfgSeq);
  lFSM.preconfigure.add(cfgAlgoSeq);
  lFSM.connect.add(cfgRxSeq);
  lFSM.fsm.addTransition("dummyNoOp", processor::RunControlFSM::kStateCfg, processor::RunControlFSM::kStateInitial);
}


DummyProcessor::~DummyProcessor() {
}


std::string DummyProcessor::firmwareInfo() const {
  return "none";
}


void DummyProcessor::retrieveMetricValues()
{
  setMetricValue<uint64_t>(metricFirmwareVersion_, driver_->getFirmwareVersion());
}


}
}

