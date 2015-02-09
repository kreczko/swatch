/**
 * @file    CrateStub.hpp
 * @author  Alessandro Thea
 * @brief   Brief description
 * @date    11/11/14
 */

#ifndef SWATCH_SYSTEM_CRATESTUB
#define SWATCH_SYSTEM_CRATESTUB

// C++ Headers
#include <ostream>
#include <string>
#include <stdint.h>
#include <vector>

namespace swatch {
namespace system {

/**
 * @class CrateStub
 * @brief Struct to hold the data to construct an AMC13
 */
class CrateStub {
public:
    //! Name of the Crate
    std::string name;

    //! Description of the crate
    std::string description;

    //! Geo location of the crate
    std::string location;

};

std::ostream& operator<<(std::ostream& os, const CrateStub& sd );

} // namespace system
} // namespace swatch

#endif /* SWATCH_SYSTEM_CRATESTUB */