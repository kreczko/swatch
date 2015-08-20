
#ifndef __SWATCH_PROCESSOR_TEST_DUMMYTXPORT_HPP__
#define __SWATCH_PROCESSOR_TEST_DUMMYTXPORT_HPP__


#include "swatch/processor/Port.hpp"


namespace swatch {
namespace processor {
namespace test {
    
class DummyDriver;

//! Dummy output port implementation (used for testing)
class DummyTxPort : public OutputPort {
public:
  DummyTxPort (const std::string& aId, uint32_t aNumber, DummyDriver& aDriver);
  virtual ~DummyTxPort ();

  /*
  virtual bool isEnabled() const;

  virtual bool isOperating() const;
   */
  
  virtual void retrieveMetricValues();
  
private:
  uint32_t channelID_;
  DummyDriver& driver_;
};

} // namespace test
} // namespace processor
} // namespace swatch

#endif /* SWATCH_PROCESSOR_TEST_DUMMYTXPORT_HPP */
