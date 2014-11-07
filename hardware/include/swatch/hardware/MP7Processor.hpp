#ifndef SWATCH_HARDWARE_MP7PROCESSOR_HPP
#define SWATCH_HARDWARE_MP7PROCESSOR_HPP

#include "swatch/processor/Processor.hpp"

// Forward declarations

namespace mp7 {
class MP7Controller;
}

namespace swatch {
namespace hardware {

class MP7Processor: public processor::Processor {
public:
    MP7Processor(const std::string& id, const swatch::core::ParameterSet& params);
    ~MP7Processor();
    

    virtual uint32_t getSlot() const;

    virtual const std::string& getCrateId() const;
    

    virtual std::vector<std::string> clockModes() const;
    

    virtual void reset(const std::string& mode);

private:
    
    uint32_t slot_;
    
    std::string crate_;

    std::vector<std::string> clockModes_;
    
    mp7::MP7Controller* driver_;

};

} // namespace hardware
} // namespace swatch


#endif

