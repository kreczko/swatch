/**
 * @file    DummyMonitorableObjects.cpp
 * @author  Luke Kreczko, Tom Williams
 * @date    December 2015
 *
 */

#include "swatch/core/test/DummyMonitorableObjects.hpp"


#include "swatch/core/MetricConditions.hpp"
#include "swatch/logger/Logger.hpp"


namespace swatch {
namespace core {
namespace test {


DummyMonitorableObject::DummyMonitorableObject() :
  MonitorableObject("DummyMonitorableObject"),
  // fail if smaller than 0
  nonCriticalIntegerMetric_( registerMetric<int>("DummyNonCriticalInteger", swatch::core::LessThanCondition<int>(0))),
  criticalIntegerMetric_( registerMetric<int>("DummyCriticalInteger", swatch::core::LessThanCondition<int>(0))),
  disabledIntegerMetric_( registerMetric<int>("DummyDisabledInteger", swatch::core::LessThanCondition<int>(0)))
{
  nonCriticalIntegerMetric_.setMonitoringStatus(monitoring::kNonCritical);
  disabledIntegerMetric_.setMonitoringStatus(monitoring::kDisabled);
}

DummyMonitorableObject::~DummyMonitorableObject()
{
}

void DummyMonitorableObject::setCriticalMetric(int v)
{
  setMetricValue<>(criticalIntegerMetric_, v);
}

void DummyMonitorableObject::setNonCriticalMetric(int v)
{
  setMetricValue<>(nonCriticalIntegerMetric_, v);
}

void DummyMonitorableObject::setDisabledMetric(int v)
{
  setMetricValue<>(disabledIntegerMetric_, v);
}

const Metric<int>& DummyMonitorableObject::getNonCriticalMetric() const
{
  return nonCriticalIntegerMetric_;
}

const Metric<int>& DummyMonitorableObject::getCriticalMetric() const
{
  return criticalIntegerMetric_;
}

const Metric<int>& DummyMonitorableObject::getDisabledMetric() const
{
  return disabledIntegerMetric_;
}

MonitorableObject& DummyMonitorableObject::addMonitorable(MonitorableObject* aObj)
{
  MonitorableObject::addMonitorable(aObj);
  return *aObj;
}
  
void DummyMonitorableObject::retrieveMetricValues()
{
  setMetricValue<>(criticalIntegerMetric_, 1);
  setMetricValue<>(nonCriticalIntegerMetric_, 2);
  //    and do nothing to the disabled metric
}




DummyMonitorableStatus::DummyMonitorableStatus() :
  mIsUpdatingMetrics(false),
  mNumberOfMetricReaders(0)
{
}

DummyMonitorableStatus::~DummyMonitorableStatus()
{
}

void DummyMonitorableStatus::throwIfWrongGuard(const MonitorableStatusGuard& aGuard) const
{
  if ( ! aGuard.isCorrectGuard(*this) )
    throw std::invalid_argument("Incorrect MonitorableStatusGuard given to DummyMonitorableStatus method");
}

bool DummyMonitorableStatus::isUpdatingMetrics(const MonitorableStatusGuard& aGuard) const
{
  throwIfWrongGuard(aGuard);
  return mIsUpdatingMetrics;
}

void DummyMonitorableStatus::waitUntilReadyToUpdateMetrics(MonitorableStatusGuard& aGuard)
{
  throwIfWrongGuard(aGuard);
  while ( mIsUpdatingMetrics || (mNumberOfMetricReaders > 0) )
  {
    getUniqueLock(aGuard).unlock();
    boost::this_thread::sleep_for(boost::chrono::microseconds(2));
    getUniqueLock(aGuard).lock();
  };
  mIsUpdatingMetrics = true;
}

void DummyMonitorableStatus::finishedUpdatingMetrics(const MonitorableStatusGuard& aGuard)
{
  throwIfWrongGuard(aGuard);
  mIsUpdatingMetrics = false;
}

void DummyMonitorableStatus::waitUntilReadyToReadMetrics(MonitorableStatusGuard& aGuard)
{
  throwIfWrongGuard(aGuard);
  while ( mIsUpdatingMetrics )
  {
    getUniqueLock(aGuard).unlock();
    boost::this_thread::sleep_for(boost::chrono::microseconds(2));
    getUniqueLock(aGuard).lock();
  };
  mNumberOfMetricReaders++;
}

void DummyMonitorableStatus::finishedReadingMetrics(const MonitorableStatusGuard& aGuard)
{
  throwIfWrongGuard(aGuard);
  mNumberOfMetricReaders--;
}


log4cplus::Logger DummyMasterMonitorableObject::sLogger = swatch::logger::Logger::getInstance("swatch.core.DummyMasterMonitorableObject");

DummyMasterMonitorableObject::DummyMasterMonitorableObject() : 
  DummyMonitorableObject()
{
  setMonitorableStatus(mStatus, sLogger);
}

DummyMasterMonitorableObject::~DummyMasterMonitorableObject()
{
}


} /* namespace test */
} /* namespace core */
} /* namespace swatch */
