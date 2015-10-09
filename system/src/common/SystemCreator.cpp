
#include "swatch/system/SystemCreator.hpp"


// boost headers
#include "boost/foreach.hpp"

// SWATCH headers
#include "swatch/core/Factory.hpp"

#include "swatch/processor/Port.hpp"
#include "swatch/processor/Processor.hpp"
#include "swatch/processor/Link.hpp"
#include "swatch/processor/LinkStub.hpp"

#include "swatch/system/Crate.hpp"
#include "swatch/system/CrateStub.hpp"
#include "swatch/system/CrateStub.hpp"
#include "swatch/dtm/DaqTTCManager.hpp"
#include "swatch/system/Service.hpp"
#include "swatch/system/System.hpp"


namespace swco = swatch::core;
namespace swpro = swatch::processor;


//SWATCH_REGISTER_CREATOR(swatch::system::SystemCreator);


namespace swatch {
namespace system {

/*
//---
//swatch::system::System*
swatch::core::Object*
SystemCreator::operator()(const swatch::core::AbstractStub& aStub) {
    // validity check should go here
    System* sys = createSystem(aStub);
    addCrates(sys);
    addProcessors(sys);
    addDaqTTCs(sys);
//    addServices(sys);
    addLinks(sys);

    sys->registerActions();

    return sys;
}


//---
swatch::system::System*
SystemCreator::createSystem(const swatch::core::AbstractStub& aStub){
  try {
    const SystemStub& sysStub = dynamic_cast<const swatch::system::SystemStub&>(aStub);
    System* sys = swco::Factory::get()->make<System>(sysStub.className, sysStub);
    return sys;
  }
  catch ( swatch::core::exception& xc ) {
    std::ostringstream xss;
    xss << "Failed to create System (id: '" << aStub.id << "'):" << std::endl << xc.what();
    throw SystemCreationFailed(xss.str());
  }
}


//---
void SystemCreator::addCrates(System* system) {
    
    BOOST_FOREACH(auto& cStub,system->getStub().crates) {
        addCrate(system, cStub);
    }
}


//---
void SystemCreator::addCrate(System* system, const CrateStub& aStub) {
  
  Crate * crate = new Crate(aStub);
  system->add(crate);

}


//---
void SystemCreator::addProcessors(System* system) {

  BOOST_FOREACH(auto& pStub,system->getStub().processors) {
    swpro::Processor* p;
    try {
      p = swco::Factory::get()->make<swpro::Processor>(pStub.creator, pStub);
    } catch ( swatch::core::exception& xc ) {
      std::ostringstream xss;
      xss << "Failed to create Processor (id: '" << pStub.id << "'):" << std::endl << xc.what();
      throw SystemCreationFailed(xss.str());
    }
    system->add(p);
  }
}


//---
void SystemCreator::addDaqTTCs(System* system) {

  BOOST_FOREACH(auto& dStub, system->getStub().daqttcs) {
    DaqTTCManager* daqttc;
    try {
      daqttc = swco::Factory::get()->make<DaqTTCManager>(dStub.creator, dStub);
    } catch (swatch::core::exception& xc) {
      std::ostringstream xss;
      xss << "Failed to create DaqTTCManager (id: '" << dStub.id << "'): " << std::endl << xc.what();
      throw SystemCreationFailed(xss.str());
    }
    system->add(daqttc);
  }
}


////---
//void SystemCreator::addServices(System* system) {
//  
//  BOOST_FOREACH(auto& sStub, system->getStub().services) {
//    Service* s = swco::Factory::get()->make<Service>(sStub);
//  
//    system->add(s);
//  }
//}



//---
void SystemCreator::addLinks(swatch::system::System* system) {

  BOOST_FOREACH(auto& lStub, system->getStub().links) {
    try {
      processor::OutputPort* srcPort = system->getObj<processor::OutputPort>(lStub.src);
      processor::InputPort*  dstPort = system->getObj<processor::InputPort>(lStub.dst);
    
      processor::Link * link = new processor::Link(lStub.id, srcPort,dstPort);
      system->add(link);
    }
    catch (swatch::core::exception& e) {
      std::ostringstream oss;
      oss << "Failed to create internal link (id: '" << lStub.id << "'): " << std::endl << e.what();
      throw SystemCreationFailed(oss.str());
    }
  }
}
*/
} // namespace system
} // namespace swatch
