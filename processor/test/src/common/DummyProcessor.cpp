/**
 * @file    DummyProcessor.hpp
 * @author  Alessandro Thea
 * @brief   Brief description
 * @date    
 */

#include "uhal/ConnectionManager.hpp"

// Swatch Headers
#include "swatch/logger/Log.hpp"
#include "swatch/processor/ProcessorFactory.hpp"
#include "swatch/processor/ProcessorStub.hpp"
#include "swatch/processor/test/DummyProcessor.hpp"
#include "swatch/processor/test/DummyRxPort.hpp"
#include "swatch/processor/test/DummyTxPort.hpp"

// XDAQ Headers
#include "xdata/String.h"

// Boost Headers
#include <boost/assign.hpp>
#include <boost/foreach.hpp>

// C++ Headers
#include <iomanip>

SWATCH_PROCESSOR_REGISTER_CLASS(swatch::processor::test::DummyProcessor);


namespace swatch {
namespace processor {
namespace test {

// SWATCH_PROCESSOR_REGISTER_CLASS(DummyProcessor);

DummyProcessor::DummyProcessor(const std::string& id,
    const swatch::core::XParameterSet& params) :
        Processor(id, params),
        ranTests_() {
  std::stringstream ss;
  for (int chan(0); chan < 4; ++chan) {
    ss.str(std::string());
    ss << "rx" << std::setw(2) << std::setfill('0') << chan;
    this->addInput(new DummyRxPort(ss.str()));
  }

  for (int chan(0); chan < 2; ++chan) {
    ss.str(std::string());
    ss << "tx" << std::setw(2) << std::setfill('0') << chan;
    this->addOutput(new DummyTxPort(ss.str()));
  }

  swatch::processor::ProcessorBag& stub = params.get<swatch::processor::ProcessorBag>("stub");
  crate_ = stub.bag.crate;
  slot_ = stub.bag.slot;
}

DummyProcessor::~DummyProcessor() {
}

const std::string& DummyProcessor::getCrateId() const {
  return crate_;
}

uint32_t DummyProcessor::getSlot() const {
  return slot_;
}

swatch::core::XParameterSet DummyProcessor::generateParams() {
  const std::string addrtab = "file://processor/test/etc/dummy.xml";

  swatch::processor::ProcessorStub stubTemplate;
  stubTemplate.name = "";
  stubTemplate.creator = "swatch::processor::test::DummyProcessor";
  stubTemplate.addressTable = addrtab;
  stubTemplate.uri = "";
  stubTemplate.crate = "s2g20-10";
  stubTemplate.slot = 0;

  // x is ready for testing
  swatch::processor::ProcessorBag p0bag;
  p0bag.bag = stubTemplate;

  swatch::core::XParameterSet params;
  params.add("name", xdata::String("Processor 0"));
  params.add("class", p0bag.bag.creator);
  params.add("stub", p0bag);
  return params;
}

void DummyProcessor::test1() {
  // some code to test the device
  ranTests_.push_back("test1");
}

void DummyProcessor::test2() {
  // some code to test the device
  ranTests_.push_back("test2");
}

const std::vector<std::string> DummyProcessor::ranTests() const {
  return ranTests_;
}

uint64_t DummyProcessor::firmwareVersion() const {
  return 0;
}

std::string DummyProcessor::firmwareInfo() const {
  return "none";
}

}
}
}

