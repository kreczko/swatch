/* 
 * File:   DummyProcessor.cpp
 * Author: ale
 * 
 * Created on July 27, 2014, 9:19 PM
 */

#include "swatch/system/test/DummyProcessor.hpp"

// Swatch Headers
#include "swatch/core/Utilities.hpp"
#include "swatch/core/Port.hpp"

#include "swatch/processor/ProcessorFactory.hpp"
#include <boost/foreach.hpp>

// C++ Headers
#include <iomanip>

// TOREMOVE
#include "uhal/log/log.hpp"


namespace swatch {
namespace system {
namespace test {

SWATCH_PROCESSOR_REGISTER_CLASS(DummyProcessor);

using std::cout;
using std::endl;

DummyProcessor::DummyProcessor(const std::string& aId, const core::Arguments& aArguments) 
    : processor::Processor(aId, aArguments ) {
    
    using namespace uhal;
        
    log(Debug(), "Id:",this->id());
    log(Debug(), "Arguments:");
    BOOST_FOREACH( std::string name, aArguments.names() ) {
        log(Debug(), "   ", name, " : ", core::anyToString(aArguments.get(name)));
    }
    std::stringstream ss;
    for ( int chan(0); chan < 4 ; ++chan) { 
        ss.str(std::string());
        ss << "rx" << std::setw(2) << std::setfill('0') << chan;
        this->addInput(new core::InputPort(ss.str()));
    }
    
    for ( int chan(0); chan < 2 ; ++chan) { 
        ss.str(std::string());
        ss << "tx" << std::setw(2) << std::setfill('0') << chan;
        this->addOutput(new core::OutputPort(ss.str()));
    }
    

    mSlot = aArguments.get<int>("slot",processor::Processor::NoSlot);
    mCrateId = aArguments.get<std::string>("crate","");
    
}

DummyProcessor::~DummyProcessor() {
}


} // namespace test
} // namespace system
} // namespace swatch
