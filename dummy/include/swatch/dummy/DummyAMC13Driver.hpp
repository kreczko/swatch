
#ifndef __SWATCH_DUMMY_DUMMYAMC13DRIVER_HPP__
#define	__SWATCH_DUMMY_DUMMYAMC13DRIVER_HPP__


// boost headers
#include "boost/date_time/posix_time/posix_time_types.hpp"


namespace swatch {
namespace dummy {

 
class DummyAMC13Driver {
public:
  
  // Forward declarations
  struct TTCStatus;
    
  DummyAMC13Driver();

  ~DummyAMC13Driver();

  TTCStatus readTTCStatus() const;

  uint16_t readFedId() const;  

  void reboot();
  
  void reset(size_t aWarnAfter, size_t aErrorAfter);
  
  void configureDaq(uint16_t fedId);

  void startDaq(size_t aWarnAfter, size_t aErrorAfter);
  
private:
  typedef boost::posix_time::ptime ptime;
  
  ptime mErrTimeReset;
  ptime mErrTimeDaq;
  ptime mWrnTimeReset;
  ptime mWrnTimeDaq;

  uint16_t mFedId;
  
public:
  struct TTCStatus {
    double clockFreq;
    uint32_t bc0Counter;
    uint32_t errCountBC0;
    uint32_t errCountSingleBit;
    uint32_t errCountDoubleBit;
  };
  
};


} // namespace dummy
} // namespace swatch

#endif	/* SWATCH_DUMMY_DUMMYAMC13DRIVER_HPP */

