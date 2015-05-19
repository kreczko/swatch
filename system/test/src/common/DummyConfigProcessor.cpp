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
#include "swatch/processor/ProcessorStub.hpp"
#include "swatch/system/test/DummyConfigProcessor.hpp"
#include "swatch/processor/ProcessorConfigSequence.hpp"

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
    registerParameter("aa", swatch::core::Placeholder< xdata::Integer >() );
    registerParameter("todo", xdata::String(""));
  }

  template<typename T>  DummyConfigCommand( const std::string& aId , const T& aDefault) :
    swatch::core::Command( aId , aDefault) {
    registerParameter("aa", swatch::core::Placeholder< xdata::Integer >() );
    registerParameter("todo", xdata::String(""));
  }

  virtual ~DummyConfigCommand(){}

  virtual void code(swatch::core::XParameterSet& params )///Should take const reference but xdata::serializable is const-correctness broken
  {
    DummyConfigProcessor* res = getParent<DummyConfigProcessor>();
  
    BOOST_FOREACH(const std::string& n, params.keys()) {
      LOG(logger::kInfo) << n << " : " << params.get(n).toString();
    }
  
    std::string todo = params.get<xdata::String>("todo");
    if (todo == "print") {
      LOG(logger::kInfo) << id() << ".exec() running case 'print' with 'aa'=" << params.get<xdata::Integer>("aa");
      res->setSomething("|12345|");
      res->setNumber(54);
      setProgress(100.0);
      setResult(xdata::Integer(99));
      setProgress(99., "doing stuff");
      setDone("Dummy command successfully completed");
  
    } else if (todo == "error") {
      LOG(logger::kInfo) << id() << ".exec() running case 'error'";
      //      setProgress(50.49);
      setProgress(50.49, "Dummy command did something");
      setError("But ended up in error");
  
    } else {
      LOG(logger::kInfo) << id() << ".exec() running case OTHER";
      //      setProgress(0.0);
      setProgress(0.0, "Not even started");
      setWarning("Nothing was done");
  
    }
  }


};



// --------------------------------------------------------------------------------------------------------------------------------



DummyConfigProcessor::DummyConfigProcessor(const std::string& id, const swatch::core::XParameterSet& params) :
Processor(id, params)
{
  swatch::processor::ProcessorStub& stub = params.get<swatch::processor::ProcessorBag>("stub").bag;
  crate_ = stub.crate;
  slot_ = stub.slot;

  core::Command* lDummy1 = Register<DummyConfigCommand>("dummy_1");
  core::Command* lDummy2 = Register<DummyConfigCommand>("dummy_2");
  core::Command* lDummy3 = Register<DummyConfigCommand>("dummy_3");
  Register<DummyConfigCommand>("dummy_4");
  Register<DummyConfigCommand>("dummy_5");
  Register<DummyConfigCommand>("dummy_6");

  //Define the sequence through pointers to commands - run, then and operator() are all aliases
  core::ConfigSequence* lSequence1 = Register<processor::ProcessorConfigSequence>( "sequence_1" );
  lSequence1->run( lDummy1 ).then( lDummy2 )( lDummy3 );

  //Define the sequence through command ids - run, then and operator() are all aliases
  core::ConfigSequence* lSequence2 = Register<processor::ProcessorConfigSequence>( "sequence_2" );
  lSequence2->run( "dummy_4" ).then( "dummy_5" )( "dummy_6" );


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

