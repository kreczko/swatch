/* 
 * File:   System.cpp
 * Author: ale
 * Date:   July 2014
 */

#include "swatch/system/System.hpp"

// boost headers
#include "boost/foreach.hpp"

// SWATCH headers
#include "swatch/logger/Log.hpp"
#include "swatch/core/Factory.hpp"
#include "swatch/core/SystemStateMachine.hpp"
#include "swatch/core/Utilities.hpp"
#include "swatch/processor/Link.hpp"
#include "swatch/processor/Port.hpp"
#include "swatch/processor/Processor.hpp"
#include "swatch/system/Crate.hpp"
#include "swatch/system/Service.hpp"
#include "swatch/system/DaqTTCManager.hpp"


SWATCH_REGISTER_CLASS(swatch::system::System)


using namespace std;
namespace swlog = swatch::logger;

namespace swatch {
namespace system {


const std::string SysRunControlFSM::kId = "runControl";
const std::string SysRunControlFSM::kStateInitial = "HALTED";
const std::string SysRunControlFSM::kStateError = "ERROR";
const std::string SysRunControlFSM::kStateSync = "synchronised";
const std::string SysRunControlFSM::kStatePreCfg = "preconfigured";
const std::string SysRunControlFSM::kStateCfg = "configured";
const std::string SysRunControlFSM::kStateRunning = "running";
const std::string SysRunControlFSM::kStatePaused = "paused";

const std::string SysRunControlFSM::kTrColdReset = "coldReset";
const std::string SysRunControlFSM::kTrSetup = "setup";
const std::string SysRunControlFSM::kTrPreCfg = "preconfigure";
const std::string SysRunControlFSM::kTrConnect = "connect";
const std::string SysRunControlFSM::kTrStart = "start";
const std::string SysRunControlFSM::kTrPause = "pause";
const std::string SysRunControlFSM::kTrResume = "resume";
const std::string SysRunControlFSM::kTrStop = "stop";


SysRunControlFSM::SysRunControlFSM(core::SystemStateMachine& aFSM) :
  fsm( addStates(aFSM) ),
  coldReset( fsm.addTransition(kTrColdReset, kStateInitial, kStateInitial)),
  setup( fsm.addTransition(kTrSetup, kStateInitial, kStateSync)),
  preconfigure( fsm.addTransition( kTrPreCfg, kStateSync, kStatePreCfg)),
  connect( fsm.addTransition(kTrConnect, kStatePreCfg, kStateCfg)),
  start( fsm.addTransition(kTrStart, kStateCfg, kStateRunning)),
  pause( fsm.addTransition(kTrPause, kStateRunning, kStatePaused)),
  resume( fsm.addTransition(kTrResume, kStatePaused, kStateRunning)),
  stopFromPaused( fsm.addTransition(kTrStop, kStatePaused, kStateCfg)),
  stopFromRunning( fsm.addTransition(kTrStop, kStateRunning, kStateCfg))
{
}


core::SystemStateMachine& SysRunControlFSM::addStates(core::SystemStateMachine& aFSM)
{
  aFSM.addState(kStateSync);
  aFSM.addState(kStatePreCfg);
  aFSM.addState(kStateCfg);
  aFSM.addState(kStateRunning);
  aFSM.addState(kStatePaused);
  return aFSM;
}


//---
System::System( const swatch::core::AbstractStub& aStub ) : 
  ActionableSystem(aStub.id),
  mRunControl( registerStateMachine(SysRunControlFSM::kId, SysRunControlFSM::kStateInitial, SysRunControlFSM::kStateError) ),
  stub_(dynamic_cast<const swatch::system::SystemStub&>(aStub))
{
  addCrates();
  addProcessors();
  addDaqTTCs();
//  addServices(sys);
  addLinks();
}


//---
System::~System() {
}


//---
const SystemStub& System::getStub() const {
  return stub_;
}


//---
const std::deque<processor::Processor*>& System::getProcessors() {
  return processors_;
}


//---
const std::deque<DaqTTCManager*>& System::getDaqTTC() {
  return daqTtc_;
}


//---
const std::deque<Service*>& System::getServices() {
  return services_;
}


//---
const std::deque<processor::Link*>& System::getLinks() {
  return links_;
}


//---
const System::CratesMap& System::getCrates() {
  return cratesMap_;
}


//---
bool System::hasCrate(const std::string& crate_id) const{
  return cratesMap_.find(crate_id) != cratesMap_.end();
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
  this->addObj(aProcessor, core::ActionableObject::Deleter());

  // but keep it aside
  processors_.push_back(aProcessor);

  // and give it a different view
  std::string crateId = aProcessor->getCrateId();
  if (!hasCrate(crateId)){
    stringstream ss;
    ss << "System '" << this->getId() << ";: Cannot find Crate " << crateId;
    throw runtime_error(ss.str());
  }
  else
    cratesMap_[crateId]->add(aProcessor);

  LOG(swlog::kDebug) <<  aProcessor->getId() <<  " added (path = " << aProcessor->getPath() <<  ")";
}


//---
void
System::add(system::DaqTTCManager* aAMC13) {
  if (aAMC13 == NULL)
    throw std::invalid_argument("AMC13 pointer is NULL!");

  // build a family
  this->addObj(aAMC13, core::ActionableObject::Deleter());

  // but keep it aside
  daqTtc_.push_back(aAMC13);
  // Is this still a service?
//    services_.push_back(aAMC13);

      // and give it a different view
  std::string crateId = aAMC13->getCrateId();

  CratesMap::iterator cit = cratesMap_.find(crateId);
  if (!hasCrate(crateId)) {
    stringstream ss;
    ss << "System '" << this->getId() << "': Cannot find Crate " << crateId;
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
void System::add( Crate* crate ){
  if (crate == NULL)
    throw std::invalid_argument ("Crate pointer is NULL!");
  this->addObj(crate);
  cratesMap_[crate->getId()] = crate;
}


//---
void System::addCrates()
{
  BOOST_FOREACH(auto& cStub, getStub().crates)
  {
    try {
      add( new Crate(cStub) );
    }
    catch (const core::exception& e ) {
      std::ostringstream oss;
      oss << "Failed to create/add crate (id: '" << cStub.id << "'):" << e.what();
      throw SystemConstructionFailed(oss.str());
    }
  }
}

//---
void System::addProcessors()
{
  BOOST_FOREACH(auto& pStub, getStub().processors)
  {
    try {
      processor::Processor* p = core::Factory::get()->make<processor::Processor>(pStub.creator, pStub);
      add(p);
    }
    catch (const core::exception& e ) {
      std::ostringstream oss;
      oss << "Failed to create/add Processor (id: '" << pStub.id << "'):" << e.what();
      throw SystemConstructionFailed(oss.str());
    }
  }
}

//---
void System::addDaqTTCs()
{
  BOOST_FOREACH(auto& dStub, getStub().daqttcs)
  {
    try {
      DaqTTCManager* mgr = core::Factory::get()->make<DaqTTCManager>(dStub.creator, dStub);
      add(mgr);
    }
    catch (const core::exception& e) {
      std::ostringstream oss;
      oss << "Failed to create/add DaqTTCManager (id: '" << dStub.id << "'): " << e.what();
      throw SystemConstructionFailed(oss.str());
    }
  }
}

//---
void System::addLinks()
{
  BOOST_FOREACH(auto& lStub, getStub().links)
  {
    try {
      processor::OutputPort* srcPort = getObj<processor::OutputPort>(lStub.src);
      processor::InputPort*  dstPort = getObj<processor::InputPort>(lStub.dst);
    
      processor::Link* link = new processor::Link(lStub.id, srcPort,dstPort);
      add(link);
    }
    catch (const core::exception& e) {
      std::ostringstream oss;
      oss << "Failed to create/add internal link (id: '" << lStub.id << "'): " << e.what();
      throw SystemConstructionFailed(oss.str());
    }
  }
}

} // end ns system
} // end ns swatch

