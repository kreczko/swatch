/**
 * @file    AMC13Service.cpp
 * @author  Alessandro Thea
 * @brief   Brief description
 * @date    07/11/14
 */

#include "swatch/hardware/AMC13Service.hpp"

// AMC13 Headers
#include "amc13/AMC13.hh"


namespace swatch {
namespace hardware {

AMC13Service::AMC13Service(const std::string& aId, const core::ParameterSet& params) :
    swatch::system::AMC13Service(aId, params),
    driver_(0x0) {

    std::string uriT1, uriT2;
    std::string addrTableT1, addrTableT2;

    try {
        crate_ = params.get<std::string>("crate");
        slot_ = params.get<uint32_t>("slot");

        uriT1 = params.get<std::string>("uriT1");
        addrTableT1 = params.get<std::string>("addrtabT1");
        uriT2 = params.get<std::string>("uriT2");
        addrTableT2 = params.get<std::string>("addrtabT2");

    } catch ( swatch::core::ParameterNotFound& e ) {
        // Don't proceed any further
        // TODO: Throw here
        return;
    }

    std::cout << addrTableT1.substr(7) << std::endl;
    std::cout << addrTableT2.substr(7) << std::endl;

    driver_ = new amc13::AMC13(uriT1, addrTableT1.substr(7),
                        uriT2, addrTableT2.substr(7) );

    driver_->getStatus()->Report(2);
}

AMC13Service::~AMC13Service() {

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
    driver_->getStatus()->Report(2);


}

uint32_t
AMC13Service::getSlot() const {
    return slot_;
}

const std::string&
AMC13Service::getCrateId() const {
    return crate_;
}


} // namespace hardware
} // namespace swatch