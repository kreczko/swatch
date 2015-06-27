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
Processor::Processor( const std::string& aId, const core::XParameterSet& params ) :
    ActionableObject(aId, params),
    metricFirmwareVersion_( registerMetric<uint64_t>("firmwareVersion") ),
    slot_(NoSlot),
    ttc_(NULL),
    readout_(NULL),
    algo_(NULL),
    links_(NULL)
{
  // Set crate and slot number from processor stub  
  processor::ProcessorBag& desc = params.get<processor::ProcessorBag>("stub");
  crateId_ = desc.bag.crate;
  slot_ = desc.bag.slot;
}

Processor::~Processor() {
}


uint32_t
Processor::getSlot() const {
  return slot_;
}


const std::string&
Processor::getCrateId() const {
  return crateId_;
}


TTCInterface&
Processor::ttc() {
  if (ttc_ == NULL)
    throw std::runtime_error("Processor \""+this->path()+"\" has not registered any TTC interface object");
  else
    return *ttc_;
}


ReadoutInterface&
Processor::readout() {
  if (readout_ == NULL)
    throw std::runtime_error("Processor \""+this->path()+"\" has not registered any readout interface object");
  else
    return *readout_;
}


AlgoInterface&
Processor::algo() {
  if (algo_ == NULL)
    throw std::runtime_error("Processor \""+this->path()+"\" has not registered any algo interface object");
  else
    return *algo_;
}


LinkInterface&
Processor::linkInterface() {
  if (links_ == NULL)
    throw std::runtime_error("Processor \""+this->path()+"\" has not registered any link interface object");
  else
    return *links_;
}


const std::vector<std::string> Processor::defaultMetrics = { "firmwareVersion" };


const std::vector<std::string> Processor::defaultMonitorableObjects = { "ttc", "links", "readout", "algo" };



void Processor::Add( TTCInterface* aTTCInterface )
{
  if( ttc_ ) throw TTCInterfaceAlreadyDefined( "TTCInterface already defined" );
  this->addObj(aTTCInterface);
  ttc_ = aTTCInterface;
}


void Processor::Add( ReadoutInterface* aReadoutInterface )
{
  if( readout_ ) throw ReadoutInterfaceAlreadyDefined( "ReadoutInterface already defined" );
  this->addObj(aReadoutInterface);
  readout_ = aReadoutInterface;
}

void Processor::Add( AlgoInterface* aAlgoInterface )
{
  if( algo_ ) throw AlgoInterfaceAlreadyDefined( "AlgoInterface already defined" );
  this->addObj(aAlgoInterface);
  algo_ = aAlgoInterface;
}

void Processor::Add( LinkInterface* aLinkInterface )
{
  if( links_ ) throw LinkInterfaceAlreadyDefined( "LinkInterface already defined" );
  this->addObj(aLinkInterface);
  links_ = aLinkInterface;
}


} // namespace processor
} // namespace swatch

