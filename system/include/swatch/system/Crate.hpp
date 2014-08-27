/* 
 * File:   CrateView.hpp
 * Author: ale
 *
 * Created on July 21, 2014, 10:12 AM
 */

#ifndef __swatch_system_crateview_hpp__
#define	__swatch_system_crateview_hpp__

#include "swatch/core/Object.hpp"
#include "swatch/core/ParameterSet.hpp"

namespace swatch {

namespace processor {
class Processor;
}

namespace system {

class Service;
class AMC13Service;

//! View class map swatch objects in crates. It doesn't own the objects it points to.
class Crate : public core::ObjectView {
public:
    Crate( const std::string id, const core::Arguments& aAttributes = core::Arguments() );
    virtual ~Crate();

    
    void add( system::AMC13Service* aAMC13 );
    void add( processor::Processor* aProcessor );
    
    processor::Processor* amc( uint32_t slot );
    
    system::AMC13Service* amc13() { return amc13_; }
    
    std::vector<uint32_t> getPopulatedSlots() const;
    
private:
    Service* mch_;
    AMC13Service* amc13_;
    std::vector<processor::Processor*> amcs_;
    
    uint32_t min_;
    uint32_t max_;
    
    friend class System;    
    friend std::ostream& operator<<(std::ostream& os, const Crate& cv);
    
};

}
}

#endif	/* __swatch_system_crateview_hpp__ */

