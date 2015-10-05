
#ifndef __SWATCH_DUMMY_DUMMYDRIVER_HPP__
#define	__SWATCH_DUMMY_DUMMYDRIVER_HPP__


// boost headers
#include "boost/date_time/posix_time/posix_time_types.hpp"


namespace swatch {
namespace dummy {

    
class DummyProcDriver{
public:
  
  // Forward declarations
  struct TTCStatus;
  struct RxPortStatus;
    
  DummyProcDriver();

  virtual ~DummyProcDriver();

  uint64_t getFirmwareVersion() const;

  TTCStatus getTTCStatus() const;
  
  RxPortStatus getRxPortStatus(uint32_t channelId) const;

  bool isTxPortOperating(uint32_t channelId) const;
  
  void reboot();
  
  void reset(size_t errorTime);
  
  void configureTxPorts(size_t errorTime);

  void configureRxPorts(size_t errorTime);

  void configureReadout(size_t errorTime);

  void configureAlgo(size_t errorTime);
  
private:
  typedef boost::posix_time::ptime ptime;
    
  ptime timestampReset_;
  ptime timestampConfigureTx_;
  ptime timestampConfigureRx_;
  ptime timestampConfigureDaq_;
  ptime timestampConfigureAlgo_;
  
  size_t errorTimeClk_;
  size_t errorTimeDaq_;
  size_t errorTimeTx_;
  size_t errorTimeRx_;
  size_t errorTimeAlgo_;
  
  static bool isInvalidTimeOrAfterErrorTime(const ptime& aStartTime, size_t aSecondsBeforeError);
  
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
  };
  
  struct RxPortStatus {
    RxPortStatus(bool aIsLocked, bool aIsAligned, uint32_t aCrcErrCount) : 
      isLocked(aIsLocked),
      isAligned(aIsAligned),
      crcErrCount(aCrcErrCount)
    {}
    bool isLocked;
    bool isAligned;
    uint32_t crcErrCount;
  };
};


} // namespace dummy
} // namespace swatch

#endif	/* SWATCH_DUMMY_DUMMYPROCESSOR_HPP */

