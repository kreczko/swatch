
#include "swatch/dummy/DummyAlgo.hpp"
#include "swatch/dummy/DummyProcDriver.hpp"
#include "swatch/core/MetricConditions.hpp"


namespace swatch {
namespace dummy {


DummyAlgo::DummyAlgo(DummyProcDriver& aDriver) :
  AlgoInterface(),
  mDriver(aDriver),
  mRateCounterA(registerMetric<float>("rateCounterA", core::GreaterThanCondition<float>(80e3), core::GreaterThanCondition<float>(40e3))),
  mRateCounterB(registerMetric<float>("rateCounterB", core::GreaterThanCondition<float>(80e3), core::GreaterThanCondition<float>(40e3)))
{
}


DummyAlgo::~DummyAlgo()
{
}


void DummyAlgo::retrieveMetricValues()
{
  DummyProcDriver::AlgoStatus lStatus = mDriver.getAlgoStatus();

  setMetricValue(mRateCounterA, lStatus.rateCounterA);
  setMetricValue(mRateCounterB, lStatus.rateCounterB);
}

} // namespace dummy
} // namespace swatch
