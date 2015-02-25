#ifndef SWATCH_HARDWARE_MP7PROCESSOR_HPP
#define SWATCH_HARDWARE_MP7PROCESSOR_HPP

#include "swatch/processor/Processor.hpp"

// Boost Headers
#include <boost/unordered_map.hpp>

// Forward declarations

namespace mp7 {
class MP7Controller;
}

namespace swatch {
namespace hardware {

// Temporary class
struct MP7ClockMode {
    std::string clkCfg;
    std::string clk40Src;
    std::string ttcCfg;
};

class MP7Processor: public processor::Processor {
public:
    MP7Processor(const std::string& id, const swatch::core::XParameterSet& aPars);
    ~MP7Processor();
    
    virtual uint32_t getSlot() const;

    virtual const std::string& getCrateId() const;

    mp7::MP7Controller& driver() { return *driver_; } 
private:
    
    uint32_t slot_;
    
    std::string crate_;
    
    mp7::MP7Controller* driver_;

};

} // namespace hardware
} // namespace swatch


#endif

