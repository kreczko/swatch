
#include "swatch/processor/test/DummyProcessor.hpp"

// SWATCH headers
#include "swatch/logger/Log.hpp"
#include "swatch/core/CommandSequence.hpp"
#include "swatch/core/Factory.hpp"
#include "swatch/core/StateMachine.hpp"
#include "swatch/processor/PortCollection.hpp"
#include "swatch/processor/ProcessorStub.hpp"

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
  Processor(aStub)
{
  // 1) Interfaces
  registerInterface( new DummyTTC() );
  registerInterface( new DummyReadoutInterface() );
  registerInterface( new DummyAlgo() );
  registerInterface( new processor::PortCollection() );
  
  const ProcessorStub& stub = getStub();
  
  for(auto it = stub.rxPorts.begin(); it != stub.rxPorts.end(); it++)
    getPorts().addInput(new DummyRxPort(it->id, it->number));
  for(auto it = stub.txPorts.begin(); it != stub.txPorts.end(); it++)
    getPorts().addOutput(new DummyTxPort(it->id, it->number));
}


DummyProcessor::~DummyProcessor() {
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

  return stubTemplate;
}


void DummyProcessor::retrieveMetricValues()
{
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

