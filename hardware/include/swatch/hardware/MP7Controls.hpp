/* 
 * File:   MP7Controls.hpp
 * Author: Alessandro Thea
 *
 * Created on November 6, 2014, 4:50 PM
 */

#ifndef SWATCH_HARDWARE_MP7CONTROLS_HPP
#define SWATCH_HARDWARE_MP7CONTROLS_HPP


// Swatch Headers
#include "swatch/processor/Controls.hpp"

// Forward declaration

namespace mp7 {
class MP7Controller;
}

namespace swatch {
namespace hardware {

class MP7Processor;

class MP7Controls : public swatch::processor::Controls {
private:
    friend class MP7Processor;
    
    MP7Controls( mp7::MP7Controller* );

public:

    ~MP7Controls();
    

    virtual uint32_t firmwareVersion();
    
    virtual uint32_t numberOfInputs();

    virtual uint32_t numberOfOutputs();

    virtual void reloadFirmware(std::string version);

    virtual void softReset();

    
private:
    
    mp7::MP7Controller* driver_;

};
} // hardware
} // namespace swatch

#endif