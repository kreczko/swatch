/**
 * @file    IPBusProcessor.hpp
 * @author  Alessandro Thea
 * @brief   Brief description
 * @date    
 */

#ifndef SWATCH_PROCESSOR_TEST_DUMMYPROCESSOR_HPP
#define	SWATCH_PROCESSOR_TEST_DUMMYPROCESSOR_HPP

// Swatch Headers
#include "swatch/processor/Processor.hpp"
#include "swatch/core/XParameterSet.hpp"

// C++ Headers
#include <vector>

namespace swatch {
namespace system {
namespace test {

//----------------------------------------------------------------------------//

class DummyConfigProcessor : public swatch::processor::Processor {
public:
  DummyConfigProcessor(const swatch::core::AbstractStub& aStub );
    virtual ~DummyConfigProcessor();

    virtual uint64_t firmwareVersion() const;
    virtual std::string firmwareInfo() const;

    uint32_t number() const;
  
    void setNumber(uint32_t number);
  
    std::string something() const;
  
    void setSomething(std::string something);
  
    void fail();
  
  private:
      
    void retrieveMetricValues() {}
    
    std::string something_;
    uint32_t number_;
};

        
} // namespace test
} // namespace processor
} // namespace swatch

#endif	/* SWATCH_PROCESSOR_TEST_DUMMYPROCESSOR_HPP */

