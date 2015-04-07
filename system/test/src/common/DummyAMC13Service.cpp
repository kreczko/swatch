/* 
 * File:   DummyAMC13Service.cpp
 * Author: ale
 * 
 * Created on August 3, 2014, 9:18 PM
 */

#include "swatch/system/test/DummyAMC13Service.hpp"
#include "swatch/system/DaqTTCFactory.hpp"
// Boost Headers
#include <boost/foreach.hpp>

// Swatch Headers
#include "swatch/logger/Log.hpp"
#include "swatch/system/ServiceFactory.hpp"
#include "swatch/system/AMC13ServiceStub.hpp"
#include <swatch/core/xoperators.hpp>
#include <xdata/Integer.h>

// XDAQ Headers
#include "xdata/Serializable.h"

// Namespace resolution
using namespace std;

namespace swlog = swatch::logger;
namespace swsys = swatch::system;

SWATCH_DAQTTC_REGISTER_CLASS(swatch::system::test::DummyAMC13Service);

namespace swatch {
namespace system {
namespace test {

DummyAMC13Service::DummyAMC13Service( const std::string& aId, const core::XParameterSet& aPars ) : system::DaqTTCService(aId,aPars) {
    LOG(swlog::kInfo) << "Building a DummyAMC13Service";
    
    LOG(swlog::kDebug) << "Id:" << this->id();;
    LOG(swlog::kDebug) << "XParameterSet:";
    
    BOOST_FOREACH( const std::string& k, aPars.keys() ) {

        LOG(swlog::kDebug) << "   " << k << " : " << aPars[k];
    }

    swsys::AMC13ServiceBag& stub = aPars.get<swsys::AMC13ServiceBag>("stub");

    crate_ = stub.bag.crate;
    slot_ = stub.bag.slot;
}

DummyAMC13Service::~DummyAMC13Service() {

  
}

uint32_t
DummyAMC13Service::getSlot() const {
    return slot_;
}

const std::string&
DummyAMC13Service::getCrateId() const {
    return crate_;
}


void
DummyAMC13Service::reset() {
}


void
DummyAMC13Service::enableTTC(const std::vector<uint32_t>& aSlots) {
    
    cout << "Enabling slots ";
    BOOST_FOREACH( uint32_t s, aSlots ) {
        cout << s << " ";
    }
    cout << endl;
    
}

void DummyAMC13Service::configureClock(const std::string& mode) {

}


} // namespace test
} // namespace system
} // namespace swatch
