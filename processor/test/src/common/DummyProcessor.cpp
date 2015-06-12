/**
 * @file    DummyProcessor.hpp
 * @author  Alessandro Thea
 * @brief   Brief description
 * @date    
 */

#include "uhal/ConnectionManager.hpp"

// Swatch Headers
#include "swatch/logger/Log.hpp"
#include "swatch/core/Factory.hpp"
//#include "swatch/processor/ProcessorFactory.hpp"
#include "swatch/processor/LinkInterface.hpp"
#include "swatch/processor/ProcessorStub.hpp"
#include "swatch/processor/test/DummyProcessor.hpp"
#include "swatch/processor/test/DummyRxPort.hpp"
#include "swatch/processor/test/DummyTxPort.hpp"

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

DummyProcessor::DummyProcessor(const std::string& id,
    const swatch::core::XParameterSet& params) :
        Processor(id, params),
        ranTests_() {

  Add( new processor::LinkInterface() );

  ProcessorStub& stub = params.get<ProcessorBag>("stub").bag;
  
  std::vector<ProcessorPortBag>::iterator it;
  for(it = stub.rxPorts.begin(); it != stub.rxPorts.end(); it++)
    linkInterface()->addInput(new DummyRxPort(it->bag.name));
  for(it = stub.txPorts.begin(); it != stub.txPorts.end(); it++)
    linkInterface()->addOutput(new DummyTxPort(it->bag.name));
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


swatch::core::XParameterSet DummyProcessor::generateParams() {
  const std::string addrtab = "file://processor/test/etc/swatch/processor/test/dummy.xml";

  swatch::processor::ProcessorStub stubTemplate;
  stubTemplate.name = "";
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
  
  // x is ready for testing
  swatch::processor::ProcessorBag p0bag;
  p0bag.bag = stubTemplate;
  
  swatch::core::XParameterSet params;
  params.add("name", xdata::String("Processor 0"));
  params.add("class", p0bag.bag.creator);
  params.add("stub", p0bag);
  return params;
}


void DummyProcessor::implementUpdateMetrics()
{
  setMetricValue<uint64_t>(metricFirmwareVersion_, 0);
}


xdata::Bag<ProcessorPortStub> DummyProcessor::getPortBag(const std::string& name, size_t number)
{
  xdata::Bag<ProcessorPortStub> b;
  b.bag.name = name;
  b.bag.number = number;
  return b;
}

}
}
}

