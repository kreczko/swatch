/* 
 * File:   DummyProcessor.cpp
 * Author: ale
 * 
 * Created on July 27, 2014, 9:19 PM
 */

#include "swatch/system/test/DummyProcessor.hpp"

// Swatch Headers
#include "swatch/logger/Log.hpp"
#include "swatch/core/Utilities.hpp"
#include "swatch/core/Port.hpp"
#include "swatch/processor/ProcessorStub.hpp"

#include "swatch/processor/ProcessorFactory.hpp"
#include <boost/foreach.hpp>

// C++ Headers
#include <iomanip>

// TOREMOVE
namespace swlog = swatch::logger;
namespace swpro = swatch::processor;

namespace swatch {
namespace system {
namespace test {

SWATCH_PROCESSOR_REGISTER_CLASS(DummyProcessor);

using std::cout;
using std::endl;

DummyProcessor::DummyProcessor(const std::string& aId, const core::ParameterSet& params)
    : processor::Processor(aId, params ) {
    
    LOG(swlog::kDebug) <<  "Id:" << this->id();
    LOG(swlog::kDebug) <<  "ParameterSet:";
    BOOST_FOREACH( std::string name, params.names() ) {
        LOG(swlog::kDebug) <<  "   " << name << " : " << core::anyToString(params.get(name));
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
    
    const swpro::ProcessorStub& stub = params.get<swpro::ProcessorStub>("descriptor");

    slot_ = stub.slot;
    crate_ = stub.crate;
    
}

DummyProcessor::~DummyProcessor() {
}

std::set<std::string> DummyProcessor::getModes() const {
    return std::set<std::string>();
}

void DummyProcessor::reset(const std::string& mode) {

}




} // namespace test
} // namespace system
} // namespace swatch
