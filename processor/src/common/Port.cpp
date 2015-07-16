/* 
 * File:   Port.cpp
 * Author: ale
 */

#include "swatch/processor/Port.hpp"


#include "swatch/core/MetricConditions.hpp"


namespace swatch {
namespace processor {

InputPort::InputPort( const std::string& aId) : 
    MonitorableObject(aId),
    metricIsLocked_( registerMetric<bool>("isLocked", core::EqualCondition<bool>(false)) ),
    metricIsAligned_( registerMetric<bool>("isAligned", core::EqualCondition<bool>(false)) ),
    metricCRCErrors_( registerMetric<uint32_t>("crcErrors", core::GreaterThanCondition<uint32_t>(0)) )        
{
}


const std::vector<std::string> InputPort::defaultMetrics = { "isLocked", "isAligned", "crcErrors" };
  



OutputPort::OutputPort( const std::string& aId ) :
    MonitorableObject(aId ),
    metricIsOperating_( registerMetric<bool>("isOperating", core::EqualCondition<bool>(false)) )
{
}


const std::vector<std::string> OutputPort::defaultMetrics = { "isOperating" };


}
}
