/**
 * @file    ProcessorStub.hpp
 * @author  Alessandro Thea
 * @brief   Brief description
 * @date    09/11/14
 */


#ifndef SWATCH_PROCESSOR_PROCESSORSTUB_HPP
#define SWATCH_PROCESSOR_PROCESSORSTUB_HPP

// C++ Headers
#include <ostream>
#include <string>
#include <stdint.h>
#include <vector>
 
namespace swatch {
namespace processor {

/**
 * @class ProcessorPortStub
 * @brief Structure to hold data to build a Processor port.
 * 
 */
class ProcessorPortStub {
public:
    //! Name of the port
    std::string  name;

    //! Port number
    uint32_t number;
};


/**
 * @class ProcessorStub
 * @brief Struct to hold the data to construct a processor
 */

class ProcessorStub {
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
    std::string crate;

    //! Slot where the board is installed 
    uint32_t slot;

    //! List of receiver port bags
    std::vector<ProcessorPortStub> rxPorts;

    //! List of transmitter port bags
    std::vector<ProcessorPortStub> txPorts;
};

std::ostream& operator<<(std::ostream& os, const ProcessorStub& pd );

} // namespace processor
} // namespace swatch

#endif /* SWATCH_PROCESSOR_PROCESSORSTUB_HPP */