/*
 * FSM_test.cpp
 *
 *  Created on: 18 Nov 2014
 *      Author: kreczko
 */
#include <boost/test/unit_test.hpp>

// swatch headers
#include "swatch/core/Metric.hpp"
#include "swatch/core/MetricConditions.hpp"
#include "swatch/logger/Log.hpp"
#include "swatch/core/test/DummyMetric.hpp"

using namespace swatch::logger;
namespace swatch {
namespace core {
namespace test {
BOOST_AUTO_TEST_SUITE( MetricTestSuite )

BOOST_AUTO_TEST_CASE(SimpleMetric) {
  LOG(kInfo) << "Running MetricTestSuite/SimpleMetric";
  DummyMetric<int> m(1);
  MetricSnapshot ms = m.getValue();
  BOOST_CHECK_EQUAL(ms.getValue(), "1");
  BOOST_CHECK_EQUAL(ms.getStatus(), swatch::core::StatusFlag::kNoLimit);
}

BOOST_AUTO_TEST_CASE(MetricWithConditions) {
  LOG(kInfo) << "Running MetricTestSuite/MetricWithConditions";
  DummyMetric<int> m(1, new LessThanCondition<int>(0));
  MetricSnapshot ms = m.getValue();
  BOOST_CHECK_EQUAL(ms.getValue(), "1");
  BOOST_CHECK_EQUAL(ms.getStatus(), StatusFlag::kGood);
  m.setValue(-1);
  MetricSnapshot ms2 = m.getValue();
  BOOST_CHECK_EQUAL(ms2.getStatus(), StatusFlag::kError);
}

BOOST_AUTO_TEST_CASE(NonCriticalMetric) {
  LOG(kInfo) << "Running MetricTestSuite/NonCriticalMetric";
  DummyMetric<int> m(1);
  m.setMonitoringStatus(MonitoringStatus::kNonCritical);
  MetricSnapshot ms = m.getValue();
  BOOST_CHECK_EQUAL(ms.getValue(), "1");
  BOOST_CHECK_EQUAL(ms.getMonitoringStatus(), MonitoringStatus::kNonCritical);
}

BOOST_AUTO_TEST_CASE(DisabledMetric) {
  LOG(kInfo) << "Running MetricTestSuite/DisabledMetric";
  Metric<int>* m = new Metric<int>;
  m->setMonitoringStatus(MonitoringStatus::kDisabled);
  MetricSnapshot ms = m->getValue();
  BOOST_CHECK_EQUAL(ms.getMonitoringStatus(), MonitoringStatus::kDisabled);
  BOOST_CHECK_EQUAL(ms.getStatus(), StatusFlag::kNoLimit);
}

BOOST_AUTO_TEST_CASE(DisabledMetricWithConditions) {
  LOG(kInfo) << "Running MetricTestSuite/DisabledMetricWithConditions";
  DummyMetric<int> m(1, new LessThanCondition<int>(0));
  m.setMonitoringStatus(MonitoringStatus::kDisabled);
  MetricSnapshot ms = m.getValue();
  BOOST_CHECK_EQUAL(ms.getValue(), "1");
  BOOST_CHECK_EQUAL(ms.getMonitoringStatus(), MonitoringStatus::kDisabled);
  BOOST_CHECK_EQUAL(ms.getStatus(), StatusFlag::kNoLimit);
  m.setValue(-1);
  MetricSnapshot ms2 = m.getValue();
  BOOST_CHECK_EQUAL(ms2.getStatus(), StatusFlag::kNoLimit);
}

BOOST_AUTO_TEST_SUITE_END() // MetricTestSuite
} /* namespace test */
} /* namespace core */
} /* namespace swatch */
