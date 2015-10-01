/* 
 * File:   DummyAMC13Service.cpp
 * Author: ale
 * 
 * Created on August 3, 2014, 9:18 PM
 */

#include "swatch/system/test/DummyAMC13Manager.hpp"

// boost headers
#include "boost/foreach.hpp"

// SWATCH headers
#include "swatch/logger/Log.hpp"
#include "swatch/core/Factory.hpp"
#include "swatch/system/DaqTTCStub.hpp"
#include "swatch/system/test/DummyAMC13Driver.hpp"
#include "swatch/system/test/DummyAMC13ManagerCommands.hpp"


SWATCH_REGISTER_CLASS(swatch::system::test::DummyAMC13Manager);


namespace swatch {
namespace system {
namespace test {


DummyAMC13Manager::DummyAMC13Manager( const swatch::core::AbstractStub& aStub ) : 
  system::DaqTTCManager(aStub),
  driver_(new DummyAMC13Driver())
{
  // 1) Commands
  registerFunctionoid<DummyAMC13RebootCommand>("reboot");
  registerFunctionoid<DummyAMC13ResetCommand>("reset");
  registerFunctionoid<DummyAMC13ConfigureDaqCommand>("configureDaq");
  registerFunctionoid<DummyAMC13EnableDaqCommand>("enableDaq");
  
  // 2) Command sequences
  //registerFunctionoid<DaqTTCMgrCommandSequence>("resetAndConfigure").run(reset).then(configureDaq);
}


DummyAMC13Manager::~DummyAMC13Manager() {
}




void DummyAMC13Manager::retrieveMetricValues() {
  DummyAMC13Driver::TTCStatus s = driver_->readTTCStatus();
  
  setMetricValue<double>(ttcMetricClockFreq_, s.clockFreq);
  setMetricValue<uint32_t>(ttcMetricBC0Counter_, s.bc0Counter);
  setMetricValue<uint32_t>(ttcMetricBC0Errors_, s.errCountBC0);
  setMetricValue<uint32_t>(ttcMetricSingleBitErrors_, s.errCountSingleBit);
  setMetricValue<uint32_t>(ttcMetricDoubleBitErrors_, s.errCountDoubleBit);
  
  setMetricValue<uint16_t>(daqMetricFedId_, driver_->readFedId());
}


} // namespace test
} // namespace system
} // namespace swatch
