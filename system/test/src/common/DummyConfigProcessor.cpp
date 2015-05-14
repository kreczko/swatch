/**
 * @file    DummyConfigProcessor.hpp
 * @author  Alessandro Thea
 * @brief   Brief description
 * @date    
 */

#include "uhal/ConnectionManager.hpp"

// Swatch Headers
#include "swatch/logger/Log.hpp"
#include "swatch/core/Factory.hpp"
#include "swatch/processor/ProcessorStub.hpp"
#include "swatch/system/test/DummyConfigProcessor.hpp"
#include "swatch/processor/ProcessorConfigSequence.hpp"

// XDAQ Headers
#include "xdata/String.h"

// Boost Headers
#include <boost/assign.hpp>
#include <boost/foreach.hpp>

#include "swatch/core/test/DummyCommand.hpp"
#include "swatch/core/test/DummyOperation.hpp"

// C++ Headers
#include <iomanip>


SWATCH_REGISTER_CLASS(swatch::system::test::DummyConfigProcessor);


namespace swatch {
namespace system {
namespace test {

DummyConfigProcessor::DummyConfigProcessor(const std::string& id, const swatch::core::XParameterSet& params) :
Processor(id, params)
{
  swatch::processor::ProcessorStub& stub = params.get<swatch::processor::ProcessorBag>("stub").bag;
  crate_ = stub.crate;
  slot_ = stub.slot;

  core::Command* lDummy1 = Register<core::test::DummyCommand>("dummy_1");
  core::Command* lDummy2 = Register<core::test::DummyCommand>("dummy_2");
  core::Command* lDummy3 = Register<core::test::DummyCommand>("dummy_3");
  Register<core::test::DummyCommand>("dummy_4");
  Register<core::test::DummyCommand>("dummy_5");
  Register<core::test::DummyCommand>("dummy_6");

  //Define the sequence through pointers to commands
  core::ConfigSequence* lSequence1 = Register<processor::ProcessorConfigSequence>( "sequence_1" );
  lSequence1->run( lDummy1 ).then( lDummy2 ).then( lDummy3 );

  //Define the sequence through command ids
  core::ConfigSequence* lSequence2 = Register<processor::ProcessorConfigSequence>( "sequence_2" );
  lSequence2->run( "dummy_4" )( "dummy_5" )( "dummy_6" );



}

DummyConfigProcessor::~DummyConfigProcessor() {
}

const std::string& DummyConfigProcessor::getCrateId() const {
  return crate_;
}

uint32_t DummyConfigProcessor::getSlot() const {
  return slot_;
}

uint64_t DummyConfigProcessor::firmwareVersion() const {
  return 0;
}

std::string DummyConfigProcessor::firmwareInfo() const {
  return "none";
}

}
}
}

