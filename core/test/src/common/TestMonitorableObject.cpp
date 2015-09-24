/*
 * FSM_test.cpp
 *
 *  Created on: 18 Nov 2014
 *      Author: kreczko
 */
#include <boost/test/unit_test.hpp>

// swatch headers
#include "swatch/core/MonitorableObject.hpp"
#include "swatch/core/MetricConditions.hpp"
#include "swatch/logger/Log.hpp"
#include "swatch/core/test/DummyMetric.hpp"

using namespace swatch::logger;
namespace swatch {
namespace core {
namespace test {

class DummyMonitorableObject: public swatch::core::MonitorableObject {
public:
  DummyMonitorableObject() :
          swatch::core::MonitorableObject("DummyMonitorableObject"),
          // fail if smaller than 0
          nonCriticalIntegerMetric_(
              registerMetric<int>("DummyNonCriticalInteger",
                  swatch::core::LessThanCondition<int>(0))),
          criticalIntegerMetric_(
              registerMetric<int>("DummyCriticalInteger",
                  swatch::core::LessThanCondition<int>(0))),
          disabledIntegerMetric_(
              registerMetric<int>("DummyDisabledInteger",
                  swatch::core::LessThanCondition<int>(0))) {
    nonCriticalIntegerMetric_.setMonitoringStatus(
        MonitoringStatus::kNonCritical);
    disabledIntegerMetric_.setMonitoringStatus(MonitoringStatus::kDisabled);
  }

  virtual void retrieveMetricValues() {
    setMetricValue<>(criticalIntegerMetric_, 1);
    setMetricValue<>(nonCriticalIntegerMetric_, 2);
//    and do nothing to the disabled metric
  }

  void setCriticalMetric(int v) {
    setMetricValue<>(criticalIntegerMetric_, v);
  }

  void setNonCriticalMetric(int v) {
    setMetricValue<>(nonCriticalIntegerMetric_, v);
  }

  void setDisabledMetric(int v) {
    setMetricValue<>(disabledIntegerMetric_, v);
  }

private:
  swatch::core::Metric<int>& nonCriticalIntegerMetric_;
  swatch::core::Metric<int>& criticalIntegerMetric_;
  swatch::core::Metric<int>& disabledIntegerMetric_;

};

BOOST_AUTO_TEST_SUITE( MonitorableObjectTestSuite )

BOOST_AUTO_TEST_CASE(GoodMonitorableObject) {
  LOG(kInfo) << "Running MonitorableObjectTestSuite/GoodMonitorableObject";
  DummyMonitorableObject m;
  m.updateMetrics();
  MetricSnapshot ms = m.getMetric("DummyCriticalInteger").getValue();
  MetricSnapshot ms2 = m.getMetric("DummyNonCriticalInteger").getValue();
  BOOST_CHECK_EQUAL(ms.getValue(), "1");
  BOOST_CHECK_EQUAL(ms2.getValue(), "2");
  BOOST_CHECK_EQUAL(ms.getStatus(), swatch::core::StatusFlag::kGood);
  BOOST_CHECK_EQUAL(ms2.getStatus(), swatch::core::StatusFlag::kGood);
  BOOST_CHECK_EQUAL(m.getStatus(), swatch::core::StatusFlag::kGood);
}

BOOST_AUTO_TEST_CASE(CriticalFailure) {
  LOG(kInfo) << "Running MonitorableObjectTestSuite/CriticalFailure";
  DummyMonitorableObject m;
  m.updateMetrics();
  // set a value outside the range
  m.setCriticalMetric(-232);
  MetricSnapshot ms = m.getMetric("DummyCriticalInteger").getValue();
  MetricSnapshot ms2 = m.getMetric("DummyNonCriticalInteger").getValue();
  BOOST_CHECK_EQUAL(ms.getValue(), "-232");
  BOOST_CHECK_EQUAL(ms2.getValue(), "2");
  BOOST_CHECK_EQUAL(ms.getStatus(), swatch::core::StatusFlag::kError);
  BOOST_CHECK_EQUAL(ms2.getStatus(), swatch::core::StatusFlag::kGood);
  BOOST_CHECK_EQUAL(m.getStatus(), swatch::core::StatusFlag::kError);
}

BOOST_AUTO_TEST_CASE(NonCriticalFailure) {
  LOG(kInfo) << "Running MonitorableObjectTestSuite/NonCriticalFailure";
  DummyMonitorableObject m;
  m.updateMetrics();
  // set a value outside the range
  m.setNonCriticalMetric(-232);
  MetricSnapshot ms = m.getMetric("DummyCriticalInteger").getValue();
  MetricSnapshot ms2 = m.getMetric("DummyNonCriticalInteger").getValue();
  BOOST_CHECK_EQUAL(ms2.getMonitoringStatus(), MonitoringStatus::kNonCritical);
  BOOST_CHECK_EQUAL(ms.getValue(), "1");
  BOOST_CHECK_EQUAL(ms2.getValue(), "-232");
  BOOST_CHECK_EQUAL(ms.getStatus(), swatch::core::StatusFlag::kGood);
  // non-critical metric fails
  BOOST_CHECK_EQUAL(ms2.getStatus(), swatch::core::StatusFlag::kError);
  // but the MonitoringObject status should be still good
  BOOST_CHECK_EQUAL(m.getStatus(), swatch::core::StatusFlag::kGood);
}

BOOST_AUTO_TEST_CASE(DisabledFailure) {
  LOG(kInfo) << "Running MonitorableObjectTestSuite/DisabledFailure";
  DummyMonitorableObject m;
  m.updateMetrics();
  // set a value outside the range
  m.setDisabledMetric(-232);
  MetricSnapshot ms = m.getMetric("DummyDisabledInteger").getValue();
  BOOST_CHECK_EQUAL(ms.getMonitoringStatus(), MonitoringStatus::kDisabled);
  BOOST_CHECK_EQUAL(ms.getValue(), "-232");
  // nothing changes for disabled Metric
  BOOST_CHECK_EQUAL(ms.getStatus(), swatch::core::StatusFlag::kNoLimit);
  // and no effect on MonitoringObject either
  BOOST_CHECK_EQUAL(m.getStatus(), swatch::core::StatusFlag::kGood);
}

BOOST_AUTO_TEST_CASE(DisabledFailureNoUnknown) {
  LOG(kInfo) << "Running MonitorableObjectTestSuite/DisabledFailure";
  DummyMonitorableObject m;
  m.updateMetrics();
  // at this point the time of last update should be 0
  // normally MonitoringObject will attempt to set the value
  // to unknown, but that should not be the case for disabled metrics
  MetricSnapshot ms = m.getMetric("DummyDisabledInteger").getValue();
  BOOST_CHECK_EQUAL(ms.getMonitoringStatus(), MonitoringStatus::kDisabled);
  BOOST_CHECK_EQUAL(ms.getStatus(), swatch::core::StatusFlag::kNoLimit);
  BOOST_CHECK_EQUAL(m.getStatus(), swatch::core::StatusFlag::kGood);
}

BOOST_AUTO_TEST_SUITE_END() // MonitorableObjectTestSuite
} /* namespace test */
} /* namespace core */
} /* namespace swatch */
