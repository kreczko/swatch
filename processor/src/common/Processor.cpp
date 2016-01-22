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

// Static initialization
  
//---
const std::string RunControlFSM::kId = "runControl";
const std::string RunControlFSM::kStateInitial = "Halted";
const std::string RunControlFSM::kStateError = "Error";
const std::string RunControlFSM::kStateSync = "Synchronised";
const std::string RunControlFSM::kStateConfigured = "Configured";
const std::string RunControlFSM::kStateAligned = "Aligned";

const std::string RunControlFSM::kTrColdReset = "coldReset";
const std::string RunControlFSM::kTrSetup = "setup";
const std::string RunControlFSM::kTrConfigure = "configure";
const std::string RunControlFSM::kTrAlign = "align";

//---
RunControlFSM::RunControlFSM(core::StateMachine& aFSM) : 
  fsm ( addStates(aFSM) ),
  coldReset ( fsm.addTransition(kTrColdReset, kStateInitial, kStateInitial) ),
  setup( fsm.addTransition(kTrSetup, kStateInitial, kStateSync ) ),
  configure( fsm.addTransition(kTrConfigure, kStateSync, kStateConfigured) ),
  align( fsm.addTransition(kTrAlign, kStateConfigured, kStateAligned) )
{
}

//---
core::StateMachine& RunControlFSM::addStates(core::StateMachine& aFSM)
{
  aFSM.addState(kStateSync);
  aFSM.addState(kStateConfigured);
  aFSM.addState(kStateAligned);
  return aFSM;
}

  
//---
const uint32_t Processor::kNoSlot =  0x7fffffffL;
const std::vector<std::string> Processor::kDefaultMetrics = { "firmwareVersion" };
const std::vector<std::string> Processor::kDefaultMonitorableObjects = { "ttc", "ports", "readout", "algo" };

//---
Processor::Processor( const swatch::core::AbstractStub& aStub) :
    ActionableObject(aStub.id, aStub.loggerName),
    metricFirmwareVersion_( registerMetric<uint64_t>("firmwareVersion") ),
    mStub(dynamic_cast<const processor::ProcessorStub&>(aStub)),
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
  return mStub;
}


//---
uint32_t
Processor::getSlot() const {
  return mStub.slot;
}


//---
const std::string&
Processor::getCrateId() const {
  return mStub.crate;
}


//---
const TTCInterface&
Processor::getTTC() const {
  if (mTTC == NULL)
    throw std::runtime_error("Processor \"" + getPath() + "\" has not registered any TTC interface object");
  else
    return *mTTC;
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
const ReadoutInterface&
Processor::getReadout() const {
  if (mReadout == NULL)
    throw std::runtime_error("Processor \"" + getPath() + "\" has not registered any readout interface object");
  else
    return *mReadout;
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
const AlgoInterface&
Processor::getAlgo() const {
  if (mAlgo == NULL)
    throw std::runtime_error("Processor \"" + getPath() + "\" has not registered any algo interface object");
  else
    return *mAlgo;
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
const PortCollection&
Processor::getPorts() const {
  if (mPorts == NULL)
    throw std::runtime_error("Processor \"" + getPath() + "\" has not registered any link interface object");
  else
    return *mPorts;
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
const std::vector<std::string>& Processor::getGateKeeperTables() const
{
  // Can't set the table names in constructor, since don't know parent at that time ...
  // ... instead, have to set tables names first time this method is called
  if( mGateKeeperTables.empty() )
  {
    mGateKeeperTables.push_back(getPath());

    std::string basePath = getPath();
    basePath.resize(basePath.size() - getId().size());
    mGateKeeperTables.push_back(basePath + getStub().role);
    mGateKeeperTables.push_back(basePath + "processors");
    mGateKeeperTables.push_back(basePath + getStub().hwtype);
  }
  return mGateKeeperTables;
}


//---
TTCInterface& Processor::registerInterface( TTCInterface* aTTCInterface )
{
  if( mTTC ){
    delete aTTCInterface;
    throw ProcessorInterfaceAlreadyDefined( "TTCInterface already defined for processor '" + getPath() + "'" );
  }
  this->addMonitorable(aTTCInterface);
  mTTC = aTTCInterface;
  return *mTTC;
}


ReadoutInterface& Processor::registerInterface( ReadoutInterface* aReadoutInterface )
{
  if( mReadout ){
    delete aReadoutInterface;
    throw ProcessorInterfaceAlreadyDefined( "ReadoutInterface already defined for processor '" + getPath() + "'" );
  }
  this->addMonitorable(aReadoutInterface);
  mReadout = aReadoutInterface;
  return *mReadout;
}


AlgoInterface& Processor::registerInterface( AlgoInterface* aAlgoInterface )
{
  if( mAlgo ){
    delete aAlgoInterface;
    throw ProcessorInterfaceAlreadyDefined( "AlgoInterface already defined for processor '" + getPath() + "'" );
  }
  this->addMonitorable(aAlgoInterface);
  mAlgo = aAlgoInterface;
  return *mAlgo;
}


PortCollection& Processor::registerInterface( PortCollection* aPortCollection )
{
  if( mPorts ){
    delete aPortCollection;
    throw ProcessorInterfaceAlreadyDefined( "PortCollection already defined for processor '" + getPath() + "'" );
  }
  this->addMonitorable(aPortCollection);
  mPorts = aPortCollection;
  return *mPorts;
}


RunControlFSM& Processor::getRunControlFSM()
{
  return mRunControlFSM;
}


} // namespace processor
} // namespace swatch

