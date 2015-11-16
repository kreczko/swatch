/**
 * @file    DummyConfigProcessor.cpp
 * @author  Alessandro Thea
 * @brief   Brief description
 * @date    
 */

#include "uhal/ConnectionManager.hpp"

// Swatch Headers
#include "swatch/logger/Log.hpp"
#include "swatch/core/Command.hpp"
#include "swatch/core/CommandSequence.hpp"
#include "swatch/core/Factory.hpp"
#include "swatch/system/test/DummyConfigProcessor.hpp"

// XDAQ Headers
#include "xdata/String.h"
#include "xdata/Integer.h"

// Boost Headers
#include <boost/assign.hpp>
#include <boost/foreach.hpp>

// C++ Headers
#include <iomanip>


SWATCH_REGISTER_CLASS(swatch::system::test::DummyConfigProcessor);


namespace swatch {
namespace system {
namespace test {
class DummyConfigProcessor;

class DummyConfigCommand: public swatch::core::Command {
public:
  DummyConfigCommand( const std::string& aId, swatch::core::ActionableObject& aProcessor ) :
    core::Command( aId , aProcessor, xdata::Integer(-33)) {
    registerParameter("parameter", xdata::String("Default Value"));
  }

  virtual ~DummyConfigCommand(){}

  virtual State code(const swatch::core::XParameterSet& params )
  {

    std::string lParameter = params.get<xdata::String>("parameter").value_;
    LOG(logger::kInfo) << getId() << ".exec() running case with 'parameter'='" << lParameter<<"'";
    setStatusMsg("Dummy command successfully completed");
    return State::kDone;
  }


};



// --------------------------------------------------------------------------------------------------------------------------------



DummyConfigProcessor::DummyConfigProcessor(const swatch::core::AbstractStub& aStub) :
Processor(aStub)
{
  core::Command& lDummy1 = registerCommand<DummyConfigCommand>("dummy_1");
  core::Command& lDummy2 = registerCommand<DummyConfigCommand>("dummy_2");
  core::Command& lDummy3 = registerCommand<DummyConfigCommand>("dummy_3");
  registerCommand<DummyConfigCommand>("dummy_4");
  registerCommand<DummyConfigCommand>("dummy_5");
  registerCommand<DummyConfigCommand>("dummy_6");

  //Define the sequence through pointers to commands - run, then and operator() are all aliases
  core::CommandSequence& lSequence1 = registerSequence( "sequence_1", lDummy1 );
  lSequence1.run( lDummy2 ).then( lDummy3 );

  //Define the sequence through command ids - run, then and operator() are all aliases
  core::CommandSequence& lSequence2 = registerSequence( "sequence_2", "dummy_4" );
  lSequence2.then( "dummy_5" ).then( "dummy_6" );
}


DummyConfigProcessor::~DummyConfigProcessor() {
}


uint64_t DummyConfigProcessor::firmwareVersion() const {
  return 0;
}

std::string DummyConfigProcessor::firmwareInfo() const {
  return "none";
}

void DummyConfigProcessor::setSomething(std::string something){
  something_ = something;
}

void DummyConfigProcessor::setNumber(uint32_t number){
  number_ = number;
}


std::string DummyConfigProcessor::something() const {
  return something_;
}

uint32_t DummyConfigProcessor::number() const {
  return number_;
}

void DummyConfigProcessor::fail() {
  throw core::exception("DummyConfigProcessor failed");
}

}
}
}

