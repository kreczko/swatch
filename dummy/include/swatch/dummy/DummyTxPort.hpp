
#ifndef __SWATCH_DUMMY_DUMMYTXPORT_HPP__
#define __SWATCH_DUMMY_DUMMYTXPORT_HPP__


#include "swatch/processor/Port.hpp"


namespace swatch {
namespace dummy {


class DummyProcDriver;

//! Dummy output port implementation (used for testing)
class DummyTxPort : public processor::OutputPort {
public:
  DummyTxPort (const std::string& aId, uint32_t aNumber, DummyProcDriver& aDriver);
  virtual ~DummyTxPort ();

  /*
  virtual bool isEnabled() const;

  virtual bool isOperating() const;
   */
  
  virtual void retrieveMetricValues();
  
private:
  uint32_t channelID_;
  DummyProcDriver& driver_;
};


} // namespace dummy
} // namespace swatch

#endif /* SWATCH_DUMMY_DUMMYTXPORT_HPP */
