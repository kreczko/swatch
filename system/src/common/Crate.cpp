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
#include "swatch/dtm/DaqTTCManager.hpp"


// Namespace resolution
using namespace std;

namespace swatch {
namespace system {


Crate::Crate(const swatch::core::AbstractStub& aStub) :
  ObjectView(aStub.id), 
  mStub(dynamic_cast<const CrateStub&>(aStub)), 
  mMCH(NULL), 
  mAMC13(NULL), 
  mAMCs(12, NULL), 
  mMinSlot(1), 
  mMaxSlot(12)
{
}


Crate::~Crate() {
}


void
Crate::add(dtm::DaqTTCManager* aAMC13) {
    mAMC13 = aAMC13;
    addObj( aAMC13, "amc13" );
}


void
Crate::add(processor::Processor* aProcessor) {
    
    // Check the slot number range
    uint32_t slot = aProcessor->getSlot();
    if (slot < mMinSlot || slot > mMaxSlot) {
        stringstream ss;
        ss << "Crate '"<< getId() << "': Slot " << slot << " out of range";
        throw CrateSlotOutOfRange(ss.str());
    }    

    // Check if the slot is available
    if ( isAMCSlotTaken(slot) ) {
        stringstream ss;
        ss << this->getId() << ": Cannot add card " << aProcessor->getId() << " to slot " << slot
                << ". Slot already assigned to card " << this->mAMCs[slot-mMinSlot]->getId();
        throw CrateSlotTaken(ss.str());
    }
    
    // All clear, add the processor
    mAMCs[slot-mMinSlot] = aProcessor;
    addObj(aProcessor, core::strPrintf("amc%02d", slot));
}


processor::Processor*
Crate::amc(uint32_t slot) {

    if (slot < mMinSlot || slot > mMaxSlot) {
        stringstream ss;
        ss << "Crate '"<< getId() << "': Slot " << slot << " out of range";
        
        throw CrateSlotOutOfRange(ss.str());
    }
    return mAMCs[slot-mMinSlot];
}


std::vector<uint32_t>
Crate::getPopulatedSlots() const {
    std::vector<uint32_t> slots = getAMCSlots();
    
    if ( mAMC13 ) slots.push_back(mAMC13->getSlot());
    return slots;
}


std::vector<uint32_t>
Crate::getAMCSlots() const {
    std::vector<uint32_t> slots;
    BOOST_FOREACH( processor::Processor* p, mAMCs ) {
        if (p) slots.push_back(p->getSlot());
    }
    
    return slots;
}


bool Crate::isAMCSlotTaken( uint32_t slot ) const {
  return mAMCs[slot - mMinSlot] != NULL;
}


const CrateStub& Crate::getStub() const {
  return mStub; 
}


std::ostream&
operator<<(std::ostream& os, const swatch::system::Crate& cv) {
    os << "mch(" << cv.mMCH << ") amc13(" << cv.mAMC13 << ")";
    for ( uint8_t i = cv.mMinSlot; i < cv.mMaxSlot; ++i ) {
        os << " amc[" << (int)i << "](" << cv.mAMCs[i] << ")"; 
    }
    return os;
}


}
}
