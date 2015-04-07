/* 
 * File:   CrateView.hpp
 * Author: ale
 *
 * Created on July 21, 2014, 10:12 AM
 */

#ifndef __SWATCH_SYSTEM_CRATEVIEW_HPP__
#define	__SWATCH_SYSTEM_CRATEVIEW_HPP__

#include "swatch/core/Object.hpp"

namespace swatch {

namespace processor {
class Processor;
}

namespace system {

class Service;
class DaqTTCManager;

//! View class map swatch objects in crates. It doesn't own the objects it points to.
class Crate : public core::ObjectView {
public:
    Crate( const std::string id, const core::XParameterSet& aPars = core::XParameterSet() );
    virtual ~Crate();

    
    void add( system::DaqTTCManager* aAMC13 );
    void add( processor::Processor* aProcessor );
    
    processor::Processor* amc( uint32_t slot );
    
    system::DaqTTCManager* amc13() { return amc13_; }
    
    std::vector<uint32_t> getPopulatedSlots() const;
    
    std::vector<uint32_t> getAMCSlots() const;
    bool isSlotTaken( uint32_t slot ) const;
    
private:
    Service* mch_;
    DaqTTCManager* amc13_;
    std::vector<processor::Processor*> amcs_;
    
    uint32_t min_;
    uint32_t max_;
    
    friend class System;    
    friend std::ostream& operator<<(std::ostream& os, const Crate& cv);
    
};

DEFINE_SWATCH_EXCEPTION(CrateSlotTaken);
DEFINE_SWATCH_EXCEPTION(CrateSlotOutOfRange);

}
}

#endif	/* __SWATCH_SYSTEM_CRATEVIEW_HPP__ */

