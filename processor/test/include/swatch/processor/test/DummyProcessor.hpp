/**
 * @file    IPBusProcessor.hpp
 * @author  Alessandro Thea
 * @brief   Brief description
 * @date    
 */

#ifndef __SWATCH_PROCESSOR_TEST_DUMMYPROCESSOR_HPP__
#define	__SWATCH_PROCESSOR_TEST_DUMMYPROCESSOR_HPP__


// C++ headers
#include <vector>

// SWATCH headers
#include "swatch/processor/Processor.hpp"


namespace swatch {
namespace processor {
namespace test {

//----------------------------------------------------------------------------//
class DummyDriver;
    
    
class DummyProcessor : public swatch::processor::Processor {
public:
  DummyProcessor( const swatch::core::AbstractStub& aStub );
  virtual ~DummyProcessor();

  virtual std::string firmwareInfo() const;

  const std::vector<std::string> ranTests() const;
  void test1();
  void test2();

  DummyDriver& getDriver() {return *driver_;}
  
//    static swatch::core::XParameterSet generateParams();
  static ProcessorStub generateParams( const std::string& aId );

protected:
  virtual void retrieveMetricValues();

private:
  boost::scoped_ptr<DummyDriver> driver_;
 
  std::vector<std::string> ranTests_;
    
  static ProcessorPortStub getPortBag(const std::string& name, size_t number);
};

        
} // namespace test
} // namespace processor
} // namespace swatch

#endif	/* SWATCH_PROCESSOR_TEST_DUMMYPROCESSOR_HPP */

