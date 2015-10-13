/* 
 * File:   DaqTTCManager.cpp
 * Author: ale
 * Date:   July 2014
 */

#include "swatch/dtm/DaqTTCManager.hpp"


// SWATCH headers
#include "swatch/core/MetricConditions.hpp"
#include "swatch/core/StateMachine.hpp"


namespace swatch {
namespace dtm {

const std::string RunControlFSM::kId = "runControl";
const std::string RunControlFSM::kStateInitial = "HALTED";
const std::string RunControlFSM::kStateError = "ERROR";
const std::string RunControlFSM::kStateClockOK = "clockOK";
const std::string RunControlFSM::kStateCfg = "configured";
const std::string RunControlFSM::kStateRunning = "running";
const std::string RunControlFSM::kStatePaused = "paused";

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


DaqTTCManager::DaqTTCManager(const swatch::core::AbstractStub& aStub ) : 
  swatch::core::ActionableObject(aStub.id),
  stub_(dynamic_cast<const DaqTTCStub&>(aStub)),
  ttcMetricClockFreq_( registerMetric<double>("clkFreq", core::InvRangeCondition<double>(39.9e6, 40.1e6)) ),
  ttcMetricBC0Counter_( registerMetric<uint32_t>("bc0Counter") ),
  ttcMetricBC0Errors_( registerMetric<uint32_t>("bc0Errors", core::GreaterThanCondition<uint32_t>(0)) ),
  ttcMetricSingleBitErrors_( registerMetric<uint32_t>("ttcSingleBitErrors", core::GreaterThanCondition<uint32_t>(0)) ),
  ttcMetricDoubleBitErrors_( registerMetric<uint32_t>("ttcDoubleBitErrors", core::GreaterThanCondition<uint32_t>(0)) ),
  daqMetricFedId_( registerMetric<uint16_t>("fedId", core::NotEqualCondition<uint16_t>(stub_.fedId)) ),
  mRunControlFSM( registerStateMachine(RunControlFSM::kId, RunControlFSM::kStateInitial, RunControlFSM::kStateError) )
{
}


DaqTTCManager::~DaqTTCManager() {
}


const DaqTTCStub& DaqTTCManager::getStub() const {
  return stub_;
}


uint32_t DaqTTCManager::getSlot() const {
  return stub_.slot; 
}


const std::string& DaqTTCManager::getCrateId() const {
  return stub_.crate;
}


uint16_t DaqTTCManager::getFedId() const {
  return stub_.fedId;
}


const std::vector<std::string> DaqTTCManager::defaultMetrics = {"clkFreq", "bc0Counter", "bc0Errors", "ttcSingleBitErrors", "ttcDoubleBitErrors", "fedId"};


const std::vector<std::string>& DaqTTCManager::getGateKeeperTables() const
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

} // namespace dtm
} // namespace swatch

