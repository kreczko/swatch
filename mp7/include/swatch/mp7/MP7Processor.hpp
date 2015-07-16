#ifndef SWATCH_MP7_MP7PROCESSOR_HPP
#define SWATCH_MP7_MP7PROCESSOR_HPP


#include "swatch/processor/Processor.hpp"


// Forward declarations
namespace mp7 {
class MP7Controller;
}


namespace swatch {
namespace mp7 {

// Temporary class
struct MP7ClockMode {
    std::string clkCfg;
    std::string clk40Src;
    std::string ttcCfg;
};

class MP7Processor: public processor::Processor {
public:
    MP7Processor(const swatch::core::AbstractStub& aStub);
    virtual ~MP7Processor();
    
    virtual uint64_t retrieveFirmwareVersion() const;
    
    virtual std::string firmwareInfo() const;

    ::mp7::MP7Controller& driver() { return *driver_; } 
    
protected:
    virtual void retrieveMetricValues();
    
private:
    
    ::mp7::MP7Controller* driver_;
};

} // namespace mp7 
} // namespace swatch


#endif

