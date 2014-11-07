/**
 * @file    Controls.hpp
 * @author  Maxime Guilbaud
 * @brief   Brief description
 * @date    24/07/2014
 */

#ifndef SWATCH_PROCESSOR_CONTROLS_HPP
#define SWATCH_PROCESSOR_CONTROLS_HPP

// OTHER HEADERS

// SWATCH HEADERS

// C++ HEADERS
#include <stdint.h>
#include <string>

namespace swatch {

namespace core
{
class ParameterSet;
}

namespace processor {

//! A class derived from swatch::processor::Processor used as a base class for concrete type of uTCA processors
class Controls  {
protected:
    /**
     * Constructor
     * @param id The name of the processor
     */
    Controls() {}

public:
    /**
     * Destructor Made virtual to delegate this task to this class's children and avoid potential run time errors
     */
    virtual ~Controls() {}

    /**
     * 
     * @return Firmware version
     */
    virtual uint32_t firmwareVersion() = 0;

    virtual uint32_t numberOfInputs() = 0;
    
    virtual uint32_t numberOfOutputs() = 0;

//    virtual std::set<std::string> clockConfigs() const = 0;

    virtual void reloadFirmware(std::string version) = 0;
    virtual void softReset() = 0;

//    virtual void clk40Reset() = 0;
//    virtual void configureClock( const std::string& config ) = 0;

};

} // namespace processor
} // namespace swatch
#endif /* SWATCH_PROCESSOR_CONTROLS_HPP */
