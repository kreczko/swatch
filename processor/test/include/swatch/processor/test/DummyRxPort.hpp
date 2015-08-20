/*
 * DymmyRxPort.hpp
 */

#ifndef __SWATCH_PROCESSOR_TEST_DUMMYRXPORT_HPP__
#define __SWATCH_PROCESSOR_TEST_DUMMYRXPORT_HPP__


#include <string>

#include "swatch/processor/Port.hpp"


namespace swatch {
namespace processor {
namespace test {

class DummyDriver;

//! Dummy input port implementation (used for testing)
class DummyRxPort : public InputPort {
public:
  DummyRxPort(const std::string& aId, uint32_t aNumber, DummyDriver& aDriver);

  virtual ~DummyRxPort();

  /*
  virtual bool isEnabled() const;

  virtual bool isLocked() const;

  virtual bool isAligned() const;

  virtual uint32_t getCRCErrors() const;
  */

  virtual void retrieveMetricValues();

private:
  uint32_t channelID_;
  DummyDriver& driver_;
};

} // namespace test
} // namespace processor
} // namespace swatch
#endif /* SWATCH_PROCESSOR_TEST_DUMMYRXPORT_HPP */
