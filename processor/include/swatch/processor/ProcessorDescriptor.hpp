/**
 * @file    ProcessorDescriptor.hpp
 * @author  Alessandro Thea
 * @brief   Brief description
 * @date    09/11/14
 */


#ifndef SWATCH_PROCESSOR_PROCESSORDESCRIPTOR
#define SWATCH_PROCESSOR_PROCESSORDESCRIPTOR

// C++ Headers
#include <ostream>
#include <string>
#include <stdint.h>
#include <vector>
 
namespace swatch {
namespace processor {

/**
 * @class ProcessorPortDescriptor
 * @brief Structure to hold data to build a Processor port.
 * 
 */
class ProcessorPortDescriptor {
public:
    //! Name of the port
    std::string  name;

    //! Port number
    uint32_t number;
};


/**
 * @class ProcessorDescriptor
 * @brief Struct to hold the data to construct a processor
 */

class ProcessorDescriptor {
public:
    //! Name of the Processor
    std::string name;

    //! Class to create the Processor object
    std::string creator;

    //! Uri to access the hardware resource
    std::string uri;

    //! Address table
    std::string addressTable;

    //! Id of the uTCA crate where the Processor is installed 
    std::string crateId;

    //! Slot where the board is installed 
    uint32_t slot;

    //! List of receiver port bags
    std::vector<ProcessorPortDescriptor> rxPorts;

    //! List of transmitter port bags
    std::vector<ProcessorPortDescriptor> txPorts;
};

std::ostream& operator<<(std::ostream& os, const ProcessorDescriptor& pd );

} // namespace processor
} // namespace swatch

#endif