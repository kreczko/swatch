/* 
 * File:   DaqTTCManager.cpp
 * Author: ale
 * 
 * Created on July 28, 2014, 12:13 AM
 */

#include "swatch/system/DaqTTCManager.hpp"

// Boost Headers
#include "boost/foreach.hpp"

// SWATCH headers
#include "swatch/core/MetricConditions.hpp"


// Namespace resolution
using namespace std;


namespace swatch {
namespace system {

DaqTTCManager::DaqTTCManager(const std::string& aId, const core::XParameterSet& aPars) : 
  swatch::core::ActionableObject(aId, aPars),
  ttcMetricClockFreq_( registerMetric<double>("clkFreq", core::InvRangeCondition<double>(39.9e6, 40.1e6)) ),
  ttcMetricBC0Counter_( registerMetric<uint32_t>("bc0Counter") ),
  ttcMetricBC0Errors_( registerMetric<uint32_t>("bc0Errors", core::GreaterThanCondition<uint32_t>(0)) ),
  ttcMetricSingleBitErrors_( registerMetric<uint32_t>("ttcSingleBitErrors", core::GreaterThanCondition<uint32_t>(0)) ),
  ttcMetricDoubleBitErrors_( registerMetric<uint32_t>("ttcDoubleBitErrors", core::GreaterThanCondition<uint32_t>(0)) )
{
}


DaqTTCManager::~DaqTTCManager() {
}


const std::vector<std::string> DaqTTCManager::defaultMetrics = { "clkFreq", "bc0Counter", "bc0Errors", "ttcSingleBitErrors", "ttcDoubleBitErrors"};


} // namespace system
} // namespace swatch

