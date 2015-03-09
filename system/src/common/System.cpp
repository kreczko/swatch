/* 
 * File:   System.cpp
 * Author: ale
 * 
 * Created on July 13, 2014, 11:20 AM
 */

#include "swatch/system/System.hpp"
#include "swatch/system/Crate.hpp"
#include "swatch/system/Service.hpp"
#include "swatch/system/DaqTTCService.hpp"

#include "swatch/core/Link.hpp"
#include "swatch/processor/Processor.hpp"


// Swatch Headers
#include "swatch/logger/Log.hpp"
#include "swatch/core/Utilities.hpp"

using namespace std;

namespace swlog = swatch::logger;

namespace swatch {
namespace system {


//---
System::System(const std::string& aId, const core::XParameterSet& params)
: Device(aId, params) {
}


//---
System::~System() {
}

//---
System::CratesMap&
System::getCrates() {
    return cratesMap_;
}

//---
void
System::add(processor::Processor* aProcessor) {
    if (aProcessor == NULL)
        throw std::invalid_argument("Processor pointer is NULL!");
    // check if slot and crate attribute are present
    if (aProcessor->getCrateId().empty() || aProcessor->getSlot() == processor::Processor::NoSlot) {
        throw runtime_error("No crate, no slot, no party");
    }

    // build a family
    this->addObj(aProcessor);

    // but keep it aside
    processors_.push_back(aProcessor);

    // and give it a different view
    std::string crateId = aProcessor->getCrateId();
    if (!hasCrate(crateId)){
    	stringstream ss;
    	ss << "System '" << this->id() << ";: Cannot find Crate " << crateId;
    	throw runtime_error(ss.str());
    }
    else
    	cratesMap_[crateId]->add(aProcessor);
    
    LOG(swlog::kDebug) <<  aProcessor->id() <<  " added (path = " << aProcessor->path() <<  ")";
}

//---
void
System::add(system::DaqTTCService* aAMC13) {
    if (aAMC13 == NULL)
        throw std::invalid_argument("AMC13 pointer is NULL!");
    // build a family
    this->addObj(aAMC13);
    
    // but keep it aside
    daqTtc_.push_back(aAMC13);
    // Is this still a service?
    services_.push_back(aAMC13);
    
        // and give it a different view
    std::string crateId = aAMC13->getCrateId();

    CratesMap::iterator cit = cratesMap_.find(crateId);
    if (!hasCrate(crateId)) {
    	stringstream ss;
    	ss << "System '" << this->id() << "': Cannot find Crate " << crateId;
    	throw runtime_error(ss.str());
    } else
    	cratesMap_[crateId]->add(aAMC13);
    
}


//---
void
System::add(Service* aService) {
  
  //TODO: This is a bit disgusting, to be revised along with the ServiceFactory interface.  
  system::DaqTTCService* daqttc = 0x0;
  
  if ( (daqttc = dynamic_cast<system::DaqTTCService*>(aService) ) != 0x0 ) {
    this->add(daqttc);
  } else {
    if (aService == NULL)
        throw std::invalid_argument("Service pointer is NULL!");
    this->addObj(aService);
    services_.push_back(aService);
  }
}


//---
void
System::add(core::Link* aLink) {
    if (aLink == NULL)
        throw std::invalid_argument("Link pointer is NULL!");
    this->addObj(aLink);
    links_.push_back(aLink);
}

//---
void
System::add( Crate* crate ){
    if (crate == NULL)
        throw std::invalid_argument ("Crate pointer is NULL!");
	this->addObj(crate);
	cratesMap_[crate->id()] = crate;
}

//---
std::deque<processor::Processor*>&
System::getProcessors() {
    return processors_;
}

std::deque<DaqTTCService*>&
System::getDaqTTC() {
    return daqTtc_;
}


//---
std::deque<Service*>&
System::getServices() {
    return services_;
}


//---
std::deque<core::Link*>&
System::getLinks() {
    return links_;
}


bool System::hasCrate(const std::string& crate_id) const{
	return cratesMap_.find(crate_id) != cratesMap_.end();
}

} // end ns system
} // end ns swatch

