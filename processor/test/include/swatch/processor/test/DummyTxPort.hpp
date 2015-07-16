/*
 * DymmyRxPort.hpp
 */

#ifndef SWATCH_PROCESSOR_TEST_DUMMYTXPORT_HPP
#define SWATCH_PROCESSOR_TEST_DUMMYTXPORT_HPP
#include "swatch/processor/Port.hpp"

namespace swatch {
namespace processor {
namespace test {
/**
 * @class DummyTxPort
 * @brief Dummy ouput port implementation
 */
class DummyTxPort : public OutputPort {
public:
  DummyTxPort ( const std::string& aId);
  virtual ~DummyTxPort ();

  /*
  virtual bool isEnabled() const;

  virtual bool isOperating() const;
   */
  
  virtual void retrieveMetricValues();
};
} // namespace test
} // namespace processor
} // namespace swatch
#endif /* SWATCH_PROCESSOR_TEST_DUMMYTXPORT_HPP */
