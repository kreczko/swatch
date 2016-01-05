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
#include "swatch/dtm/DaqTTCManager.hpp"


SWATCH_REGISTER_CLASS(swatch::system::System)


using namespace std;
namespace swlog = swatch::logger;

namespace swatch {
namespace system {


const std::string RunControlFSM::kId = "runControl";
const std::string RunControlFSM::kStateInitial = "Halted";
const std::string RunControlFSM::kStateError = "Error";
const std::string RunControlFSM::kStateSync = "Synchronised";
const std::string RunControlFSM::kStateConfigured = "Configured";
const std::string RunControlFSM::kStateAligned = "Aligned";
const std::string RunControlFSM::kStateRunning = "Running";
const std::string RunControlFSM::kStatePaused = "Paused";

const std::string RunControlFSM::kTrColdReset = "coldReset";
const std::string RunControlFSM::kTrSetup = "setup";
const std::string RunControlFSM::kTrConfigure = "configure";
const std::string RunControlFSM::kTrAlign = "align";
const std::string RunControlFSM::kTrStart = "start";
const std::string RunControlFSM::kTrPause = "pause";
const std::string RunControlFSM::kTrResume = "resume";
const std::string RunControlFSM::kTrStop = "stop";


RunControlFSM::RunControlFSM(core::SystemStateMachine& aFSM) :
  fsm( addStates(aFSM) ),
  coldReset( fsm.addTransition(kTrColdReset, kStateInitial, kStateInitial)),
  setup( fsm.addTransition(kTrSetup, kStateInitial, kStateSync)),
  configure( fsm.addTransition( kTrConfigure, kStateSync, kStateConfigured)),
  align( fsm.addTransition(kTrAlign, kStateConfigured, kStateAligned)),
  start( fsm.addTransition(kTrStart, kStateAligned, kStateRunning)),
  pause( fsm.addTransition(kTrPause, kStateRunning, kStatePaused)),
  resume( fsm.addTransition(kTrResume, kStatePaused, kStateRunning)),
  stopFromPaused( fsm.addTransition(kTrStop, kStatePaused, kStateAligned)),
  stopFromRunning( fsm.addTransition(kTrStop, kStateRunning, kStateAligned))
{
}


core::SystemStateMachine& RunControlFSM::addStates(core::SystemStateMachine& aFSM)
{
  aFSM.addState(kStateSync);
  aFSM.addState(kStateConfigured);
  aFSM.addState(kStateAligned);
  aFSM.addState(kStateRunning);
  aFSM.addState(kStatePaused);
  return aFSM;
}


//---
System::System( const swatch::core::AbstractStub& aStub ) : 
  ActionableSystem(aStub.id, aStub.loggerName),
  mStub(dynamic_cast<const swatch::system::SystemStub&>(aStub)),
  mRunControlFSM( registerStateMachine(RunControlFSM::kId, RunControlFSM::kStateInitial, RunControlFSM::kStateError) )
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
  return mStub;
}


//---
const std::deque<processor::Processor*>& System::getProcessors() {
  return mProcessors;
}


//---
const std::deque<dtm::DaqTTCManager*>& System::getDaqTTCs() {
  return mDaqTtc;
}


//---
const std::deque<Service*>& System::getServices() {
  return mServices;
}


//---
const std::deque<processor::Link*>& System::getLinks() {
  return mLinks;
}


//---
const System::CratesMap_t& System::getCrates() {
  return mCratesMap;
}


//---
bool System::hasCrate(const std::string& aCrateId) const{
  return mCratesMap.find(aCrateId) != mCratesMap.end();
}


//---
void
System::add(processor::Processor* aProcessor) {
  if (aProcessor == NULL)
    throw std::invalid_argument("Processor pointer is NULL!");
  // check if slot and crate attribute are present
  if (aProcessor->getCrateId().empty() || aProcessor->getSlot() == processor::Processor::kNoSlot) {
    throw runtime_error("No crate, no slot, no party");
  }

  // build a family
  addActionable(aProcessor);

  // but keep it aside
  mProcessors.push_back(aProcessor);

  // and give it a different view
  std::string crateId = aProcessor->getCrateId();
  if (!hasCrate(crateId)){
    stringstream ss;
    ss << "System '" << this->getId() << ";: Cannot find Crate " << crateId;
    throw runtime_error(ss.str());
  }
  else
    mCratesMap[crateId]->add(aProcessor);

  LOG(swlog::kDebug) <<  aProcessor->getId() <<  " added (path = " << aProcessor->getPath() <<  ")";
}


//---
void
System::add(dtm::DaqTTCManager* aAMC13) {
  if (aAMC13 == NULL)
    throw std::invalid_argument("AMC13 pointer is NULL!");

  // build a family
  addActionable(aAMC13);

  // but keep it aside
  mDaqTtc.push_back(aAMC13);
  // Is this still a service?
//    services_.push_back(aAMC13);

      // and give it a different view
  std::string crateId = aAMC13->getCrateId();

  CratesMap_t::iterator cit = mCratesMap.find(crateId);
  if (!hasCrate(crateId)) {
    stringstream ss;
    ss << "System '" << this->getId() << "': Cannot find Crate " << crateId;
    throw runtime_error(ss.str());
  } else
    mCratesMap[crateId]->add(aAMC13);
}


//---
void
System::add(Service* aService) {
  if (aService == NULL)
    throw std::invalid_argument("Service pointer is NULL!");
  
  this->addObj(aService);
  mServices.push_back(aService);
}


//---
void
System::add(processor::Link* aLink) {
  if (aLink == NULL)
    throw std::invalid_argument("Link pointer is NULL!");
  this->addObj(aLink);
  mLinks.push_back(aLink);
}


//---
void System::add( Crate* crate ){
  if (crate == NULL)
    throw std::invalid_argument ("Crate pointer is NULL!");
  this->addObj(crate);
  mCratesMap[crate->getId()] = crate;
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
      dtm::DaqTTCManager* mgr = core::Factory::get()->make<dtm::DaqTTCManager>(dStub.creator, dStub);
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


RunControlFSM& System::getRunControlFSM() 
{
  return mRunControlFSM;
}


} // end ns system
} // end ns swatch

