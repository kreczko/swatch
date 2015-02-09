/**
 * @file    AMC13Service.cpp
 * @author  Alessandro Thea
 * @brief   Brief description
 * @date    07/11/14
 */

#include "swatch/hardware/AMC13Service.hpp"

// Swatch Headers
#include "swatch/system/AMC13ServiceStub.hpp"
#include "swatch/system/ServiceFactory.hpp"

// AMC13 Headers
#include "amc13/AMC13.hh"

// Boost Headers
#include <boost/assign.hpp>

namespace swatch {
namespace hardware {
SWATCH_SERVICE_REGISTER_CLASS(AMC13Service)

AMC13Service::AMC13Service(const std::string& aId, const core::ParameterSet& params) :
    swatch::system::DaqTTCService(aId, params),
    driver_(0x0) {
    
    using namespace boost::assign;
    modes_ += "ttsloopback", "external";
            
    const system::AMC13ServiceStub& desc = params.get<system::AMC13ServiceStub>("descriptor");
    
    crate_ = desc.crate;
    slot_  = desc.slot;

    driver_ = new amc13::AMC13(desc.uriT1, desc.addressTableT1.substr(7),
                        desc.uriT2, desc.addressTableT2.substr(7) );
    

    driver_->getStatus()->Report(1);
}

AMC13Service::~AMC13Service() {

}

uint32_t
AMC13Service::getSlot() const {
    return slot_;
}

const std::string&
AMC13Service::getCrateId() const {
    return crate_;
}

void
AMC13Service::enableTTC(const std::vector<uint32_t>& slots) {
    uint32_t mask(0x0);
    BOOST_FOREACH( uint32_t s, slots ) {
        mask |= 1 << (s-1);
    }
    std::cout << "AMC mask: 0x" << std::hex <<  mask << std::endl;
    // apply mask
    driver_->AMCInputEnable(mask);

    // and check the status
    driver_->getStatus()->Report(1);


}

std::set<std::string> AMC13Service::getModes() const {
    return modes_;            
}

void AMC13Service::reset(const std::string& mode) {
    if ( modes_.count(mode) == 0 ) {
        throw std::runtime_error("Invalid AMC13 mode "+mode );
    }
    
    if ( mode == "ttsloopback" ) {
        driver_->reset(amc13::AMC13Simple::T1);
        driver_->reset(amc13::AMC13Simple::T2);
        std::cout << "Enabling local TTC" << std::endl;
        driver_->localTtcSignalEnable(true);
        
    } else {
//        std::cout << "Mode " << mode << "not supported yet" << std::endl;
        driver_->reset(amc13::AMC13Simple::T1);
        driver_->reset(amc13::AMC13Simple::T2);
        std::cout << "Enabling local TTC" << std::endl;
        driver_->localTtcSignalEnable(false);
    }
    
    // and check the status
    driver_->getStatus()->Report(1);
}

} // namespace hardware
} // namespace swatch