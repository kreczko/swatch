/* 
 * File:   Processor.cpp
 * Author: ale
 * 
 * Created on July 11, 2014, 10:55 AM
 */


#include "swatch/processor/Processor.hpp"

// Swatch Headers
#include "swatch/core/Utilities.hpp"

// Boost Headers
#include <boost/detail/container_fwd.hpp>
#include <boost/unordered/unordered_map.hpp>
#include <boost/foreach.hpp>

#include "swatch/processor/Controls.hpp"
#include "swatch/processor/TTCInterface.hpp"
#include "swatch/processor/ReadoutInterface.hpp"
#include "swatch/processor/OutputChannel.hpp"


using namespace std;

namespace swatch {
namespace processor {

const uint32_t Processor::NoSlot =  0x7fffffffL;

///---
Processor::Processor( const std::string& aId, const core::ParameterSet& params ) :
    Device(aId, params),
    ctrl_(0x0),
    ttc_(0x0),
    readout_(0x0),
    algo_(0x0) {
    
}

Processor::~Processor() {
}


Controls*
Processor::ctrl() {
    return ctrl_;
}


TTCInterface*
Processor::ttc() {
    return ttc_;
}


ReadoutInterface*
Processor::readout() {
    return readout_;
}


AlgoInterface*
Processor::algo() {
    return algo_;
}

const std::vector<InputChannel*>&
Processor::inputChannels() const {
    return inputChannels_;
}

const std::vector<OutputChannel*>&
Processor::outputChannels() const {
    return outputChannels_;
}

InputChannel*
Processor::inputChannel(uint32_t i) {
    return inputChannels_.at(i);
}

OutputChannel*
Processor::outputChannel(uint32_t i) {
    return outputChannels_.at(i);
}

// Standard state machine operations
bool
Processor::canHalt() {
	return true;
}

void
Processor::doHalt(const core::ParameterSet& params) {

}

bool
Processor::canConfigure() {
	return true;
}

void
Processor::doConfigure(const core::ParameterSet& params) {

}


} // namespace processor
} // namespace swatch

