/**
 * @file    DummyMonitorableObjects.hpp
 * @author  Luke Kreczko, Tom Williams
 * @date    December 2015
 *
 */

#ifndef SWATCH_CORE_TEST_DUMMYMONITORABLEOBJECTS_HPP
#define SWATCH_CORE_TEST_DUMMYMONITORABLEOBJECTS_HPP


#include "swatch/core/AbstractMonitorableStatus.hpp"
#include "swatch/core/MonitorableObject.hpp"


namespace swatch {
namespace core {
namespace test {


class DummyMonitorableObject: public MonitorableObject {
public:
  DummyMonitorableObject();
  virtual ~DummyMonitorableObject();

  void setCriticalMetric(int v);

  void setNonCriticalMetric(int v);

  void setDisabledMetric(int v);

  const Metric<int>& getNonCriticalMetric() const;

  const Metric<int>& getCriticalMetric() const;

  const Metric<int>& getDisabledMetric() const;

  // Expose as public for unit tests
  MonitorableObject& addMonitorable(MonitorableObject* aObj);

private:
  virtual void retrieveMetricValues();

  swatch::core::Metric<int>& nonCriticalIntegerMetric_;
  swatch::core::Metric<int>& criticalIntegerMetric_;
  swatch::core::Metric<int>& disabledIntegerMetric_;
};


class DummyMonitorableStatus : public AbstractMonitorableStatus {
public:
  DummyMonitorableStatus();
  ~DummyMonitorableStatus();

  void throwIfWrongGuard(const MonitorableStatusGuard& aGuard) const;

  bool isUpdatingMetrics(const MonitorableStatusGuard& aGuard) const;
  void waitUntilReadyToUpdateMetrics(MonitorableStatusGuard& aGuard);
  void finishedUpdatingMetrics(const MonitorableStatusGuard& aGuard);

  void waitUntilReadyToReadMetrics(MonitorableStatusGuard& aGuard);
  void finishedReadingMetrics(const MonitorableStatusGuard& aGuard);

private:
  bool mIsUpdatingMetrics;
  size_t mNumberOfMetricReaders;
};


class DummyMasterMonitorableObject : public DummyMonitorableObject {
public:
  DummyMasterMonitorableObject();
  
  ~DummyMasterMonitorableObject();

private:
  DummyMonitorableStatus mStatus;
  
  static log4cplus::Logger sLogger;
};


} /* namespace test */
} /* namespace core */
} /* namespace swatch */

#endif /* SWATCH_CORE_TEST_DUMMYMONITORABLEOBJECTS_HPP */
