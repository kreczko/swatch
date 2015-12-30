
#include "swatch/dummy/DummyAMC13Driver.hpp"


typedef boost::posix_time::microsec_clock MicrosecClk_t;


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
  bool allOK = (MicrosecClk_t::universal_time() < mErrTimeReset);
  
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
  return mFedId;
}


void DummyAMC13Driver::reboot()
{
  mErrTimeReset = ptime( boost::posix_time::min_date_time );
  mErrTimeDaq = ptime( boost::posix_time::min_date_time );
  
  mFedId = 0;
}


void DummyAMC13Driver::reset(size_t aWarnAfter, size_t aErrorAfter)
{
  ptime lNow = MicrosecClk_t::universal_time();
  mWrnTimeReset = lNow + boost::posix_time::seconds(aWarnAfter);
  mErrTimeReset = lNow + boost::posix_time::seconds(aErrorAfter);

  mErrTimeDaq = ptime( boost::posix_time::min_date_time );
  mWrnTimeDaq = ptime( boost::posix_time::min_date_time );
  mFedId = 0;
}


void DummyAMC13Driver::configureDaq(uint16_t fedId)
{
  if (mErrTimeReset <= MicrosecClk_t::universal_time())
    throw std::runtime_error("Couldn't configure daq - no clock!");
  else {
    mErrTimeDaq = ptime( boost::posix_time::max_date_time );
    mFedId = fedId;
  }
}


void DummyAMC13Driver::startDaq(size_t aWarnAfter, size_t aErrorAfter)
{
  if (mErrTimeReset <= MicrosecClk_t::universal_time() )
    throw std::runtime_error("Couldn't enable daq - no clock!");
  else if ( mErrTimeDaq <= MicrosecClk_t::universal_time() )
    throw std::runtime_error("Couldn't enable daq - my daq block isn't configured!");
  else {
    ptime lNow = MicrosecClk_t::universal_time();
    mWrnTimeDaq = lNow + boost::posix_time::seconds(aWarnAfter);
    mErrTimeDaq = lNow + boost::posix_time::seconds(aErrorAfter);
  }
}


} // namespace dummy
} // namespace swatch
