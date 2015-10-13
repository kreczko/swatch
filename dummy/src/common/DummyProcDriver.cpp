#include "swatch/dummy/DummyProcDriver.hpp"

namespace swatch {
namespace dummy {

typedef boost::posix_time::microsec_clock ms_clk;

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
  bool allOK = (ms_clk::universal_time() < errTimeClk_);
  
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
  if ( (ms_clk::universal_time() >= errTimeClk_) || (ms_clk::universal_time() >= errTimeRx_) )
    return RxPortStatus(false, false, 42);
  else
    return RxPortStatus(true, true, 0);
}


bool DummyProcDriver::isTxPortOperating(uint32_t channelId) const
{
  if ( (ms_clk::universal_time() >= errTimeClk_) || (ms_clk::universal_time() >= errTimeTx_) )
    return false;
  else
    return true;
}


void DummyProcDriver::reboot()
{
  errTimeClk_  = ptime( boost::posix_time::min_date_time );
  errTimeTx_   = ptime( boost::posix_time::min_date_time );
  errTimeRx_   = ptime( boost::posix_time::min_date_time );
  errTimeDaq_  = ptime( boost::posix_time::min_date_time );
  errTimeAlgo_ = ptime( boost::posix_time::min_date_time );
}


void DummyProcDriver::reset(size_t aErrorAfter)
{
  errTimeClk_ = ms_clk::universal_time() + boost::posix_time::seconds(aErrorAfter);

  errTimeTx_ = ptime( boost::posix_time::min_date_time );
  errTimeRx_ = ptime( boost::posix_time::min_date_time );
  errTimeDaq_ = ptime( boost::posix_time::min_date_time );
}


void DummyProcDriver::configureTxPorts(size_t aErrorAfter)
{
  if ( ms_clk::universal_time() >= errTimeClk_ )
    throw std::runtime_error("Couldn't configure tx ports - no clock!");
  else
    errTimeTx_ = ms_clk::universal_time() + boost::posix_time::seconds(aErrorAfter);
}


void DummyProcDriver::configureRxPorts(size_t aErrorAfter)
{
  if ( ms_clk::universal_time() >= errTimeClk_ )
    throw std::runtime_error("Couldn't configure rx ports - no clock!");
  else
    errTimeRx_ = ms_clk::universal_time() + boost::posix_time::seconds(aErrorAfter);
}


void DummyProcDriver::configureReadout(size_t aErrorAfter)
{
  if ( ms_clk::universal_time() >= errTimeClk_ )
    throw std::runtime_error("Couldn't configure readout block - no clock!");
  else
    errTimeDaq_ = ms_clk::universal_time() + boost::posix_time::seconds(aErrorAfter);
}


void DummyProcDriver::configureAlgo(size_t aErrorAfter)
{
  if ( ms_clk::universal_time() >= errTimeClk_ )
    throw std::runtime_error("Couldn't configure algo - no clock!");
  else
    errTimeAlgo_ = ms_clk::universal_time() + boost::posix_time::seconds(aErrorAfter);
}


}
}