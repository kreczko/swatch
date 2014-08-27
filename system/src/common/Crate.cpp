/* 
 * File:   CrateView.cpp
 * Author: ale
 * 
 * Created on July 21, 2014, 10:12 AM
 */

#include "swatch/system/Crate.hpp"

// Swatch Headers
#include "swatch/processor/Processor.hpp"
#include "swatch/system/Service.hpp"
#include "swatch/system/AMC13Service.hpp"

// Boost Headers
#include <boost/foreach.hpp>

// C++ Headers
#include <sstream>

// Namespace resolution
using namespace std;

namespace swatch {
namespace system {


Crate::Crate(const std::string aId, const core::Arguments& aArguments)
: ObjectView(aId), mch_(NULL), amc13_(NULL), amcs_(12, NULL), min_(1), max_(12) {
}

Crate::~Crate() {
}

void
Crate::add(system::AMC13Service* aAMC13) {
    amc13_ = aAMC13;
    addObj( aAMC13, "amc13" );
}

void
Crate::add(processor::Processor* aProcessor) {
    
    // Check the slot number range
    uint32_t slot = aProcessor->getSlot();
    if (slot < min_ || slot > max_) {
        stringstream ss;
        ss << "Slot " << slot << " out of range";
        throw std::out_of_range(ss.str());
    }    

    // Check if the slot is available
    if ( amcs_[slot]) {
        stringstream ss;
        ss << "Cannot add card " << this->id() << " to slot " << slot
                << ". Slot already assigned to card " << this->amcs_[slot]->id();
        throw runtime_error(ss.str());
    }
    
    // All clear, add the processor
    amcs_[slot-min_] = aProcessor;
    addObj(aProcessor, core::strPrintf("amc%02d", slot));
}

processor::Processor*
Crate::amc(uint32_t slot) {

    if (slot < min_ || slot > max_) {
        stringstream ss;
        ss << "Slot " << slot << " out of range";
        throw std::out_of_range(ss.str());
    }
    return amcs_[slot-min_];
}

std::vector<uint32_t>
Crate::getPopulatedSlots() const {
    std::vector<uint32_t> slots;
    BOOST_FOREACH( processor::Processor* p, amcs_ ) {
        if (p) slots.push_back(p->getSlot());
    }
    
    if ( amc13_ ) slots.push_back(amc13_->getSlot());
    return slots;
}

std::ostream&
operator<<(std::ostream& os, const Crate& cv) {
    os << "mch(" << cv.mch_ << ") amc13(" << cv.amc13_ << ")";
    for ( uint8_t i = cv.min_; i < cv.max_; ++i ) {
        os << " amc[" << (int)i << "](" << cv.amcs_[i] << ")"; 
    }
    return os;
}


}
}
