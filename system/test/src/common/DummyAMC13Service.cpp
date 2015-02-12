/* 
 * File:   DummyAMC13Service.cpp
 * Author: ale
 * 
 * Created on August 3, 2014, 9:18 PM
 */

#include "swatch/system/test/DummyAMC13Service.hpp"

// Swatch Headers
#include <swatch/core/xoperators.hpp>

// Boost Headers
#include <boost/foreach.hpp>

// Swatch Headers
#include "swatch/logger/Log.hpp"
#include "swatch/system/ServiceFactory.hpp"
#include "swatch/system/AMC13ServiceStub.hpp"

// Namespace resolution
using namespace std;

namespace swlog = swatch::logger;
namespace swsys = swatch::system;


namespace swatch {
namespace system {
namespace test {

SWATCH_SERVICE_REGISTER_CLASS(DummyAMC13Service);

DummyAMC13Service::DummyAMC13Service( const std::string& aId, const core::XParameterSet& aPars ) : system::DaqTTCService(aId,aPars) {
//    using namespace uhal;
    LOG(swlog::kNotice) << "Building a DummyAMC13Service";

    LOG(swlog::kInfo) << "Id:" << this->id();;
    LOG(swlog::kInfo) << "XParameterSet:";
    BOOST_FOREACH( const std::string& name, aPars.keys() ) {
//      const xdata::Serializable& s = aPars[name];
      std::ostringstream oss;
//      oss << aPars[name];
//        LOG(swlog::kInfo) << "   " << name << " : " << s << aPars[name];
    }

    const swsys::AMC13ServiceStub& stub = aPars.get<swsys::AMC13ServiceStub>("descriptor");

    crate_ = stub.crate;
    slot_ = stub.slot;
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

std::set<std::string>
DummyAMC13Service::getModes() const {
    return std::set<std::string>();
}

void
DummyAMC13Service::reset(const std::string& mode) {
}


void
DummyAMC13Service::enableTTC(const std::vector<uint32_t>& aSlots) {
    
    cout << "Enabling slots ";
    BOOST_FOREACH( uint32_t s, aSlots ) {
        cout << s << " ";
    }
    cout << endl;
    
}



} // namespace test
} // namespace system
} // namespace swatch
