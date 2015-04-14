// Boost Headers
#include <boost/foreach.hpp>
#include <xdata/Vector.h>

// Swatch Headers
#include "swatch/core/Factory.hpp"
#include "swatch/processor/Processor.hpp"
#include "swatch/system/DaqTTCManager.hpp"
#include "swatch/system/Service.hpp"
#include "swatch/system/System.hpp"
#include "swatch/system/SystemCreator.hpp"
#include "swatch/system/Crate.hpp"

#include "swatch/system/CrateStub.hpp"

namespace swco = swatch::core;
//namespace swsys = swatch::system;
namespace swpro = swatch::processor;

SWATCH_REGISTER_CREATOR(swatch::system::SystemCreator);


namespace swatch {
namespace system {

//---
//swatch::system::System*
swatch::core::Object*
SystemCreator::operator()(const std::string& aId, const swatch::core::XParameterSet& aPars) {
	// validity check should go here
	System* sys = createSystem(aId, aPars);
	addCrates(sys, aPars);
	addProcessors(sys, aPars);
	addDaqTTCs(sys, aPars);
	addServices(sys, aPars);

	return sys;
}


//---
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
    
  swpro::Processor* p;
  try {
		p = swco::Factory::get()->bake<swpro::Processor>(ps);
  } catch ( swatch::core::exception& xc ) {
    std::stringstream xss;
    xss << "Failed to create Processor:" << std::endl << xc.what();
    throw SystemCreationFailed(xss.str());
  }
		system->add(p);
	}
}


//---

void SystemCreator::addDaqTTCs(System* system, const swatch::core::XParameterSet& aPars) {

  // Carry on only if there are daqttcs to build
  if (not aPars.has("daqttcs")) return;

  xdata::Vector<swco::XParameterSet> vPSets;
  vPSets = aPars.get<xdata::Vector<swco::XParameterSet> >("daqttcs");

  BOOST_FOREACH(swco::XParameterSet& ps, vPSets) {
    DaqTTCManager* daqttc;
    try {
      daqttc = swco::Factory::get()->bake<DaqTTCManager>(ps);
    } catch (swatch::core::exception& xc) {
      std::stringstream xss;
      xss << "Failed to create DaqTTCManager: " << std::endl << xc.what();
      throw SystemCreationFailed(xss.str());
    }
    system->add(daqttc);
  }
}

//---
void SystemCreator::addServices(System* system, const swatch::core::XParameterSet& aPars) {
	xdata::Vector<swco::XParameterSet> vPSets;
  
  // Carry on only if there are services to build
  if ( not aPars.has("services") ) return;
	
  vPSets = aPars.get<xdata::Vector<swco::XParameterSet> >("services");
  BOOST_FOREACH(swco::XParameterSet& ps,vPSets) {
  	Service* a = swco::Factory::get()->bake<Service>(ps);
  
  	system->add(a);
  }
}

} // namespace system
} // namespace swatch
