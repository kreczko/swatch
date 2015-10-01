
#ifndef __SWATCH_SYSTEM_TEST_DUMMYAMC13DRIVER_HPP__
#define	__SWATCH_SYSTEM_TEST_DUMMYAMC13DRIVER_HPP__


// boost headers
#include "boost/date_time/posix_time/posix_time_types.hpp"


namespace swatch {
namespace system {
namespace test {

    
class DummyAMC13Driver {
public:
  
  // Forward declarations
  struct TTCStatus;
    
  DummyAMC13Driver();

  ~DummyAMC13Driver();

  TTCStatus readTTCStatus() const;

  uint16_t readFedId() const;  

  void reboot();
  
  void reset(size_t errorTime);
  
  void configureDaq(uint16_t fedId);

  void startDaq(size_t errorTime);
  
private:
  typedef boost::posix_time::ptime ptime;
    
  ptime timestampReset_;
  ptime timestampConfigureDaq_;
  ptime timestampStartDaq_;
  
  size_t errorTimeClk_;
  size_t errorTimeDaq_;
  
  uint16_t fedId_;
  
  static bool isInvalidTimeOrAfterErrorTime(const ptime& aStartTime, size_t aSecondsBeforeError);
  
public:
  struct TTCStatus {
    double clockFreq;
    uint32_t bc0Counter;
    uint32_t errCountBC0;
    uint32_t errCountSingleBit;
    uint32_t errCountDoubleBit;
  };
  
};

        
} // namespace test
} // namespace system
} // namespace swatch

#endif	/* SWATCH_PROCESSOR_TEST_DUMMYPROCESSOR_HPP */

