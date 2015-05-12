/* 
 * File:   Processor.cpp
 * Author: ale
 * 
 * Created on July 11, 2014, 10:55 AM
 */


#include "swatch/processor/Processor.hpp"
#include "swatch/processor/ProcessorStub.hpp"

// Swatch Headers
#include "swatch/core/Utilities.hpp"

// Boost Headers
#include <boost/detail/container_fwd.hpp>
#include <boost/unordered/unordered_map.hpp>
#include <boost/foreach.hpp>

#include "swatch/processor/TTCInterface.hpp"
#include "swatch/processor/ReadoutInterface.hpp"


using namespace std;

namespace swatch {
namespace processor {

const uint32_t Processor::NoSlot =  0x7fffffffL;

///---
Processor::Processor( const std::string& aId, const core::XParameterSet& params ) :
    ActionableObject(aId, params),
    ttc_(0x0),
    readout_(0x0),
    algo_(0x0),
    device_(0x0)
{}

Processor::~Processor() {
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

core::Device*
Processor::device() {
    return device_;
}


} // namespace processor
} // namespace swatch

