/* 
 * File:   Port.cpp
 * Author: ale
 * 
 * Created on July 21, 2014, 9:57 AM
 */

#include "swatch/processor/Port.hpp"
#include "swatch/core/Metric.hpp"
#include "swatch/core/MonitorableObject.hpp"

namespace swatch {
namespace processor {

InputPort::InputPort( const std::string& aId) : 
    MonitorableObject(aId),
    metricIsEnabled_( registerMetric<bool>("isEnabled", true, true) ),
    metricIsLocked_( registerMetric<bool>("isLocked", true, true) ),
    metricIsAligned_( registerMetric<bool>("isAligned", true, true) ),
    metricCRCErrors_( registerMetric<uint32_t>("crcErrors", 0, 0) )        
{
}

InputPort::InputPort( const std::string& aId, const core::XParameterSet& params ) : 
    MonitorableObject(aId, params),
    metricIsEnabled_( registerMetric<bool>("isEnabled", true, true) ),
    metricIsLocked_( registerMetric<bool>("isLocked", true, true) ),
    metricIsAligned_( registerMetric<bool>("isAligned", true, true) ),
    metricCRCErrors_( registerMetric<uint32_t>("crcErrors", 0, 0) )        
{
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
    MonitorableObject(aId ),
    metricIsEnabled_( registerMetric<bool>("isEnabled", true, true) ),
    metricIsOperating_( registerMetric<bool>("isOperating", true, true) )
{
}
    
OutputPort::OutputPort( const std::string& aId, const core::XParameterSet& params ) :
    MonitorableObject(aId, params),
    metricIsEnabled_( registerMetric<bool>("isEnabled", true, true) ),
    metricIsOperating_( registerMetric<bool>("isOperating", true, true) )
{
}


std::vector<std::string> OutputPort::getDefaultMetrics(){
  std::vector<std::string> result;
  result.push_back("isEnabled");
  result.push_back("isOperating");
  return result;
}

}
}
