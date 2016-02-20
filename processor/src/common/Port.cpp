/* 
 * File:   Port.cpp
 * Author: ale
 */

#include "swatch/processor/Port.hpp"


#include "swatch/core/MetricConditions.hpp"


namespace swatch {
namespace processor {

InputPort::InputPort( const std::string& aId) : 
    MaskableObject(aId),
    mMetricIsLocked( registerMetric<bool>("isLocked", core::EqualCondition<bool>(false)) ),
    mMetricIsAligned( registerMetric<bool>("isAligned", core::EqualCondition<bool>(false)) ),
    mMetricCRCErrors( registerMetric<uint32_t>("crcErrors", core::GreaterThanCondition<uint32_t>(0)) )        
{
}


const std::vector<std::string> InputPort::kDefaultMetrics = { "isLocked", "isAligned", "crcErrors" };
  



OutputPort::OutputPort( const std::string& aId ) :
    MonitorableObject(aId),
    mMetricIsOperating( registerMetric<bool>("isOperating", core::EqualCondition<bool>(false)) )
{
}


const std::vector<std::string> OutputPort::kDefaultMetrics = { "isOperating" };


}
}
