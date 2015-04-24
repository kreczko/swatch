/* 
 * File:   DummyAMC13Service.cpp
 * Author: ale
 * 
 * Created on August 3, 2014, 9:18 PM
 */

#include "swatch/system/test/DummyAMC13Manager.hpp"
#include "swatch/core/Factory.hpp"

// Boost Headers
#include <boost/foreach.hpp>

// Swatch Headers
#include "swatch/logger/Log.hpp"
#include "swatch/core/xoperators.hpp"
#include "swatch/system/DaqTTCStub.hpp"

// XDAQ Headers
#include "xdata/Serializable.h"
#include <xdata/Integer.h>

// Namespace resolution
using namespace std;

namespace swlog = swatch::logger;
namespace swsys = swatch::system;

SWATCH_REGISTER_CLASS(swatch::system::test::DummyAMC13Manager);

namespace swatch {
namespace system {
namespace test {

DummyAMC13Manager::DummyAMC13Manager( const std::string& aId, const core::XParameterSet& aPars ) : system::DaqTTCManager(aId,aPars) {
    LOG(swlog::kInfo) << "Building a DummyAMC13Service";
    
    LOG(swlog::kDebug) << "Id:" << this->id();;
    LOG(swlog::kDebug) << "XParameterSet:";
    
    BOOST_FOREACH( const std::string& k, aPars.keys() ) {

        LOG(swlog::kDebug) << "   " << k << " : " << aPars[k];
    }

    swsys::DaqTTCBag& stub = aPars.get<swsys::DaqTTCBag>("stub");

    crate_ = stub.bag.crate;
    slot_ = stub.bag.slot;
}

DummyAMC13Manager::~DummyAMC13Manager() {

  
}

uint32_t
DummyAMC13Manager::getSlot() const {
    return slot_;
}

const std::string&
DummyAMC13Manager::getCrateId() const {
    return crate_;
}


void
DummyAMC13Manager::reset() {
}


void
DummyAMC13Manager::enableTTC(const std::vector<uint32_t>& aSlots) {
    
    cout << "Enabling slots ";
    BOOST_FOREACH( uint32_t s, aSlots ) {
        cout << s << " ";
    }
    cout << endl;
    
}

void DummyAMC13Manager::configureClock(const std::string& mode) {

}


double DummyAMC13Manager::ttcClockFreq() const {
  return 4e7;
}

uint32_t DummyAMC13Manager::ttcBC0Counter() const {
  return 42;
}

uint32_t DummyAMC13Manager::ttcBC0Errors() const {
  return 0;
}    

uint32_t DummyAMC13Manager::ttcSingleBitErrors() const {
  return 0;
}    

uint32_t DummyAMC13Manager::ttcDoubleBitErrors() const {
  return 0;
}



} // namespace test
} // namespace system
} // namespace swatch
