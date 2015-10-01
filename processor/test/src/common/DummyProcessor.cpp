/**
 * @file    DummyProcessor.hpp
 * @author  Alessandro Thea
 */

#include "swatch/processor/test/DummyProcessor.hpp"

// SWATCH headers
#include "swatch/logger/Log.hpp"
#include "swatch/core/CommandSequence.hpp"
#include "swatch/core/Factory.hpp"
#include "swatch/core/StateMachine.hpp"
#include "swatch/processor/PortCollection.hpp"
#include "swatch/processor/ProcessorStub.hpp"
#include "swatch/processor/test/DummyAlgo.hpp"
#include "swatch/processor/test/DummyDriver.hpp"
#include "swatch/processor/test/DummyProcessorCommands.hpp"
#include "swatch/processor/test/DummyReadout.hpp"
#include "swatch/processor/test/DummyRxPort.hpp"
#include "swatch/processor/test/DummyTxPort.hpp"
#include "swatch/processor/test/DummyTTC.hpp"

// XDAQ Headers
#include "xdata/String.h"
#include "xdata/Vector.h"

// Boost Headers
#include <boost/assign.hpp>
#include <boost/foreach.hpp>

// C++ Headers
#include <iomanip>


SWATCH_REGISTER_CLASS(swatch::processor::test::DummyProcessor);


namespace swatch {
namespace processor {
namespace test {


DummyProcessor::DummyProcessor(const swatch::core::AbstractStub& aStub) :
  Processor(aStub),
  driver_(new DummyDriver()),
  ranTests_()
{
  // 1) Interfaces
  registerInterface( new DummyTTC(*driver_) );
  registerInterface( new DummyReadoutInterface(*driver_) );
  registerInterface( new DummyAlgo(*driver_) );
  registerInterface( new processor::PortCollection() );
  
  const ProcessorStub& stub = getStub();
  
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

  // 4) Operations
  mRunControl.coldReset.add(reboot);
  mRunControl.setup.add(cfgSeq);
  mRunControl.preconfigure.add(cfgAlgoSeq);
  mRunControl.connect.add(cfgRxSeq);
  mRunControl.fsm.addTransition("dummyNoOp", RunControlFSM::kStateCfg, RunControlFSM::kStateInitial);
}


DummyProcessor::~DummyProcessor() {
}


const std::vector<std::string> DummyProcessor::ranTests() const {
  return ranTests_;
}


void DummyProcessor::test1() {
  // some code to test the device
  ranTests_.push_back("test1");
}


void DummyProcessor::test2() {
  // some code to test the device
  ranTests_.push_back("test2");
}


std::string DummyProcessor::firmwareInfo() const {
  return "none";
}

// TO DELETE
//swatch::core::XParameterSet DummyProcessor::generateParams() {
ProcessorStub DummyProcessor::generateParams( const std::string& aId ) {
  const std::string addrtab = "file://processor/test/etc/swatch/processor/test/dummy.xml";

  swatch::processor::ProcessorStub stubTemplate(aId);
  // TO DELETE
  //  stubTemplate.name = "";
  stubTemplate.creator = "swatch::processor::test::DummyProcessor";
  stubTemplate.addressTable = addrtab;
  stubTemplate.uri = "";
  stubTemplate.crate = "s2g20-10";
  stubTemplate.slot = 0;

  stubTemplate.rxPorts.push_back( getPortBag("rxA", 0) );
  stubTemplate.rxPorts.push_back( getPortBag("rxB", 1) );
  stubTemplate.rxPorts.push_back( getPortBag("rxC", 2) );
  stubTemplate.rxPorts.push_back( getPortBag("rxD", 3) );
  stubTemplate.rxPorts.push_back( getPortBag("rxE", 4) );

  stubTemplate.txPorts.push_back( getPortBag("txA", 0) );
  stubTemplate.txPorts.push_back( getPortBag("txB", 5) );
  stubTemplate.txPorts.push_back( getPortBag("txC", 10) );

/* TO DELETE  
  // x is ready for testing
  swatch::processor::ProcessorBag p0bag;
  p0bag.bag = stubTemplate;
  
  swatch::core::XParameterSet params;
  params.add("name", xdata::String("Processor 0"));
  params.add("class", p0bag.bag.creator);
  params.add("stub", p0bag);
  return params;
 */
  
  return stubTemplate;
}


void DummyProcessor::retrieveMetricValues()
{
  setMetricValue<uint64_t>(metricFirmwareVersion_, driver_->getFirmwareVersion());
}


ProcessorPortStub DummyProcessor::getPortBag(const std::string& name, size_t number)
{
//  xdata::Bag<ProcessorPortStub> b;
  ProcessorPortStub b(name);
  b.number = number;
  return b;
}

}
}
}

