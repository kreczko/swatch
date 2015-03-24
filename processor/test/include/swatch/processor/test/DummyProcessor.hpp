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

#include "swatch/processor/AlgoInterface.hpp"

// uHAL Headers
#include "uhal/HwInterface.hpp"

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

    virtual uint32_t getSlot() const;
    virtual const std::string& getCrateId() const;
    virtual uint64_t firmwareVersion() const;
    virtual std::string firmwareInfo() const;

    static swatch::core::XParameterSet generateParams();

    const std::vector<std::string> ranTests() const;
    void test1();
    void test2();
private:

    uint32_t slot_;
    std::string crate_;

    std::vector<std::string> ranTests_;
};

        
} // namespace test
} // namespace processor
} // namespace swatch

#endif	/* SWATCH_PROCESSOR_TEST_DUMMYPROCESSOR_HPP */

