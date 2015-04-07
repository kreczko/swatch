// Boost Headers
#include <boost/foreach.hpp>
#include <xdata/Vector.h>

// Swatch Headers
#include "swatch/processor/ProcessorFactory.hpp"
#include "swatch/system/DaqTTCService.hpp"
#include "swatch/system/SystemCreator.hpp"
#include "swatch/system/ServiceFactory.hpp"
#include "swatch/system/Crate.hpp"

#include "swatch/system/CrateStub.hpp"
#include "swatch/system/DaqTTCFactory.hpp"

namespace swco = swatch::core;
//namespace swsys = swatch::system;
namespace swpro = swatch::processor;


namespace swatch {
namespace system {

SWATCH_SYSTEM_REGISTER_CREATOR(SystemCreator);


swatch::system::System*
SystemCreator::operator()(const std::string& aId, const swatch::core::XParameterSet& aPars) {
	// validity check should go here
	System* sys = createSystem(aId, aPars);
	addCrates(sys, aPars);
	addProcessors(sys, aPars);
	addServices(sys, aPars);

	return sys;
}

swatch::system::System*
SystemCreator::createSystem(const std::string& aId, const swatch::core::XParameterSet& aPars){
	System* sys = new System(aId, aPars);
	return sys;
}

//---
void SystemCreator::addCrates(System* system, const swatch::core::XParameterSet& aPars) {
	xdata::Vector<swco::XParameterSet> vPSets;
    
	vPSets = aPars.get<xdata::Vector<swco::XParameterSet> >("crates");
    BOOST_FOREACH(swco::XParameterSet& ps,vPSets) {
        addCrate(system, ps);
    }

}


//---
void SystemCreator::addCrate(System* system, const swatch::core::XParameterSet& aPars) {
  	CrateBag& stub = aPars.get<CrateBag>("stub");
	Crate * crate = new Crate(stub.bag.name, aPars);
	system->add(crate);
}


//---
void SystemCreator::addProcessors(System* system, const swatch::core::XParameterSet& aPars) {
	xdata::Vector<swco::XParameterSet> vPSets;
	vPSets = aPars.get<xdata::Vector<swco::XParameterSet> >("processors");
	BOOST_FOREACH(swco::XParameterSet& ps,vPSets) {
		swpro::Processor* p = swpro::ProcessorFactory::get()->make(ps);
		system->add(p);
	}
}


//---
void SystemCreator::addDaqTTC(System* system, const swatch::core::XParameterSet& aPars) {
	xdata::Vector<swco::XParameterSet> vPSets;
	vPSets = aPars.get<xdata::Vector<swco::XParameterSet> >("daqttc");
	BOOST_FOREACH(swco::XParameterSet& ps,vPSets) {
		DaqTTCService* a = static_cast<DaqTTCService*>(DaqTTCFactory::get()->make(
                ps));

		system->add(a);
	}
}

//---
void SystemCreator::addServices(System* system, const swatch::core::XParameterSet& aPars) {
	xdata::Vector<swco::XParameterSet> vPSets;
	vPSets = aPars.get<xdata::Vector<swco::XParameterSet> >("services");
	BOOST_FOREACH(swco::XParameterSet& ps,vPSets) {
		Service* a = static_cast<Service*>(ServiceFactory::get()->make(
                ps));

		system->add(a);
	}
}

} // namespace system
} // namespace swatch
