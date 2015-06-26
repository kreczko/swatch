/**
 * @file    DummyConfigProcessor.hpp
 * @author  Alessandro Thea
 * @brief   Brief description
 * @date    
 */

#include "uhal/ConnectionManager.hpp"

// Swatch Headers
#include "swatch/logger/Log.hpp"
#include "swatch/core/Command.hpp"
#include "swatch/core/Factory.hpp"
#include "swatch/core/Placeholder.hpp"
#include "swatch/system/test/DummyConfigProcessor.hpp"
#include "swatch/processor/ProcessorCommandSequence.hpp"

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
  DummyConfigCommand( const std::string& aId ) :
    core::Command( aId , xdata::Integer(-33)) {
    registerParameter("parameter", xdata::String("Default Value"));
  }

  virtual ~DummyConfigCommand(){}

  virtual State code(swatch::core::XParameterSet& params )///Should take const reference but xdata::serializable is const-correctness broken
  {

    std::string lParameter = params.get<xdata::String>("parameter");
    LOG(logger::kInfo) << id() << ".exec() running case with 'parameter'='" << lParameter<<"'";
    setStatusMsg("Dummy command successfully completed");
    return kDone;
  }


};



// --------------------------------------------------------------------------------------------------------------------------------



DummyConfigProcessor::DummyConfigProcessor(const std::string& id, const swatch::core::XParameterSet& params) :
Processor(id, params)
{

  core::Command* lDummy1 = Register<DummyConfigCommand>("dummy_1");
  core::Command* lDummy2 = Register<DummyConfigCommand>("dummy_2");
  core::Command* lDummy3 = Register<DummyConfigCommand>("dummy_3");
  Register<DummyConfigCommand>("dummy_4");
  Register<DummyConfigCommand>("dummy_5");
  Register<DummyConfigCommand>("dummy_6");

  //Define the sequence through pointers to commands - run, then and operator() are all aliases
  core::CommandSequence* lSequence1 = Register<processor::ProcessorCommandSequence>( "sequence_1" );
  lSequence1->run( lDummy1 ).then( lDummy2 )( lDummy3 );

  //Define the sequence through command ids - run, then and operator() are all aliases
  core::CommandSequence* lSequence2 = Register<processor::ProcessorCommandSequence>( "sequence_2" );
  lSequence2->run( "dummy_4" ).then( "dummy_5" )( "dummy_6" );


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

