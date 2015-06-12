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
#include "swatch/processor/ProcessorStub.hpp"
#include "swatch/core/XParameterSet.hpp"
#include "swatch/processor/AlgoInterface.hpp"

// uHAL Headers
#include "uhal/HwInterface.hpp"

// XDAQ headers
#include "xdata/Bag.h"

// C++ Headers
#include <vector>

namespace swatch {
namespace processor {
namespace test {

//----------------------------------------------------------------------------//

class DummyProcessor : public swatch::processor::Processor {
public:
  DummyProcessor(const std::string& id, const swatch::core::XParameterSet& params);
    virtual ~DummyProcessor();

    virtual std::string firmwareInfo() const;

    const std::vector<std::string> ranTests() const;
    void test1();
    void test2();

    static swatch::core::XParameterSet generateParams();

protected:
    virtual void implementUpdateMetrics();

private:

    std::vector<std::string> ranTests_;
    
    static xdata::Bag<ProcessorPortStub> getPortBag(const std::string& name, size_t number);
};

        
} // namespace test
} // namespace processor
} // namespace swatch

#endif	/* SWATCH_PROCESSOR_TEST_DUMMYPROCESSOR_HPP */

