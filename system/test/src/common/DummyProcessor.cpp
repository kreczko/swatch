/* 
 * File:   DummyProcessor.cpp
 * Author: ale
 * 
 * Created on July 27, 2014, 9:19 PM
 */

#include "swatch/system/test/DummyProcessor.hpp"

// Swatch Headers
#include "swatch/logger/Log.hpp"
#include "swatch/core/xoperators.hpp"
#include "swatch/core/Utilities.hpp"
#include "swatch/core/Port.hpp"
#include "swatch/processor/ProcessorStub.hpp"
#include "swatch/processor/ProcessorFactory.hpp"

// Boost Headers
#include <boost/foreach.hpp>

// C++ Headers
#include <iomanip>

// TOREMOVE
namespace swlog = swatch::logger;
namespace swpro = swatch::processor;

namespace swatch {
namespace system {
namespace test {


//---
DummyRxPort::DummyRxPort(const std::string& aId) :
  InputPort(aId) {
}

//---
DummyRxPort::~DummyRxPort() {
}

//---
bool DummyRxPort::isEnabled() const {
  return true;
}


//---
bool DummyRxPort::isLocked() const {
  return true;
}

//---
bool DummyRxPort::isAligned() const {
  return true;
}

//---
uint32_t DummyRxPort::getCRCErrors() const {
  return 0;
}


//---
DummyTxPort::DummyTxPort(const std::string& aId) :
OutputPort(aId){
}

//---
DummyTxPort::~DummyTxPort() {
}

//--
bool DummyTxPort::isEnabled() const {
  return true;
}

//--
bool DummyTxPort::isOperating() const {
  return true;
}


SWATCH_PROCESSOR_REGISTER_CLASS(DummyProcessor);

using std::cout;
using std::endl;

DummyProcessor::DummyProcessor(const std::string& aId, const core::XParameterSet& aPars)
    : processor::Processor(aId, aPars ) {
    
    LOG(swlog::kDebug) <<  "Id:" << this->id();
    LOG(swlog::kDebug) <<  "XParameterSet:";
    BOOST_FOREACH( std::string k, aPars.keys() ) {
       LOG(swlog::kDebug) <<  "   " << k << " : " << aPars[k];
    }
    std::stringstream ss;
    for ( int chan(0); chan < 4 ; ++chan) { 
        ss.str(std::string());
        ss << "rx" << std::setw(2) << std::setfill('0') << chan;
        this->addInput(new DummyRxPort(ss.str()));
    }
    
    for ( int chan(0); chan < 2 ; ++chan) { 
        ss.str(std::string());
        ss << "tx" << std::setw(2) << std::setfill('0') << chan;
        this->addOutput(new DummyTxPort(ss.str()));
    }
    
    
    swpro::ProcessorBag& stub = aPars.get<swpro::ProcessorBag>("stub");
    slot_ = stub.bag.slot;
    crate_ = stub.bag.crate;
    
}

DummyProcessor::~DummyProcessor() {
}

void
DummyProcessor::reset(const std::string& mode) {

}

const std::string&
DummyProcessor::getCrateId() const {
  return crate_;
}

uint32_t
DummyProcessor::getSlot() const {
  return slot_;
}



} // namespace test
} // namespace system
} // namespace swatch
