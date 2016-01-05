/* 
 * File:   Crate.hpp
 * Author: ale
 */

#ifndef __SWATCH_SYSTEM_CRATE_HPP__
#define	__SWATCH_SYSTEM_CRATE_HPP__


#include "swatch/core/exception.hpp"
#include "swatch/core/Object.hpp"
#include "swatch/system/CrateStub.hpp"


namespace swatch {
namespace processor {
class Processor;
}
}


namespace swatch {

namespace dtm {
class DaqTTCManager;
} // namespace dtm

namespace system {

class Crate;
class Service;

std::ostream& operator<<(std::ostream& aStream, const swatch::system::Crate& aCrate);

//! Provides a view on the SWATCH objects in a crate. It doesn't own the objects it points to.
class Crate : public core::ObjectView {
public:
    Crate( const swatch::core::AbstractStub& aStub );
    virtual ~Crate();
    
    void add( dtm::DaqTTCManager* aAMC13 );
    void add( processor::Processor* aProcessor );
    
    processor::Processor* amc( uint32_t slot );
    
    dtm::DaqTTCManager* amc13() { return mAMC13; }
    
    std::vector<uint32_t> getPopulatedSlots() const;
    
    std::vector<uint32_t> getAMCSlots() const;
    bool isAMCSlotTaken( uint32_t slot ) const;
    
    const CrateStub& getStub() const;
    
private:
    CrateStub mStub;
    Service* mMCH;
    dtm::DaqTTCManager* mAMC13;
    std::vector<processor::Processor*> mAMCs;

    //! Lowest possible slot number for a processor
    const uint32_t mMinSlot;
    //! Highest possible slot number for a processor
    const uint32_t mMaxSlot;
    
    friend class System;    
    friend std::ostream& (operator<<) (std::ostream& aStream, const swatch::system::Crate& aCrate);
};

DEFINE_SWATCH_EXCEPTION(CrateSlotTaken);
DEFINE_SWATCH_EXCEPTION(CrateSlotOutOfRange);

}
}

#endif	/* __SWATCH_SYSTEM_CRATE_HPP__ */

