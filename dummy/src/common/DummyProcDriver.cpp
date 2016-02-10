#include "swatch/dummy/DummyProcDriver.hpp"


#include <stdexcept>

#include "boost/lexical_cast.hpp"


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
  TTCStatus lStatus;
  lStatus.bunchCounter = 0x00001234;
  lStatus.eventCounter = 0xdeadbeef;
  lStatus.orbitCounter = 0x0000cafe;

  lStatus.clk40Stopped = (mClkState == ComponentState::kError);
  lStatus.clk40Locked = (mClkState != ComponentState::kError);
  lStatus.bc0Locked = (mClkState != ComponentState::kError);
  lStatus.errSingleBit = (mClkState == ComponentState::kError) ? 42 : 0;
  lStatus.errDoubleBit = (mClkState == ComponentState::kError) ? 4 : 0;

  lStatus.warningSign = (mClkState != ComponentState::kGood);

  // Hardware unreachable : Driver usually throws
  if (mClkState == ComponentState::kNotReachable)
    throw std::runtime_error("Problem communicating with board (TTC block).");

  return lStatus;
}


DummyProcDriver::ReadoutStatus DummyProcDriver::getReadoutStatus() const
{
  switch (mReadoutState) {
    case ComponentState::kGood : 
      return ReadoutStatus(true, 8);
    case ComponentState::kWarning : 
      return ReadoutStatus(true, 5);
    case ComponentState::kError :
      return ReadoutStatus(false, 2);
    // Not reachable = throw
    case ComponentState::kNotReachable : 
      break;
  }
  throw std::runtime_error("Problem communicating with board (readout block).");
}


DummyProcDriver::RxPortStatus DummyProcDriver::getRxPortStatus(uint32_t aChannelId) const 
{
  switch (mRxState) {
    case ComponentState::kGood : 
    case ComponentState::kWarning : 
      return RxPortStatus(true, true, 0, mRxState == ComponentState::kWarning);
    case ComponentState::kError : 
      return RxPortStatus(false, false, 42, true);
    case ComponentState::kNotReachable : 
      break;
  }
  throw std::runtime_error("Problem communicating with board (rx port " + boost::lexical_cast<std::string>(aChannelId) + ").");
}


DummyProcDriver::TxPortStatus DummyProcDriver::getTxPortStatus(uint32_t aChannelId) const
{
  switch (mTxState) {
    case ComponentState::kGood : 
      return TxPortStatus(true, false);
    case ComponentState::kWarning : 
      return TxPortStatus(true, true);
    case ComponentState::kError : 
      return TxPortStatus(false, true);
    case ComponentState::kNotReachable : 
      break;
  }
  throw std::runtime_error("Problem communicating with board (tx port " + boost::lexical_cast<std::string>(aChannelId) + ").");
}


DummyProcDriver::AlgoStatus DummyProcDriver::getAlgoStatus() const
{
  switch (mAlgoState) {
    // All good = rates below 40kHz
    case ComponentState::kGood : 
      return AlgoStatus(12e3, 20e3);
    // Warning = rates between 40 and 80 kHz
    case ComponentState::kWarning : 
      return AlgoStatus(52e3, 54e3);
    // Error = rates above 80 kHz
    case ComponentState::kError : 
      return AlgoStatus(85e3, 120e3);
    // Not reachable = throw
    case ComponentState::kNotReachable :
      break;
  }
  throw std::runtime_error("Problem communicating with board (algo block).");
}


void DummyProcDriver::reboot()
{
  mClkState = kError;
  mTxState = kError;
  mRxState = kError;
  mReadoutState = kError;
  mAlgoState = kError;
}


void DummyProcDriver::reset()
{
  mClkState = kGood;
  
  mTxState = kError;
  mRxState = kError;
  mReadoutState = kError;
}


void DummyProcDriver::forceClkTtcState(ComponentState aNewState)
{
  mClkState = aNewState;
}


void DummyProcDriver::configureRxPorts()
{
  if (mClkState == kError) {
    mRxState = kError;
    throw std::runtime_error("Couldn't configure rx ports - no clock!");
  }
  else
    mRxState = kGood;
}


void DummyProcDriver::forceRxPortsState(ComponentState aNewState)
{
  mRxState = aNewState;
}


void DummyProcDriver::configureTxPorts()
{
  if (mClkState == kError) {
    mTxState = kError;
    throw std::runtime_error("Couldn't configure tx ports - no clock!");
  }
  else
    mTxState = kGood;
}


void DummyProcDriver::forceTxPortsState(ComponentState aNewState)
{
  mTxState = aNewState;
}


void DummyProcDriver::configureReadout()
{
  if (mClkState == kError) {
    mReadoutState = kError;
    throw std::runtime_error("Couldn't configure readout block - no clock!");
  }
  else
    mReadoutState = kGood;
}


void DummyProcDriver::forceReadoutState(ComponentState aNewState)
{
  mReadoutState = aNewState;
}


void DummyProcDriver::configureAlgo()
{
  if (mClkState == kError) {
    mReadoutState = kError;
    throw std::runtime_error("Couldn't configure algo - no clock!");
  }
  else
    mAlgoState = kGood;
}


void DummyProcDriver::forceAlgoState(ComponentState aNewState)
{
  mAlgoState = aNewState;
}


}
}
