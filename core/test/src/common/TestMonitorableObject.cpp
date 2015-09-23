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
                  swatch::core::LessThanCondition<int>(0))) {
    nonCriticalIntegerMetric_.setMonitoringStatus(
        MonitoringStatus::kNonCritical);
  }

  virtual void retrieveMetricValues() {
    setMetricValue<>(nonCriticalIntegerMetric_, 2);
    setMetricValue<>(criticalIntegerMetric_, 1);
  }

  void setCriticalMetric(int v) {
    setMetricValue<>(criticalIntegerMetric_, v);
  }

  void setNonCriticalMetric(int v) {
    setMetricValue<>(nonCriticalIntegerMetric_, v);
  }

private:
  swatch::core::Metric<int>& nonCriticalIntegerMetric_;
  swatch::core::Metric<int>& criticalIntegerMetric_;

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

BOOST_AUTO_TEST_SUITE_END() // MonitorableObjectTestSuite
} /* namespace test */
} /* namespace core */
} /* namespace swatch */
