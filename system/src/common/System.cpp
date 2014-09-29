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
: Device(aId, params),
  fsm_(System::FsmStates::HALTED)
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

    // check if slot and crate attribute are present
    if (aProcessor->getCrateId().empty() || aProcessor->getSlot() == processor::Processor::NoSlot) {
        throw runtime_error("No crate, no slot, no party");
    }

    // check the slot number
    uint32_t slot = aProcessor->getSlot();
    if (slot < 1 || slot > 12) {
        stringstream ss;
        ss << "Slot " << slot << " out of range (card " << aProcessor->id() << ")";
        throw std::out_of_range(ss.str());
    }

    // build a family
    this->addObj(aProcessor);

    // but keep it aside
    processors_.push_back(aProcessor);

    // and give it a different view
    Crate* cptr;
    std::string crateId = aProcessor->getCrateId();

    boost::unordered_map<std::string, Crate*>::iterator cit = cratesMap_.find(crateId);
    if (cit != cratesMap_.end()) {
        cptr = cit->second;
    } else {
        cptr = new Crate(crateId);
        this->addObj(cptr);

        cratesMap_[crateId] = cptr;
    }

    cptr->add(aProcessor);
    
    uhal::log(uhal::Debug(),aProcessor->id(), " added (path = ", aProcessor->path(), ")");
}

void
System::add(system::AMC13Service* aAMC13) {
    // build a family
    this->addObj(aAMC13);
    
    // but keep it aside
    amc13s_.push_back(aAMC13);
    
    // and give it a different view
    Crate* cptr;
    std::string crateId = aAMC13->getCrateId();

    boost::unordered_map<std::string, Crate*>::iterator cit = cratesMap_.find(crateId);
    if (cit != cratesMap_.end()) {
        cptr = cit->second;
    } else {
        cptr = new Crate(crateId);
        this->addObj(cptr);

        cratesMap_[crateId] = cptr;
    }

    cptr->add(aAMC13);
    
    
}
void
System::add(Service* aService) {
    this->addObj(aService);
    services_.push_back(aService);
}

void
System::add(core::Link* aLink) {
    this->addObj(aLink);

    links_.push_back(aLink);
}

const std::deque<processor::Processor*>&
System::getProcessors() const {
    return processors_;
}

const std::deque<core::Link*>&
System::getLinks() const {
    return links_;
}

void
System::halt(const core::ParameterSet& params)
{

	ostringstream msg;

	if (!c_halt())
	{
		msg << "c_halt check failed! Could not finish transition to HALTED state from " << fsm_ << endl;
		throw runtime_error(msg.str());
	}

	f_halt(params);

	fsm_ = System::FsmStates::HALTED;
	msg << "FSM set to HALTED" << endl;
}

void
System::configure(const core::ParameterSet& params)
{

	ostringstream msg;

	if (fsm_ != System::FsmStates::HALTED)
	{
		msg << "Transition from state " << fsm_ << " to CONFIGURED not allowed!" << endl;
		throw runtime_error(msg.str());
	}


	if (!c_configure())
	{
		msg << "c_configure check failed! Could not finish transition to CONFIGURED state from " << fsm_ << endl;
		throw runtime_error(msg.str());
	}

	f_configure(params);


	fsm_ = System::FsmStates::CONFIGURED;
	msg << "FSM set to CONFIGURED" << endl;
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
}

System::FsmStates
System::getFSM()
{
	return fsm_;
}

void
System::setFSMState(System::FsmStates state)
{
	fsm_ = state;
}

} // end ns system
} // end ns swatch

