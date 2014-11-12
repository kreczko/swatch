/**
 * @file    AMC13ServiceStub.hpp
 * @author  Alessandro Thea
 * @brief   Brief description
 * @date    11/11/14
 */

#ifndef SWATCH_SYSTEM_AMC13SERVICESTUB
#define SWATCH_SYSTEM_AMC13SERVICESTUB

// C++ Headers
#include <ostream>
#include <string>
#include <stdint.h>
#include <vector>

namespace swatch {
namespace system {

/**
 * @class AMC13Stub
 * @brief Struct to hold the data to construct an AMC13
 */
class AMC13ServiceStub {
public:
    //! Name of the Processor
    std::string name;

    //! Class to create the Processor object
    std::string creator;

    //! Id of the uTCA crate where the Processor is installed 
    std::string crate;

    //! Slot where the board is installed 
    uint32_t slot;

    //! Uri to access the hardware resource
    std::string uriT1;

    //! Address table
    std::string addressTableT1;

    //! Uri to access the hardware resource
    std::string uriT2;

    //! Address table
    std::string addressTableT2;

};

std::ostream& operator<<(std::ostream& os, const AMC13ServiceStub& sd );

} // namespace system
} // namespace swatch

#endif /* SWATCH_SYSTEM_AMC13SERVICESTUB */