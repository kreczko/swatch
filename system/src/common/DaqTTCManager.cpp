/* 
 * File:   DaqTTCManager.cpp
 * Author: ale
 * 
 * Created on July 28, 2014, 12:13 AM
 */

#include "swatch/system/DaqTTCManager.hpp"


// SWATCH headers
#include "swatch/core/MetricConditions.hpp"


namespace swatch {
namespace system {


DaqTTCManager::DaqTTCManager(const swatch::core::AbstractStub& aStub ) : 
  swatch::core::ActionableObject(aStub.id),
  stub_(dynamic_cast<const DaqTTCStub&>(aStub)),
  ttcMetricClockFreq_( registerMetric<double>("clkFreq", core::InvRangeCondition<double>(39.9e6, 40.1e6)) ),
  ttcMetricBC0Counter_( registerMetric<uint32_t>("bc0Counter") ),
  ttcMetricBC0Errors_( registerMetric<uint32_t>("bc0Errors", core::GreaterThanCondition<uint32_t>(0)) ),
  ttcMetricSingleBitErrors_( registerMetric<uint32_t>("ttcSingleBitErrors", core::GreaterThanCondition<uint32_t>(0)) ),
  ttcMetricDoubleBitErrors_( registerMetric<uint32_t>("ttcDoubleBitErrors", core::GreaterThanCondition<uint32_t>(0)) ),
  daqMetricFedId_( registerMetric<uint16_t>("fedId", core::NotEqualCondition<uint16_t>(stub_.fedId)) ) {
}


DaqTTCManager::~DaqTTCManager() {
}


const DaqTTCStub& DaqTTCManager::getStub() const {
  return stub_;
}


uint32_t DaqTTCManager::getSlot() const {
  return stub_.slot; 
}


const std::string& DaqTTCManager::getCrateId() const {
  return stub_.crate;
}


uint16_t DaqTTCManager::getFedId() const {
  return stub_.fedId;
}


const std::vector<std::string> DaqTTCManager::defaultMetrics = {"clkFreq", "bc0Counter", "bc0Errors", "ttcSingleBitErrors", "ttcDoubleBitErrors", "fedId"};

} // namespace system
} // namespace swatch

