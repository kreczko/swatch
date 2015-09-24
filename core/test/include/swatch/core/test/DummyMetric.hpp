/**
 * @file    DummyMetric.hpp
 * @author  Luke Kreczko
 * @brief   Dummy class for Metrics
 * @date    24.09.2015
 */

#ifndef SWATCH_CORE_TEST_DUMMYMETRIC_HPP
#define	SWATCH_CORE_TEST_DUMMYMETRIC_HPP

#include "swatch/core/Metric.hpp"
#include "swatch/logger/Log.hpp"
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

  DummyMetric(MetricCondition<DataType>* aErrorCondition,
      MetricCondition<DataType>* aWarnCondition = NULL) :
          swatch::core::Metric<DataType>(aErrorCondition, aWarnCondition) {
  }

  void setValue(const DataType& value) {
    Metric<DataType>::setValue(value);
  }

};

} // namespace swatch
} // namespace core
} // namespace test

#endif	/* SWATCH_CORE_TEST_DUMMYMETRIC_HPP */

