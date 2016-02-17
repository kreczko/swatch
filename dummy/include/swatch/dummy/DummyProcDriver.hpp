
#ifndef __SWATCH_DUMMY_DUMMYDRIVER_HPP__
#define	__SWATCH_DUMMY_DUMMYDRIVER_HPP__


#include <stdint.h>
#include <string>

#include "swatch/dummy/ComponentState.hpp"
#include "swatch/core/TTSUtils.hpp"


namespace swatch {
namespace dummy {

    
class DummyProcDriver{
public:
  // Forward declarations
  struct TTCStatus;
  struct ReadoutStatus;
  struct RxPortStatus;
  struct TxPortStatus;
  struct AlgoStatus;

  DummyProcDriver();

  virtual ~DummyProcDriver();

  uint64_t getFirmwareVersion() const;

  TTCStatus getTTCStatus() const;

  ReadoutStatus getReadoutStatus() const;

  RxPortStatus getRxPortStatus(uint32_t channelId) const;

  TxPortStatus getTxPortStatus(uint32_t channelId) const;
  
  AlgoStatus getAlgoStatus() const;

  void reboot();
  
  void reset();

  void forceClkTtcState(ComponentState aNewState);

  void configureRxPorts();

  void forceRxPortsState(ComponentState aNewState);

  void configureTxPorts();

  void forceTxPortsState(ComponentState aNewState);

  void configureReadout();

  void forceReadoutState(ComponentState aNewState);

  void configureAlgo();

  void forceAlgoState(ComponentState aNewState);
  
private:

  ComponentState mClkState;
  ComponentState mTxState;
  ComponentState mRxState;
  ComponentState mReadoutState;
  ComponentState mAlgoState;
  
public:
  struct TTCStatus {
    uint32_t bunchCounter;
    uint32_t eventCounter;
    uint32_t orbitCounter;
    bool clk40Locked;
    bool clk40Stopped;
    bool bc0Locked;
    uint32_t errSingleBit;
    uint32_t errDoubleBit;
    bool warningSign;
  };

  struct ReadoutStatus {
      ReadoutStatus(bool aAMCCoreReady, core::tts::State aTTSState) : 
        amcCoreReady(aAMCCoreReady),
        ttsState(aTTSState)
      {}
      bool amcCoreReady;
      core::tts::State ttsState;
  };

  struct RxPortStatus {
    RxPortStatus(bool aIsLocked, bool aIsAligned, uint32_t aCrcErrCount, bool aWarningSign) : 
      isLocked(aIsLocked),
      isAligned(aIsAligned),
      crcErrCount(aCrcErrCount),
      warningSign(aWarningSign)
    {}
    bool isLocked;
    bool isAligned;
    uint32_t crcErrCount;
    bool warningSign;
  };

  struct TxPortStatus {
    TxPortStatus(bool aIsOperating, bool aWarningSign) : 
      isOperating(aIsOperating),
      warningSign(aWarningSign)
    {}
    bool isOperating;
    bool warningSign;
  };

  struct AlgoStatus {
    AlgoStatus(float aRateCounterA, float aRateCounterB) : 
      rateCounterA(aRateCounterA),
      rateCounterB(aRateCounterB)
    {}
    float rateCounterA;
    float rateCounterB;
  };
};


} // namespace dummy
} // namespace swatch

#endif	/* SWATCH_DUMMY_DUMMYPROCESSOR_HPP */

