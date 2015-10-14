/* 
 * File:   DaqTTCManager.cpp
 * Author: ale
 * Date:   July 2014
 */

#include "swatch/dtm/DaqTTCManager.hpp"

#include "swatch/dtm/TTCInterface.hpp"
#include "swatch/dtm/AMCPortCollection.hpp"
#include "swatch/dtm/SLinkExpress.hpp"

// SWATCH headers
#include "swatch/core/MetricConditions.hpp"
#include "swatch/core/StateMachine.hpp"


namespace swatch {
namespace dtm {

const std::string RunControlFSM::kId = "runControl";
const std::string RunControlFSM::kStateInitial = "Halted";
const std::string RunControlFSM::kStateError = "Error";
const std::string RunControlFSM::kStateClockOK = "ClockOK";
const std::string RunControlFSM::kStateCfg = "Configured";
const std::string RunControlFSM::kStateRunning = "Running";
const std::string RunControlFSM::kStatePaused = "Paused";

const std::string RunControlFSM::kTrColdReset = "coldReset";
const std::string RunControlFSM::kTrClockSetup = "clockSetup";
const std::string RunControlFSM::kTrCfgDaq = "cfgDaq";
const std::string RunControlFSM::kTrStart = "start";
const std::string RunControlFSM::kTrPause = "pause";
const std::string RunControlFSM::kTrResume = "resume";
const std::string RunControlFSM::kTrStop = "stop";


RunControlFSM::RunControlFSM(core::StateMachine& aFSM) : 
  fsm( addStates(aFSM) ),
  coldReset( fsm.addTransition(kTrColdReset, kStateInitial, kStateInitial) ),
  clockSetup( fsm.addTransition(kTrClockSetup, kStateInitial, kStateClockOK) ),
  cfgDaq( fsm.addTransition(kTrCfgDaq, kStateClockOK, kStateCfg)),
  start( fsm.addTransition(kTrStart, kStateCfg, kStateRunning) ),
  pause( fsm.addTransition(kTrPause, kStateRunning, kStatePaused) ),
  resume( fsm.addTransition(kTrResume, kStatePaused, kStateRunning) ),
  stopFromPaused( fsm.addTransition(kTrStop, kStatePaused, kStateCfg) ),
  stopFromRunning( fsm.addTransition(kTrStop, kStateRunning, kStateCfg) )
{
}


core::StateMachine& RunControlFSM::addStates(core::StateMachine& aFSM)
{
  aFSM.addState(kStateClockOK);
  aFSM.addState(kStateCfg);
  aFSM.addState(kStateRunning);
  aFSM.addState(kStatePaused);
  return aFSM;
}

const std::vector<std::string> DaqTTCManager::defaultMetrics = {"fedId"};
const std::vector<std::string> DaqTTCManager::defaultMonitorableObjects = {"ttc","evb","amcports"};

DaqTTCManager::DaqTTCManager(const swatch::core::AbstractStub& aStub ) : 
  swatch::core::ActionableObject(aStub.id),
  stub_(dynamic_cast<const DaqTTCStub&>(aStub)),
  mTTC(0x0),
  mSLink(0x0),
  mAMCPorts(0x0),
  mEvb(0x0),
  mRunControlFSM( registerStateMachine(RunControlFSM::kId, RunControlFSM::kStateInitial, RunControlFSM::kStateError) ),
  daqMetricFedId_( registerMetric<uint16_t>("fedId", core::NotEqualCondition<uint16_t>(stub_.fedId)) )
{
}


DaqTTCManager::~DaqTTCManager() {
}


const DaqTTCStub& DaqTTCManager::getStub() const {
  return stub_;
}


uint32_t
DaqTTCManager::getSlot() const {
  return stub_.slot; 
}


const
std::string& DaqTTCManager::getCrateId() const {
  return stub_.crate;
}


uint16_t
DaqTTCManager::getFedId() const {
  return stub_.fedId;
}

const std::vector<std::string>&
DaqTTCManager::getGateKeeperTables() const
{
  // Can't set the table names in constructor, since don't know parent at that time ...
  // ... instead, have to set tables names first time this method is called
  if( gateKeeperTables_.empty() )
  {
    gateKeeperTables_.push_back(getPath());

    std::string basePath = getPath();
    basePath.resize(basePath.size() - getId().size());
    gateKeeperTables_.push_back(basePath + getStub().role);
    gateKeeperTables_.push_back(basePath + "daqttcs");
  }
  return gateKeeperTables_;
}


RunControlFSM& DaqTTCManager::getRunControlFSM()
{
  return mRunControlFSM;
}


// --------------------------------------------------------
TTCInterface& 
DaqTTCManager::getTTC() {
  return *mTTC;
}


// --------------------------------------------------------
dtm::AMCPortCollection& 
DaqTTCManager::getAMCPorts() {
  return *mAMCPorts;
}

// --------------------------------------------------------
TTCInterface&
DaqTTCManager::registerInterface(TTCInterface* aTTCInterface) {
  if (mTTC) {
    delete aTTCInterface;
    throw DaqTTCManagerInterfaceAlreadyDefined("TTCInterface already defined for amc13 '" + getPath() + "'");
  }
  this->addObj(aTTCInterface);
  mTTC = aTTCInterface;
  return *mTTC;
}


// --------------------------------------------------------
SLinkExpress&
DaqTTCManager::registerInterface(SLinkExpress* aSLink) {
  if (mSLink) {
    delete aSLink;
    throw DaqTTCManagerInterfaceAlreadyDefined("SLink already defined for amc13 '" + getPath() + "'");
  }
  this->addObj(aSLink);
  mSLink = aSLink;
  return *mSLink;
}

// --------------------------------------------------------
dtm::AMCPortCollection&
DaqTTCManager::registerInterface( dtm::AMCPortCollection* aAMCPortCollection )
{
  if( mAMCPorts ){
    delete aAMCPortCollection;
    throw DaqTTCManagerInterfaceAlreadyDefined( "PortCollection already defined for amc13 '" + getPath() + "'" );
  }
  this->addObj(aAMCPortCollection);
  mAMCPorts = aAMCPortCollection;
  return *mAMCPorts;
}

// --------------------------------------------------------
EVBInterface&
DaqTTCManager::registerInterface(EVBInterface* aEventBuilder) {
  if (mEvb) {
    delete aEventBuilder;
    throw DaqTTCManagerInterfaceAlreadyDefined("TTCInterface already defined for amc13 '" + getPath() + "'");
  }
  this->addObj(aEventBuilder);
  mEvb = aEventBuilder;
  return *mEvb;
}

} // namespace dtm
} // namespace swatch

