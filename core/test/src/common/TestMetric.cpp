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

BOOST_AUTO_TEST_CASE(SimpleMetric)
{
  DummyMetric<int> m(1);
  MetricSnapshot ms = m.getSnapshot();
  BOOST_CHECK_EQUAL(ms.getValue(), "1");
  BOOST_CHECK_EQUAL(ms.getStatusFlag(), swatch::core::StatusFlag::kNoLimit);
}

BOOST_AUTO_TEST_CASE(MetricWithConditions)
{
  DummyMetric<int> m(1, new LessThanCondition<int>(0));
  MetricSnapshot ms = m.getSnapshot();
  BOOST_CHECK_EQUAL(ms.getValue(), "1");
  BOOST_CHECK_EQUAL(ms.getStatusFlag(), StatusFlag::kGood);
  m.setValue(-1);
  MetricSnapshot ms2 = m.getSnapshot();
  BOOST_CHECK_EQUAL(ms2.getStatusFlag(), StatusFlag::kError);
}

BOOST_AUTO_TEST_CASE(NonCriticalMetric)
{
  DummyMetric<int> m(1);
  m.setMonitoringStatus(monitoring::kNonCritical);
  MetricSnapshot ms = m.getSnapshot();
  BOOST_CHECK_EQUAL(ms.getValue(), "1");
  BOOST_CHECK_EQUAL(ms.getMonitoringStatus(), monitoring::kNonCritical);
}

BOOST_AUTO_TEST_CASE(DisabledMetric)
{
  Metric<int>* m = new Metric<int>;
  m->setMonitoringStatus(monitoring::kDisabled);
  MetricSnapshot ms = m->getSnapshot();
  BOOST_CHECK_EQUAL(ms.getMonitoringStatus(), monitoring::kDisabled);
  BOOST_CHECK_EQUAL(ms.getStatusFlag(), StatusFlag::kNoLimit);
}

BOOST_AUTO_TEST_CASE(DisabledMetricWithConditions)
{
  DummyMetric<int> m(1, new LessThanCondition<int>(0));
  m.setMonitoringStatus(monitoring::kDisabled);
  MetricSnapshot ms = m.getSnapshot();
  BOOST_CHECK_EQUAL(ms.getValue(), "1");
  BOOST_CHECK_EQUAL(ms.getMonitoringStatus(), monitoring::kDisabled);
  BOOST_CHECK_EQUAL(ms.getStatusFlag(), StatusFlag::kNoLimit);
  m.setValue(-1);
  MetricSnapshot ms2 = m.getSnapshot();
  BOOST_CHECK_EQUAL(ms2.getStatusFlag(), StatusFlag::kNoLimit);
}

BOOST_AUTO_TEST_SUITE_END() // MetricTestSuite
} /* namespace test */
} /* namespace core */
} /* namespace swatch */
