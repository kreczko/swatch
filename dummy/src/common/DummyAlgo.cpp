
#include "swatch/dummy/DummyAlgo.hpp"


namespace swatch {
namespace dummy {


DummyAlgo::DummyAlgo(DummyProcDriver& aDriver) :
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

} // namespace dummy
} // namespace swatch
