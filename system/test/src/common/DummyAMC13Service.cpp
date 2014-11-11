/* 
 * File:   DummyAMC13Service.cpp
 * Author: ale
 * 
 * Created on August 3, 2014, 9:18 PM
 */

#include "swatch/system/test/DummyAMC13Service.hpp"

// Boost Headers
#include <boost/foreach.hpp>

// uHAL Headers
//#include "uhal/log/log.hpp"

// Swatch Headers
#include "swatch/system/ServiceFactory.hpp"

// Namespace resolution
using namespace std;

namespace swatch {
namespace system {
namespace test {

SWATCH_SERVICE_REGISTER_CLASS(DummyAMC13Service);

DummyAMC13Service::DummyAMC13Service( const std::string& aId, const core::ParameterSet& params ) : system::AMC13Service(aId,params) {
//    using namespace uhal;
    cout << "Building a DummyAMC13Service" << endl;

    cout << "Id:" << this->id() << endl;;
    cout << "ParameterSet:" << endl;
    BOOST_FOREACH( std::string name, params.names() ) {
        cout << "   " << name << " : " << core::anyToString(params.get(name)) << endl;
    }

    crate_ = params.get<std::string>("crate");
    slot_ = params.get<int>("slot");
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
