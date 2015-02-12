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

// XDAQ Headers
#include "xdata/String.h"
#include "xdata/Bag.h"

namespace swatch {
namespace system {

/**
 * @class CrateStub
 * @brief Struct to hold the data to construct an AMC13
 */
class CrateStub {
public:

    void registerFields(xdata::Bag<CrateStub> *bag) {
        bag->addField("name", &name);
        bag->addField("description", &description);
        bag->addField("location", &location);
    }

    //! Name of the Crate
    xdata::String name;

    //! Description of the crate
    xdata::String description;

    //! Geo location of the crate
    xdata::String location;

};


typedef xdata::Bag<CrateStub> CrateBag;

std::ostream& operator<<(std::ostream& os, const CrateStub& sd );

} // namespace system
} // namespace swatch

#endif /* SWATCH_SYSTEM_CRATESTUB */