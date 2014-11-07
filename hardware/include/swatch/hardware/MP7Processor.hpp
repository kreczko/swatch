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
    std::string clkOpt;
    bool extClk40Src;
    bool bc0Internal;
};

class MP7Processor: public processor::Processor {
public:
    MP7Processor(const std::string& id, const swatch::core::ParameterSet& params);
    ~MP7Processor();
    
    virtual uint32_t getSlot() const;

    virtual const std::string& getCrateId() const;
    
    virtual std::set<std::string> getModes() const;

    virtual void reset(const std::string& mode);

private:
    
    uint32_t slot_;
    
    std::string crate_;
    
    mp7::MP7Controller* driver_;

    boost::unordered_map<std::string, MP7ClockMode> clockModes_;


};

} // namespace hardware
} // namespace swatch


#endif

