/* 
 * File:   Crate.cpp
 * Author: ale
 * Date: July 2014
 */

#include "swatch/system/Crate.hpp"


// C++ headers
#include <sstream>

// boost headers
#include <boost/foreach.hpp>

// SWATCH headers
#include "swatch/core/Utilities.hpp"
#include "swatch/processor/Processor.hpp"
#include "swatch/system/Service.hpp"
#include "swatch/system/DaqTTCManager.hpp"


// Namespace resolution
using namespace std;

namespace swatch {
namespace system {


Crate::Crate(const swatch::core::AbstractStub& aStub) :
  ObjectView(aStub.id), 
  stub_(dynamic_cast<const CrateStub&>(aStub)), 
  mch_(NULL), 
  amc13_(NULL), 
  amcs_(12, NULL), min_(1), max_(12) {
}


Crate::~Crate() {
}


void
Crate::add(system::DaqTTCManager* aAMC13) {
    amc13_ = aAMC13;
    addObj( aAMC13, "amc13" );
}


void
Crate::add(processor::Processor* aProcessor) {
    
    // Check the slot number range
    uint32_t slot = aProcessor->getSlot();
    if (slot < min_ || slot > max_) {
        stringstream ss;
        ss << "Crate '"<< getId() << "': Slot " << slot << " out of range";
        throw CrateSlotOutOfRange(ss.str());
    }    

    // Check if the slot is available
    if ( isSlotTaken(slot) ) {
        stringstream ss;
        ss << this->getId() << ": Cannot add card " << aProcessor->getId() << " to slot " << slot
                << ". Slot already assigned to card " << this->amcs_[slot-min_]->getId();
        throw CrateSlotTaken(ss.str());
    }
    
    // All clear, add the processor
    amcs_[slot-min_] = aProcessor;
    addObj(aProcessor, core::strPrintf("amc%02d", slot));
}


processor::Processor*
Crate::amc(uint32_t slot) {

    if (slot < min_ || slot > max_) {
        stringstream ss;
        ss << "Crate '"<< getId() << "': Slot " << slot << " out of range";
        
        throw CrateSlotOutOfRange(ss.str());
    }
    return amcs_[slot-min_];
}


std::vector<uint32_t>
Crate::getPopulatedSlots() const {
    std::vector<uint32_t> slots = getAMCSlots();
    
    if ( amc13_ ) slots.push_back(amc13_->getSlot());
    return slots;
}


std::vector<uint32_t>
Crate::getAMCSlots() const {
    std::vector<uint32_t> slots;
    BOOST_FOREACH( processor::Processor* p, amcs_ ) {
        if (p) slots.push_back(p->getSlot());
    }
    
    return slots;
}


bool Crate::isSlotTaken( uint32_t slot ) const {
  return amcs_[slot - min_] != NULL;
}


const CrateStub& Crate::getStub() const {
  return stub_; 
}


std::ostream&
operator<<(std::ostream& os, const swatch::system::Crate& cv) {
    os << "mch(" << cv.mch_ << ") amc13(" << cv.amc13_ << ")";
    for ( uint8_t i = cv.min_; i < cv.max_; ++i ) {
        os << " amc[" << (int)i << "](" << cv.amcs_[i] << ")"; 
    }
    return os;
}


}
}
