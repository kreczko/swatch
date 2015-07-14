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
#include "swatch/processor/LinkInterface.hpp"
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
    slot_(NoSlot),
    ttc_(NULL),
    readout_(NULL),
    algo_(NULL),
    links_(NULL)
{
  // Set crate and slot number from processor stub  
  crateId_ = stub_.crate;
  slot_ = stub_.slot;
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
  return slot_;
}


//---
const std::string&
Processor::getCrateId() const {
  return crateId_;
}


//---
TTCInterface&
Processor::ttc() {
  if (ttc_ == NULL)
    throw std::runtime_error("Processor \""+this->path()+"\" has not registered any TTC interface object");
  else
    return *ttc_;
}


//---
ReadoutInterface&
Processor::readout() {
  if (readout_ == NULL)
    throw std::runtime_error("Processor \""+this->path()+"\" has not registered any readout interface object");
  else
    return *readout_;
}


//---
AlgoInterface&
Processor::algo() {
  if (algo_ == NULL)
    throw std::runtime_error("Processor \""+this->path()+"\" has not registered any algo interface object");
  else
    return *algo_;
}


//---
LinkInterface&
Processor::linkInterface() {
  if (links_ == NULL)
    throw std::runtime_error("Processor \""+this->path()+"\" has not registered any link interface object");
  else
    return *links_;
}

//---
const std::vector<std::string> Processor::defaultMetrics = { "firmwareVersion" };

//---
const std::vector<std::string> Processor::defaultMonitorableObjects = { "ttc", "links", "readout", "algo" };


//---
TTCInterface& Processor::registerInterface( TTCInterface* aTTCInterface )
{
  if( ttc_ ){
    delete aTTCInterface;
    throw ProcessorInterfaceAlreadyDefined( "TTCInterface already defined for processor '" + path() + "'" );
  }
  this->addObj(aTTCInterface);
  ttc_ = aTTCInterface;
  return *ttc_;
}


ReadoutInterface& Processor::registerInterface( ReadoutInterface* aReadoutInterface )
{
  if( readout_ ){
    delete aReadoutInterface;
    throw ProcessorInterfaceAlreadyDefined( "ReadoutInterface already defined for processor '" + path() + "'" );
  }
  this->addObj(aReadoutInterface);
  readout_ = aReadoutInterface;
  return *readout_;
}

AlgoInterface& Processor::registerInterface( AlgoInterface* aAlgoInterface )
{
  if( algo_ ){
    delete aAlgoInterface;
    throw ProcessorInterfaceAlreadyDefined( "AlgoInterface already defined for processor '" + path() + "'" );
  }
  this->addObj(aAlgoInterface);
  algo_ = aAlgoInterface;
  return *algo_;
}

LinkInterface& Processor::registerInterface( LinkInterface* aLinkInterface )
{
  if( links_ ){
    delete aLinkInterface;
    throw ProcessorInterfaceAlreadyDefined( "LinkInterface already defined for processor '" + path() + "'" );
  }
  this->addObj(aLinkInterface);
  links_ = aLinkInterface;
  return *links_;
}


} // namespace processor
} // namespace swatch

