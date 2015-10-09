
#include "swatch/system/test/DummyAMC13Manager.hpp"


#include "swatch/core/Factory.hpp"



SWATCH_REGISTER_CLASS(swatch::system::test::DummyAMC13Manager);


namespace swatch {
namespace system {
namespace test {


DummyAMC13Manager::DummyAMC13Manager( const swatch::core::AbstractStub& aStub ) : 
  dtm::DaqTTCManager(aStub)
{
}


DummyAMC13Manager::~DummyAMC13Manager() {
}


void DummyAMC13Manager::retrieveMetricValues() {
}


} // namespace test
} // namespace system
} // namespace swatch

