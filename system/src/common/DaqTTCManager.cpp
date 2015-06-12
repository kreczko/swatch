/* 
 * File:   DaqTTCManager.cpp
 * Author: ale
 * 
 * Created on July 28, 2014, 12:13 AM
 */

#include "swatch/system/DaqTTCManager.hpp"

// Boost Headers
#include "boost/foreach.hpp"

// Namespace resolution
using namespace std;

namespace swatch {
namespace system {

DaqTTCManager::DaqTTCManager(const std::string& aId, const core::XParameterSet& aPars) : 
  swatch::core::ActionableObject(aId, aPars),
  ttcMetricClockFreq_(registerMetric<double>("clkFreq", 39.9e6, 40.1e6)),
  ttcMetricBC0Counter_(registerMetric<uint32_t>("bc0Counter", 0, 0xFFFFFFFF)),
  ttcMetricBC0Errors_(registerMetric<uint32_t>("bc0Errors", 0, 0)),
  ttcMetricSingleBitErrors_(registerMetric<uint32_t>("ttcSingleBitErrors", 0, 0)),
  ttcMetricDoubleBitErrors_(registerMetric<uint32_t>("ttcDoubleBitErrors", 0, 0))
{
}


DaqTTCManager::~DaqTTCManager() {
}


std::vector<std::string> DaqTTCManager::getDefaultMetrics() {
  std::vector<std::string> metrics;
  metrics.push_back("clkFreq");
  metrics.push_back("bc0Counter");
  metrics.push_back("bc0Errors");
  metrics.push_back("ttcSingleBitErrors");
  metrics.push_back("ttcDoubleBitErrors");
  return metrics;
}


} // namespace system
} // namespace swatch

