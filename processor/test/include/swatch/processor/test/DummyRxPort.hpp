/*
 * DymmyRxPort.hpp
 */

#ifndef SWATCH_PROCESSOR_TEST_DUMMYRXPORT_HPP
#define SWATCH_PROCESSOR_TEST_DUMMYRXPORT_HPP
#include "swatch/core/Port.hpp"
#include <string>

namespace swatch {
namespace processor {
namespace test {
/**
 * @class DummyRxPort
 * @brief Dummy input port implementation
 */
class DummyRxPort : public core::InputPort {
public:
  DummyRxPort(const std::string& aId);

  virtual ~DummyRxPort();

  virtual bool isEnabled() const;

  virtual bool isLocked() const;

  virtual bool isAligned() const;


  virtual uint32_t getCRCErrors() const;

};

} // namespace test
} // namespace processor
} // namespace swatch
#endif /* SWATCH_PROCESSOR_TEST_DUMMYRXPORT_HPP */
