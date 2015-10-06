
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
  
  void reset(size_t aErrorAfter);
  
  void configureTxPorts(size_t aErrorAfter);

  void configureRxPorts(size_t aErrorAfter);

  void configureReadout(size_t aErrorAfter);

  void configureAlgo(size_t aErrorAfter);
  
private:
  typedef boost::posix_time::ptime ptime;
    
  ptime errTimeClk_;
  ptime errTimeTx_;
  ptime errTimeRx_;
  ptime errTimeDaq_;
  ptime errTimeAlgo_;
  
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

