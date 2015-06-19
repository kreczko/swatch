/* 
 * File:   Port.cpp
 * Author: ale
 * 
 * Created on July 21, 2014, 9:57 AM
 */

#include "swatch/processor/Port.hpp"


#include "swatch/core/MetricConditions.hpp"


namespace swatch {
namespace processor {

InputPort::InputPort( const std::string& aId) : 
    MonitorableObject(aId),
    metricIsEnabled_( registerMetric<bool>("isEnabled", core::EqualCondition<bool>(false)) ),
    metricIsLocked_( registerMetric<bool>("isLocked", core::EqualCondition<bool>(false)) ),
    metricIsAligned_( registerMetric<bool>("isAligned", core::EqualCondition<bool>(false)) ),
    metricCRCErrors_( registerMetric<uint32_t>("crcErrors", core::GreaterThanCondition<uint32_t>(0)) )        
{
}

InputPort::InputPort( const std::string& aId, const core::XParameterSet& params ) : 
    MonitorableObject(aId, params),
    metricIsEnabled_( registerMetric<bool>("isEnabled", core::EqualCondition<bool>(false)) ),
    metricIsLocked_( registerMetric<bool>("isLocked", core::EqualCondition<bool>(false)) ),
    metricIsAligned_( registerMetric<bool>("isAligned", core::EqualCondition<bool>(false)) ),
    metricCRCErrors_( registerMetric<uint32_t>("crcErrors", core::GreaterThanCondition<uint32_t>(0)) )        
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
    metricIsEnabled_( registerMetric<bool>("isEnabled", core::EqualCondition<bool>(false)) ),
    metricIsOperating_( registerMetric<bool>("isOperating", core::EqualCondition<bool>(false)) )
{
}
    
OutputPort::OutputPort( const std::string& aId, const core::XParameterSet& params ) :
    MonitorableObject(aId, params),
    metricIsEnabled_( registerMetric<bool>("isEnabled", core::EqualCondition<bool>(false)) ),
    metricIsOperating_( registerMetric<bool>("isOperating", core::EqualCondition<bool>(false)) )
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
