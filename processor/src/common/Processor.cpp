/* 
 * File:   Processor.cpp
 * Author: ale
 * 
 * Created on July 11, 2014, 10:55 AM
 */


#include "swatch/processor/Processor.hpp"
#include "swatch/processor/ProcessorStub.hpp"

// Swatch Headers
#include "swatch/core/Utilities.hpp"

// Boost Headers
#include <boost/detail/container_fwd.hpp>
#include <boost/unordered/unordered_map.hpp>
#include <boost/foreach.hpp>

#include "swatch/processor/TTCInterface.hpp"
#include "swatch/processor/ReadoutInterface.hpp"
#include "swatch/processor/AlgoInterface.hpp"
#include "swatch/processor/LinkInterface.hpp"


using namespace std;

namespace swatch {
namespace processor {

const uint32_t Processor::NoSlot =  0x7fffffffL;

///---
Processor::Processor( const std::string& aId, const core::XParameterSet& params ) :
    ActionableObject(aId, params),
    ttc_(0x0),
    readout_(0x0),
    algo_(0x0),
    links_(0x0)
{
  registerMetric<uint64_t,Processor>("firmwareVersion", *this, &Processor::firmwareVersion, 0, 0);
}

Processor::~Processor() {
}


TTCInterface*
Processor::ttc() {
    return ttc_;
}


ReadoutInterface*
Processor::readout() {
    return readout_;
}


AlgoInterface*
Processor::algo() {
    return algo_;
}

LinkInterface*
Processor::linkInterface() {
    return links_;
}


std::vector<std::string> Processor::getDefaultMetrics() {
  std::vector<std::string> metrics;
  metrics.push_back("firmwareVersion");
  
  return metrics;
}


std::vector<std::string> Processor::getDefaultMonitorableObjects() {
  std::vector<std::string> objs;
  objs.push_back("ttc");
  objs.push_back("links");
  
  return objs;
}



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

