
#include "swatch/dummy/DummyAMC13Driver.hpp"


typedef boost::posix_time::microsec_clock ms_clk;


namespace swatch {
namespace dummy {

  
DummyAMC13Driver::DummyAMC13Driver()
{
  reboot();
} 


DummyAMC13Driver::~DummyAMC13Driver()
{
}


DummyAMC13Driver::TTCStatus DummyAMC13Driver::readTTCStatus() const
{
  bool allOK = (ms_clk::universal_time() < errTimeReset_);
  
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
  errTimeReset_ = ptime( boost::posix_time::min_date_time );
  errTimeDaq_ = ptime( boost::posix_time::min_date_time );
  
  fedId_ = 0;
}


void DummyAMC13Driver::reset(size_t aWarnAfter, size_t aErrorAfter)
{
  ptime lNow = ms_clk::universal_time();
  wrnTimeReset_ = lNow + boost::posix_time::seconds(aWarnAfter);
  errTimeReset_ = lNow + boost::posix_time::seconds(aErrorAfter);

  errTimeDaq_ = ptime( boost::posix_time::min_date_time );
  wrnTimeDaq_ = ptime( boost::posix_time::min_date_time );
  fedId_ = 0;
}


void DummyAMC13Driver::configureDaq(uint16_t fedId)
{
  if (errTimeReset_ <= ms_clk::universal_time())
    throw std::runtime_error("Couldn't configure daq - no clock!");
  else {
    errTimeDaq_ = ptime( boost::posix_time::max_date_time );
    fedId_ = fedId;
  }
}


void DummyAMC13Driver::startDaq(size_t aWarnAfter, size_t aErrorAfter)
{
  if (errTimeReset_ <= ms_clk::universal_time() )
    throw std::runtime_error("Couldn't enable daq - no clock!");
  else if ( errTimeDaq_ <= ms_clk::universal_time() )
    throw std::runtime_error("Couldn't enable daq - my daq block isn't configured!");
  else {
    ptime lNow = ms_clk::universal_time();
    wrnTimeDaq_ = lNow + boost::posix_time::seconds(aWarnAfter);
    errTimeDaq_ = lNow + boost::posix_time::seconds(aErrorAfter);
  }
}


} // namespace dummy
} // namespace swatch
