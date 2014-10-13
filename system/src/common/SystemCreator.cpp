// Boost Headers
#include <boost/foreach.hpp>

// Swatch Headers
#include "swatch/processor/ProcessorFactory.hpp"
#include "swatch/system/AMC13Service.hpp"
#include "swatch/system/SystemCreator.hpp"
#include "swatch/system/ServiceFactory.hpp"
#include "swatch/system/Crate.hpp"

using namespace swatch::core;
using namespace swatch::system;
using namespace swatch::processor;

SWATCH_SYSTEM_REGISTER_CREATOR(SystemCreator);

swatch::system::System*
SystemCreator::operator()(const std::string& aId, const ParameterSet& params) {
	// validity check should go here
	System* sys = create_system(aId, params);
	add_crates(sys, params);
	add_processors(sys, params);
	add_services(sys, params);

	return sys;
}

swatch::system::System*
SystemCreator::create_system(const std::string& aId, const ParameterSet& params){
	System* sys = new System(aId, params);
	return sys;
}

void SystemCreator::add_crates(System* system, const ParameterSet& params) {
	std::vector<ParameterSet> vPSets;
	// currently one needs to loop over all services and processors to get the crates
	vPSets = params.get<std::vector<ParameterSet> >("services");
	BOOST_FOREACH(ParameterSet& ps,vPSets) {
		std::string crate_id = ps.get<std::string>("crate");
		if(!system->hasCrate(crate_id)){
			add_crate(system, ps);
		}
	}
	vPSets = params.get<std::vector<ParameterSet> >("processors");
	BOOST_FOREACH(ParameterSet& ps,vPSets) {
		std::string crate_id = ps.get<std::string>("crate");
		if (!system->hasCrate(crate_id)) {
			add_crate(system, ps);
		}
	}
}

void SystemCreator::add_crate(System* system, const ParameterSet& params) {
	ParameterSet crate_params;
	std::string crate_id = params.get<std::string>("crate");
	std::string crate_loc = params.get<std::string>("CRATE LOCATION");
	std::string crate_desc = params.get<std::string>("CRATE DESCRIPTION");
	crate_params.insert("name", crate_id);
	crate_params.insert("location", crate_loc);
	crate_params.insert("description", crate_desc);
	Crate * crate = new Crate(crate_id, crate_params);
	system->add(crate);
}
void SystemCreator::add_processors(System* system, const ParameterSet& params) {
	std::vector<ParameterSet> vPSets;
	vPSets = params.get<std::vector<ParameterSet> >("processors");
	BOOST_FOREACH(ParameterSet& ps,vPSets) {
		Processor* p = ProcessorFactory::get()->make(
				ps.get<std::string>("PROCESSOR CREATOR"),
				ps.get<std::string>("PROCESSOR NAME"), ps);
		system->add(p);
	}
}

void SystemCreator::add_services(System* system, const ParameterSet& params) {
	std::vector<ParameterSet> vPSets;
	vPSets = params.get<std::vector<ParameterSet> >("services");
	BOOST_FOREACH(ParameterSet& ps,vPSets) {
		Service* a = static_cast<Service*>(ServiceFactory::get()->make(
				ps.get<std::string>("SERVICE CREATOR"),
				ps.get<std::string>("SERVICE NAME"), ps));
		system->add(a);
	}
}
