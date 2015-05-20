/* 
 * File:   Port.cpp
 * Author: ale
 * 
 * Created on July 21, 2014, 9:57 AM
 */

#include "swatch/processor/Port.hpp"
#include "swatch/core/MonitorableObject.hpp"


namespace swatch {
namespace processor {

InputPort::InputPort( const std::string& aId) : 
    MonitorableObject(aId) 
{
  this->registerMetric<bool,InputPort>("isEnabled", *this, &InputPort::isEnabled, true, true);
  this->registerMetric<bool,InputPort>("isLocked", *this, &InputPort::isLocked, true, true);
  this->registerMetric<bool,InputPort>("isAligned", *this, &InputPort::isAligned, true, true);
  this->registerMetric<uint32_t,InputPort>("crcErrors", *this, &InputPort::getCRCErrors, 0, 0);
}


InputPort::InputPort( const std::string& aId, const core::XParameterSet& params ) : 
    MonitorableObject(aId, params) 
{
  this->registerMetric<bool,InputPort>("isEnabled", *this, &InputPort::isEnabled, true, true);
  this->registerMetric<bool,InputPort>("isLocked", *this, &InputPort::isLocked, true, true);
  this->registerMetric<bool,InputPort>("isAligned", *this, &InputPort::isAligned, true, true);
  this->registerMetric<uint32_t,InputPort>("crcErrors", *this, &InputPort::getCRCErrors, 0, 0);
}


std::vector<std::string> InputPort::getDefaultMetrics() {
    std::vector<std::string> result;
    result.push_back("isEnabled");
    result.push_back("isLocked");
    result.push_back("isAligned");
    result.push_back("crcErrors");
    return result;
}
  

OutputPort::OutputPort( const std::string& aId ) :
    MonitorableObject(aId )
{
    this->registerMetric<bool,OutputPort>("isEnabled", *this, &OutputPort::isEnabled, true, true);
    this->registerMetric<bool,OutputPort>("isOperating", *this, &OutputPort::isOperating, true, true);
}
    
OutputPort::OutputPort( const std::string& aId, const core::XParameterSet& params ) :
    MonitorableObject(aId, params) 
{
    this->registerMetric<bool,OutputPort>("isEnabled", *this, &OutputPort::isEnabled, true, true);
    this->registerMetric<bool,OutputPort>("isOperating", *this, &OutputPort::isOperating, true, true);
}


std::vector<std::string> OutputPort::getDefaultMetrics(){
  std::vector<std::string> result;
  result.push_back("isEnabled");
  result.push_back("isOperating");
  return result;
}

}
}
