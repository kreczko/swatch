/* 
 * File:   System.cpp
 * Author: ale
 * 
 * Created on July 13, 2014, 11:20 AM
 */

#include "swatch/system/System.hpp"
#include "swatch/system/Crate.hpp"
#include "swatch/system/Service.hpp"
#include "swatch/system/AMC13Service.hpp"

#include "swatch/core/Link.hpp"
#include "swatch/processor/Processor.hpp"


// Swatch Headers
#include "swatch/core/Utilities.hpp"

// uHAL Headers
#include "uhal/log/log.hpp"

using namespace std;

namespace swatch {
namespace system {

System::System(const std::string& aId, const core::ParameterSet& params)
: Device(aId, params)
{
}

System::~System() {
}

const boost::unordered_map<std::string, Crate*>&
System::getCrates() const {
    return cratesMap_;
}

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
    	ss << "Cannot find Crate " << crateId;
    	throw runtime_error(ss.str());
    }
    else
    	cratesMap_[crateId]->add(aProcessor);
    
    uhal::log(uhal::Debug(),aProcessor->id(), " added (path = ", aProcessor->path(), ")");
}

void
System::add(system::AMC13Service* aAMC13) {
    if (aAMC13 == NULL)
        throw std::invalid_argument("AMC13 pointer is NULL!");
    // build a family
    this->addObj(aAMC13);
    
    // but keep it aside
    amc13s_.push_back(aAMC13);
    
    // and give it a different view
    std::string crateId = aAMC13->getCrateId();

    boost::unordered_map<std::string, Crate*>::iterator cit = cratesMap_.find(crateId);
    if (!hasCrate(crateId)) {
    	stringstream ss;
    	ss << "Cannot find Crate " << crateId;
    	throw runtime_error(ss.str());
    } else
    	cratesMap_[crateId]->add(aAMC13);
    
}
void
System::add(Service* aService) {
    if (aService == NULL)
        throw std::invalid_argument("AMC13 pointer is NULL!");
    this->addObj(aService);
    services_.push_back(aService);
}

void
System::add(core::Link* aLink) {
    if (aLink == NULL)
        throw std::invalid_argument("Link pointer is NULL!");
    this->addObj(aLink);
    links_.push_back(aLink);
}

void System::add( Crate* crate ){
    if (crate == NULL)
        throw std::invalid_argument ("Crate pointer is NULL!");
	this->addObj(crate);
	cratesMap_[crate->id()] = crate;
}

const std::deque<processor::Processor*>&
System::getProcessors() const {
    return processors_;
}

const std::deque<Service*>&
System::getServices() const {
    return services_;
}

const std::deque<core::Link*>&
System::getLinks() const {
    return links_;
}


bool
System::c_halt()
{
	return true;
}

void
System::f_halt(const core::ParameterSet& params)
{
}

bool
System::c_configure()
{
	return true;
}

void
System::f_configure(const core::ParameterSet& params)
{
    // TODO: Prepare parameter set
    for (deque<processor::Processor*>::iterator pIt = processors_.begin();
	 pIt != processors_.end();
	 ++pIt)
    {
	Device* d = dynamic_cast<Device*>(*pIt);
	d->configure();
	// (*pIt)->configure(params);
    }

}

bool System::hasCrate(const std::string& crate_id) const{
	return cratesMap_.find(crate_id) != cratesMap_.end();
}

} // end ns system
} // end ns swatch

