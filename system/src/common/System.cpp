/* 
 * File:   System.cpp
 * Author: ale
 * Date:   July 2014
 */

#include <boost/range/algorithm/remove_if.hpp>

#include "swatch/system/System.hpp"

// boost headers
#include "boost/foreach.hpp"
#include <boost/range/algorithm/copy.hpp>

// SWATCH headers
#include "swatch/logger/Log.hpp"
#include "swatch/core/Factory.hpp"
#include "swatch/core/SystemStateMachine.hpp"
#include "swatch/core/Utilities.hpp"
#include "swatch/system/Link.hpp"
#include "swatch/processor/Port.hpp"
#include "swatch/processor/Processor.hpp"
#include "swatch/system/Crate.hpp"
#include "swatch/system/Service.hpp"
#include "swatch/dtm/DaqTTCManager.hpp"
#include "swatch/processor/PortCollection.hpp"
#include <boost/range/adaptor/map.hpp>
#include <boost/lambda/lambda.hpp>

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
  stopFromAligned( fsm.addTransition(kTrStop, kStateAligned, kStateConfigured)),
  stopFromRunning( fsm.addTransition(kTrStop, kStateRunning, kStateConfigured)),
  stopFromPaused( fsm.addTransition(kTrStop, kStatePaused, kStateConfigured))
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
  
  validateConnectedFEDs();
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
const std::deque<system::Link*>& System::getLinks() {
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
System::add(system::Link* aLink) {
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
    if (std::count(getStub().excludedBoards.begin(), getStub().excludedBoards.end(), pStub.id) > 0)
      continue;
    
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
    if (std::count(getStub().excludedBoards.begin(), getStub().excludedBoards.end(), dStub.id) > 0)
      continue;
    
    try {
      dtm::DaqTTCManager* mgr = core::Factory::get()->make<dtm::DaqTTCManager>(dStub.creator, dStub);
      add(mgr);
    }
    catch (const core::exception& e) {
      std::ostringstream msg;
      msg << "Failed to create/add DaqTTCManager (id: '" << dStub.id << "'): " << e.what();
      throw SystemConstructionFailed(msg.str());
    }
  }
}

//---
void System::addLinks()
{
  BOOST_FOREACH(auto& lStub, getStub().links)
  {
    if (std::count(getStub().excludedBoards.begin(), getStub().excludedBoards.end(), lStub.srcProcessor) > 0)
      continue;
    else if (std::count(getStub().excludedBoards.begin(), getStub().excludedBoards.end(), lStub.dstProcessor) > 0)
      continue;

    try {
      processor::Processor&  srcProcessor = getObj<processor::Processor>(lStub.srcProcessor);
      processor::Processor&  dstProcessor = getObj<processor::Processor>(lStub.dstProcessor);
      processor::InputPort& srcPort = srcProcessor.getInputPorts().getPort(lStub.srcPort);
      processor::OutputPort&  dstPort = dstProcessor.getOutputPorts().getPort(lStub.dstPort);
      
      system::Link* link = new system::Link(lStub.id, &dstProcessor, &dstPort, &srcProcessor, &srcPort);
      add(link);
    }
    catch (const core::exception& e) {
      std::ostringstream msg;
      msg << "Failed to create/add internal link (id: '" << lStub.id << "'): " << e.what();
      throw SystemConstructionFailed(msg.str());
    }
  }
}


void System::validateConnectedFEDs()
{

  // Loop over FEdConnection map to check that all referenced objects exist and are InputPorts
  SystemStub::FEDInputPortsMap lMissing;  
  BOOST_FOREACH( auto fed, getStub().connectedFEDs ) {
    BOOST_FOREACH(std::string lPortPath, fed.second) {

      try {
        // Continue if id exists and can be cast to an input port.
        if ( this->getObjPtr<processor::InputPort>(lPortPath) ) continue;        
        lMissing[fed.first].push_back(lPortPath);
        
      } catch ( std::runtime_error& e ) {
        lMissing[fed.first].push_back(lPortPath);

      }
    }
  }
  
  if ( !lMissing.empty() ) {
    std::ostringstream msg;
    msg << "InputPorts referenced in FED Connection map not found: ";
    
    BOOST_FOREACH(auto fed, lMissing) {
      msg << "FED: " << fed.first << " ";
      boost::copy(fed.second |
          boost::adaptors::transformed("'"+boost::lambda::_1+"'"),
          std::ostream_iterator<std::string>(msg, " "));
    }

    throw SystemConstructionFailed(msg.str());

  }
  
}

RunControlFSM& System::getRunControlFSM() 
{
  return mRunControlFSM;
}


} // end ns system
} // end ns swatch

