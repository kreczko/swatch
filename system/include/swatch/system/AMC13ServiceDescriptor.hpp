/**
 * @file    AMC13ServiceDescriptor.hpp
 * @author  Alessandro Thea
 * @brief   Brief description
 * @date    11/11/14
 */

#ifndef SWATCH_SYSTEM_AMC13SERVICEDESCRIPTOR
#define SWATCH_SYSTEM_AMC13SERVICEDESCRIPTOR

// C++ Headers
#include <ostream>
#include <string>
#include <stdint.h>
#include <vector>

namespace swatch {
namespace system {

/**
 * @class AMC13Descriptor
 * @brief Struct to hold the data to construct an AMC13
 */
class AMC13ServiceDescriptor {
public:
    //! Name of the Processor
    std::string name;

    //! Class to create the Processor object
    std::string creator;

    //! Id of the uTCA crate where the Processor is installed 
    std::string crateId;

    //! Slot where the board is installed 
    uint32_t slot;

    //! Uri to access the hardware resource
    std::string t1Uri;

    //! Address table
    std::string t1AddressTable;

    //! Uri to access the hardware resource
    std::string t2Uri;

    //! Address table
    std::string t2AddressTable;

};

std::ostream& operator<<(std::ostream& os, const AMC13ServiceDescriptor& sd );

} // namespace system
} // namespace swatch

#endif