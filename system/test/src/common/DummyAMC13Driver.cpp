
#include "swatch/system/test/DummyAMC13Driver.hpp"


namespace swatch {
namespace system {
namespace test {

  
DummyAMC13Driver::DummyAMC13Driver()
{
  reboot();
} 


DummyAMC13Driver::~DummyAMC13Driver()
{
}


DummyAMC13Driver::TTCStatus DummyAMC13Driver::readTTCStatus() const
{
  bool allOK = ! isInvalidTimeOrAfterErrorTime(timestampReset_, errorTimeClk_);
  
  TTCStatus s;
  s.clockFreq = allOK ? 40.0e6 : 15.0e6;
  s.bc0Counter = 42;
  s.errCountBC0 = allOK ? 0 : 5;
  s.errCountSingleBit = allOK ? 0 : 100;
  s.errCountDoubleBit = allOK ? 0 : 10;
  
  return s;
}


uint16_t DummyAMC13Driver::readFedId() const
{
  return fedId_;
}


void DummyAMC13Driver::reboot()
{
  timestampReset_ = ptime();
  timestampConfigureDaq_ = ptime();
  timestampEnableDaq_ = ptime();
  
  fedId_ = 0;
}


void DummyAMC13Driver::reset(size_t errorTime)
{
  timestampReset_ = boost::posix_time::microsec_clock::universal_time();
  errorTimeClk_ = errorTime;

  timestampConfigureDaq_ = ptime();
  timestampEnableDaq_ = ptime();
  fedId_ = 0;
}


void DummyAMC13Driver::configureDaq(uint16_t fedId)
{
  if (timestampReset_.is_not_a_date_time())
    throw std::runtime_error("Couldn't configure daq - no clock!");
  else {
    timestampConfigureDaq_ = boost::posix_time::microsec_clock::universal_time();
    fedId_ = fedId;
  }
}


void DummyAMC13Driver::enableDaq(size_t errorTime)
{
  if (timestampReset_.is_not_a_date_time() )
    throw std::runtime_error("Couldn't enable daq - no clock!");
  else if (timestampConfigureDaq_.is_not_a_date_time())
    throw std::runtime_error("Couldn't enable daq - my daq block isn't configured!");
  else {
    timestampEnableDaq_ = boost::posix_time::microsec_clock::universal_time();
    errorTimeDaq_ = errorTime;
  }
}


bool DummyAMC13Driver::isInvalidTimeOrAfterErrorTime(const ptime& aStartTime, size_t aSecondsBeforeError)
{
  if ( aStartTime.is_not_a_date_time() )
    return true;
  else{
    ptime currentTime = boost::posix_time::microsec_clock::universal_time();
    ptime errorTime = aStartTime + boost::posix_time::seconds(aSecondsBeforeError);
    return ( currentTime > errorTime );
  }
}


} // namespace test
} // namespace system
} // namespace swatch
