/* 
 * File:   CrateView.hpp
 * Author: ale
 *
 * Created on July 21, 2014, 10:12 AM
 */

#ifndef __swatch_system_crateview_hpp__
#define	__swatch_system_crateview_hpp__

#include "swatch/core/Object.hpp"

namespace swatch {

namespace processor {
class Processor;
}

namespace system {

class Service;
class DaqTTCService;

//! View class map swatch objects in crates. It doesn't own the objects it points to.
class Crate : public core::ObjectView {
public:
    Crate( const std::string id, const core::ParameterSet& params = core::ParameterSet() );
    virtual ~Crate();

    
    void add( system::DaqTTCService* aAMC13 );
    void add( processor::Processor* aProcessor );
    
    processor::Processor* amc( uint32_t slot );
    
    system::DaqTTCService* amc13() { return amc13_; }
    
    std::vector<uint32_t> getPopulatedSlots() const;
    bool isSlotTaken( uint32_t slot ) const;
    
private:
    Service* mch_;
    DaqTTCService* amc13_;
    std::vector<processor::Processor*> amcs_;
    
    uint32_t min_;
    uint32_t max_;
    
    friend class System;    
    friend std::ostream& operator<<(std::ostream& os, const Crate& cv);
    
};

}
}

#endif	/* __swatch_system_crateview_hpp__ */

