/* 
 * File:   Processor.cpp
 * Author: ale
 * 
 * Created on July 11, 2014, 10:55 AM
 */

#include "swatch/processor/Processor.hpp"


// Boost Headers
#include <boost/detail/container_fwd.hpp>
#include <boost/unordered/unordered_map.hpp>
#include <boost/foreach.hpp>

// Swatch Headers
#include "swatch/core/AbstractStub.hpp"
#include "swatch/core/Utilities.hpp"
#include "swatch/processor/AlgoInterface.hpp"
#include "swatch/processor/PortCollection.hpp"
#include "swatch/processor/ProcessorStub.hpp"
#include "swatch/processor/ReadoutInterface.hpp"
#include "swatch/processor/TTCInterface.hpp"


using namespace std;

namespace swatch {
namespace processor {

  
//---
const std::string RunControlFSM::kId = "runControl";
const std::string RunControlFSM::kStateInitial = "Halted";
const std::string RunControlFSM::kStateError = "Error";
const std::string RunControlFSM::kStateSync = "Synchronised";
const std::string RunControlFSM::kStatePreCfg = "Preconfigured";
const std::string RunControlFSM::kStateCfg = "Configured";

const std::string RunControlFSM::kTrColdReset = "coldReset";
const std::string RunControlFSM::kTrSetup = "setup";
const std::string RunControlFSM::kTrPreCfg = "preconfigure";
const std::string RunControlFSM::kTrConnect = "connect";

//---
RunControlFSM::RunControlFSM(core::StateMachine& aFSM) : 
  fsm ( addStates(aFSM) ),
  coldReset ( fsm.addTransition(kTrColdReset, kStateInitial, kStateInitial) ),
  setup( fsm.addTransition(kTrSetup, kStateInitial, kStateSync ) ),
  preconfigure( fsm.addTransition(kTrPreCfg, kStateSync, kStatePreCfg) ),
  connect( fsm.addTransition(kTrConnect, kStatePreCfg, kStateCfg) )
{
}

//---
core::StateMachine& RunControlFSM::addStates(core::StateMachine& aFSM)
{
  aFSM.addState(kStateSync);
  aFSM.addState(kStatePreCfg);
  aFSM.addState(kStateCfg);
  return aFSM;
}

  
//---
const uint32_t Processor::NoSlot =  0x7fffffffL;

//---
Processor::Processor( const swatch::core::AbstractStub& aStub) :
    ActionableObject(aStub.id),
    metricFirmwareVersion_( registerMetric<uint64_t>("firmwareVersion") ),
    stub_(dynamic_cast<const processor::ProcessorStub&>(aStub)),
    mTTC(NULL),
    mReadout(NULL),
    mAlgo(NULL),
    mPorts(NULL),
    mRunControlFSM( registerStateMachine(RunControlFSM::kId, RunControlFSM::kStateInitial, RunControlFSM::kStateError) )
{
}

Processor::~Processor() {
}


//---
const ProcessorStub& Processor::getStub() const {
  return stub_;
}


//---
uint32_t
Processor::getSlot() const {
  return stub_.slot;
}


//---
const std::string&
Processor::getCrateId() const {
  return stub_.crate;
}


//---
TTCInterface&
Processor::getTTC() {
  if (mTTC == NULL)
    throw std::runtime_error("Processor \"" + getPath() + "\" has not registered any TTC interface object");
  else
    return *mTTC;
}


//---
ReadoutInterface&
Processor::getReadout() {
  if (mReadout == NULL)
    throw std::runtime_error("Processor \"" + getPath() + "\" has not registered any readout interface object");
  else
    return *mReadout;
}


//---
AlgoInterface&
Processor::getAlgo() {
  if (mAlgo == NULL)
    throw std::runtime_error("Processor \"" + getPath() + "\" has not registered any algo interface object");
  else
    return *mAlgo;
}


//---
PortCollection&
Processor::getPorts() {
  if (mPorts == NULL)
    throw std::runtime_error("Processor \"" + getPath() + "\" has not registered any link interface object");
  else
    return *mPorts;
}


//---
const std::vector<std::string> Processor::defaultMetrics = { "firmwareVersion" };


//---
const std::vector<std::string> Processor::defaultMonitorableObjects = { "ttc", "ports", "readout", "algo" };


//---
const std::vector<std::string>& Processor::getGateKeeperTables() const
{
  // Can't set the table names in constructor, since don't know parent at that time ...
  // ... instead, have to set tables names first time this method is called
  if( gateKeeperTables_.empty() )
  {
    gateKeeperTables_.push_back(getPath());

    std::string basePath = getPath();
    basePath.resize(basePath.size() - getId().size());
    gateKeeperTables_.push_back(basePath + getStub().role);
    gateKeeperTables_.push_back(basePath + "processors");
    gateKeeperTables_.push_back(basePath + getStub().hwtype);
  }
  return gateKeeperTables_;
}


//---
TTCInterface& Processor::registerInterface( TTCInterface* aTTCInterface )
{
  if( mTTC ){
    delete aTTCInterface;
    throw ProcessorInterfaceAlreadyDefined( "TTCInterface already defined for processor '" + getPath() + "'" );
  }
  this->addObj(aTTCInterface);
  mTTC = aTTCInterface;
  return *mTTC;
}


ReadoutInterface& Processor::registerInterface( ReadoutInterface* aReadoutInterface )
{
  if( mReadout ){
    delete aReadoutInterface;
    throw ProcessorInterfaceAlreadyDefined( "ReadoutInterface already defined for processor '" + getPath() + "'" );
  }
  this->addObj(aReadoutInterface);
  mReadout = aReadoutInterface;
  return *mReadout;
}


AlgoInterface& Processor::registerInterface( AlgoInterface* aAlgoInterface )
{
  if( mAlgo ){
    delete aAlgoInterface;
    throw ProcessorInterfaceAlreadyDefined( "AlgoInterface already defined for processor '" + getPath() + "'" );
  }
  this->addObj(aAlgoInterface);
  mAlgo = aAlgoInterface;
  return *mAlgo;
}


PortCollection& Processor::registerInterface( PortCollection* aPortCollection )
{
  if( mPorts ){
    delete aPortCollection;
    throw ProcessorInterfaceAlreadyDefined( "PortCollection already defined for processor '" + getPath() + "'" );
  }
  this->addObj(aPortCollection);
  mPorts = aPortCollection;
  return *mPorts;
}


RunControlFSM& Processor::getRunControlFSM()
{
  return mRunControlFSM;
}


} // namespace processor
} // namespace swatch

