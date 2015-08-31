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

const uint32_t Processor::NoSlot =  0x7fffffffL;

///---
Processor::Processor( const swatch::core::AbstractStub& aStub) :
    ActionableObject(aStub.id),
    metricFirmwareVersion_( registerMetric<uint64_t>("firmwareVersion") ),
    stub_(dynamic_cast<const processor::ProcessorStub&>(aStub)),
    ttc_(NULL),
    readout_(NULL),
    algo_(NULL),
    ports_(NULL)
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
  if (ttc_ == NULL)
    throw std::runtime_error("Processor \"" + getPath() + "\" has not registered any TTC interface object");
  else
    return *ttc_;
}


//---
ReadoutInterface&
Processor::getReadout() {
  if (readout_ == NULL)
    throw std::runtime_error("Processor \"" + getPath() + "\" has not registered any readout interface object");
  else
    return *readout_;
}


//---
AlgoInterface&
Processor::getAlgo() {
  if (algo_ == NULL)
    throw std::runtime_error("Processor \"" + getPath() + "\" has not registered any algo interface object");
  else
    return *algo_;
}


//---
PortCollection&
Processor::getPorts() {
  if (ports_ == NULL)
    throw std::runtime_error("Processor \"" + getPath() + "\" has not registered any link interface object");
  else
    return *ports_;
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
  if( ttc_ ){
    delete aTTCInterface;
    throw ProcessorInterfaceAlreadyDefined( "TTCInterface already defined for processor '" + getPath() + "'" );
  }
  this->addObj(aTTCInterface);
  ttc_ = aTTCInterface;
  return *ttc_;
}


ReadoutInterface& Processor::registerInterface( ReadoutInterface* aReadoutInterface )
{
  if( readout_ ){
    delete aReadoutInterface;
    throw ProcessorInterfaceAlreadyDefined( "ReadoutInterface already defined for processor '" + getPath() + "'" );
  }
  this->addObj(aReadoutInterface);
  readout_ = aReadoutInterface;
  return *readout_;
}


AlgoInterface& Processor::registerInterface( AlgoInterface* aAlgoInterface )
{
  if( algo_ ){
    delete aAlgoInterface;
    throw ProcessorInterfaceAlreadyDefined( "AlgoInterface already defined for processor '" + getPath() + "'" );
  }
  this->addObj(aAlgoInterface);
  algo_ = aAlgoInterface;
  return *algo_;
}


PortCollection& Processor::registerInterface( PortCollection* aPortCollection )
{
  if( ports_ ){
    delete aPortCollection;
    throw ProcessorInterfaceAlreadyDefined( "PortCollection already defined for processor '" + getPath() + "'" );
  }
  this->addObj(aPortCollection);
  ports_ = aPortCollection;
  return *ports_;
}


} // namespace processor
} // namespace swatch

