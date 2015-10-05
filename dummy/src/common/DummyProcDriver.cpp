#include "swatch/dummy/DummyProcDriver.hpp"

namespace swatch {
namespace dummy {


DummyProcDriver::DummyProcDriver() 
{
  reboot();
}


DummyProcDriver::~DummyProcDriver()
{
}


uint64_t DummyProcDriver::getFirmwareVersion() const
{
  return 0xdeadbeef00001234;
}


DummyProcDriver::TTCStatus DummyProcDriver::getTTCStatus() const 
{
  bool allOK = ! isInvalidTimeOrAfterErrorTime(timestampReset_, errorTimeClk_);
  
  TTCStatus s;
  s.bunchCounter = allOK ? 0x00001234 : 0;
  s.eventCounter = allOK ? 0xdeadbeef : 0;
  s.orbitCounter = allOK ? 0x0000cafe : 0;
  s.clk40Stopped = ! allOK;
  s.clk40Locked = allOK;
  s.bc0Locked = allOK;
  s.errSingleBit = allOK ? 0 : 42;
  s.errDoubleBit = allOK ? 0 : 4;

  return s;  
}


DummyProcDriver::RxPortStatus DummyProcDriver::getRxPortStatus(uint32_t channelId) const 
{
  if ( isInvalidTimeOrAfterErrorTime(timestampReset_,errorTimeClk_) || isInvalidTimeOrAfterErrorTime(timestampConfigureRx_,errorTimeRx_) )
    return RxPortStatus(false, false, 42);
  else
    return RxPortStatus(true, true, 0);
}


bool DummyProcDriver::isTxPortOperating(uint32_t channelId) const
{
  if ( isInvalidTimeOrAfterErrorTime(timestampReset_,errorTimeClk_) )
    return false;
  else if ( isInvalidTimeOrAfterErrorTime(timestampConfigureTx_, errorTimeTx_) )
    return false;
  else
    return true;
}


void DummyProcDriver::reboot()
{
  timestampReset_ = ptime();
  timestampConfigureTx_ = ptime();
  timestampConfigureRx_ = ptime();
  timestampConfigureDaq_ = ptime();
  timestampConfigureAlgo_ = ptime();
}


void DummyProcDriver::reset(size_t errorTime)
{
  timestampReset_ = boost::posix_time::microsec_clock::universal_time();
  errorTimeClk_ = errorTime;

  timestampConfigureTx_ = ptime();
  timestampConfigureRx_ = ptime();
  timestampConfigureDaq_ = ptime();
}


void DummyProcDriver::configureTxPorts(size_t errorTime)
{
  if (timestampReset_.is_not_a_date_time())
    throw std::runtime_error("Couldn't configure tx ports - no clock!");
  else {
    timestampConfigureTx_ = boost::posix_time::microsec_clock::universal_time();
    errorTimeTx_ = errorTime;
  }
}


void DummyProcDriver::configureRxPorts(size_t errorTime)
{
  if (timestampReset_.is_not_a_date_time())
    throw std::runtime_error("Couldn't configure rx ports - no clock!");
  else {
    timestampConfigureRx_ = boost::posix_time::microsec_clock::universal_time();
    errorTimeRx_ = errorTime;
  }
}


void DummyProcDriver::configureReadout(size_t errorTime)
{
  if (timestampReset_.is_not_a_date_time())
    throw std::runtime_error("Couldn't configure readout block - no clock!");
  else {
    timestampConfigureDaq_ = boost::posix_time::microsec_clock::universal_time();
    errorTimeDaq_ = errorTime;
  }
}


void DummyProcDriver::configureAlgo(size_t errorTime)
{
  if (timestampReset_.is_not_a_date_time())
    throw std::runtime_error("Couldn't configure algo - no clock!");
  else {
    timestampConfigureAlgo_ = boost::posix_time::microsec_clock::universal_time();
    errorTimeAlgo_ = errorTime;
  }
}


bool DummyProcDriver::isInvalidTimeOrAfterErrorTime(const ptime& aStartTime, size_t aSecondsBeforeError)
{
  if ( aStartTime.is_not_a_date_time() )
    return true;
  else{
    ptime currentTime = boost::posix_time::microsec_clock::universal_time();
    ptime errorTime = aStartTime + boost::posix_time::seconds(aSecondsBeforeError);
    return ( currentTime > errorTime );
  }
}


}
}
