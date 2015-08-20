
#include "swatch/processor/test/DummyAlgo.hpp"


namespace swatch {
namespace processor {
namespace test {

DummyAlgo::DummyAlgo(DummyDriver& aDriver) :
  AlgoInterface(),
  driver_(aDriver)
{
}


DummyAlgo::~DummyAlgo()
{
}


void DummyAlgo::retrieveMetricValues()
{
  //TODO: extract status from driver
}

} // namespace test
} // namespace processor
} // namespace swatch
