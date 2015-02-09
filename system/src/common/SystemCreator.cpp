// Boost Headers
#include <boost/foreach.hpp>

// Swatch Headers
#include "swatch/processor/ProcessorFactory.hpp"
#include "swatch/system/AMC13Service.hpp"
#include "swatch/system/SystemCreator.hpp"
#include "swatch/system/ServiceFactory.hpp"
#include "swatch/system/Crate.hpp"

#include "swatch/system/CrateStub.hpp"

namespace swco = swatch::core;
//namespace swsys = swatch::system;
namespace swpro = swatch::processor;


namespace swatch {
namespace system {

SWATCH_SYSTEM_REGISTER_CREATOR(SystemCreator);


swatch::system::System*
SystemCreator::operator()(const std::string& aId, const swatch::core::ParameterSet& params) {
	// validity check should go here
	System* sys = createSystem(aId, params);
	addCrates(sys, params);
	addProcessors(sys, params);
	addServices(sys, params);

	return sys;
}

swatch::system::System*
SystemCreator::createSystem(const std::string& aId, const swatch::core::ParameterSet& params){
	System* sys = new System(aId, params);
	return sys;
}

void SystemCreator::addCrates(System* system, const swatch::core::ParameterSet& params) {
	std::vector<swco::ParameterSet> vPSets;
    
	vPSets = params.get<std::vector<swco::ParameterSet> >("crates");
    BOOST_FOREACH(swco::ParameterSet& ps,vPSets) {
        addCrate(system, ps);
    }

}

void SystemCreator::addCrate(System* system, const swatch::core::ParameterSet& params) {
    const CrateStub& stub = params.get<CrateStub>("stub");
	Crate * crate = new Crate(stub.name, params);
	system->add(crate);
}
void SystemCreator::addProcessors(System* system, const swatch::core::ParameterSet& params) {
	std::vector<swco::ParameterSet> vPSets;
	vPSets = params.get<std::vector<swco::ParameterSet> >("processors");
	BOOST_FOREACH(swco::ParameterSet& ps,vPSets) {
		swpro::Processor* p = swpro::ProcessorFactory::get()->make(
                ps);
		system->add(p);
	}
}

void SystemCreator::addServices(System* system, const swatch::core::ParameterSet& params) {
	std::vector<swco::ParameterSet> vPSets;
	vPSets = params.get<std::vector<swco::ParameterSet> >("services");
	BOOST_FOREACH(swco::ParameterSet& ps,vPSets) {
		Service* a = static_cast<Service*>(ServiceFactory::get()->make(
                ps));
		system->add(a);
	}
}

} // namespace system
} // namespace swatch
