/* 
 * File:   System.cpp
 * Author: ale
 * 
 * Created on July 13, 2014, 11:20 AM
 */

#include "swatch/system/System.hpp"
#include "swatch/system/Crate.hpp"
#include "swatch/system/Service.hpp"
#include "swatch/system/DaqTTCManager.hpp"

#include "swatch/processor/Link.hpp"
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
: ActionableObject(aId, params) {
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
System::add(system::DaqTTCManager* aAMC13) {
    if (aAMC13 == NULL)
        throw std::invalid_argument("AMC13 pointer is NULL!");
    // build a family
    this->addObj(aAMC13);
    
    // but keep it aside
    daqTtc_.push_back(aAMC13);
    // Is this still a service?
//    services_.push_back(aAMC13);
    
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
  if (aService == NULL)
    throw std::invalid_argument("Service pointer is NULL!");
  
    this->addObj(aService);
    services_.push_back(aService);
}


//---
void
System::add(processor::Link* aLink) {
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

std::deque<DaqTTCManager*>&
System::getDaqTTC() {
    return daqTtc_;
}


//---
std::deque<Service*>&
System::getServices() {
    return services_;
}


//---
std::deque<processor::Link*>&
System::getLinks() {
    return links_;
}


bool System::hasCrate(const std::string& crate_id) const{
	return cratesMap_.find(crate_id) != cratesMap_.end();
}

} // end ns system
} // end ns swatch

