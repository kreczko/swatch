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

using namespace swatch::logger;
namespace swatch {
namespace core {
namespace test {

template<typename DataType>
class DummyMetric: public swatch::core::Metric<DataType> {

public:
  DummyMetric(const DataType v) :
          swatch::core::Metric<DataType>() {
    setValue(v);
  }

  DummyMetric(const DataType v, MetricCondition<DataType>* aErrorCondition,
      MetricCondition<DataType>* aWarnCondition = NULL) :
          swatch::core::Metric<DataType>(aErrorCondition, aWarnCondition) {
    setValue(v);
  }

  void setDummyValue(const DataType v) {
    setValue(v);
  }
};

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
  m.setDummyValue(-1);
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

BOOST_AUTO_TEST_SUITE_END() // MetricTestSuite
} /* namespace test */
} /* namespace core */
} /* namespace swatch */
