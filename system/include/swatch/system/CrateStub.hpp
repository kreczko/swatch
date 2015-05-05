/**
 * @file    CrateStub.hpp
 * @author  Alessandro Thea
 * @brief   Brief description
 * @date    11/11/14
 */

#ifndef __SWATCH_SYSTEM_CRATESTUB_HPP__
#define __SWATCH_SYSTEM_CRATESTUB_HPP__

// C++ Headers
#include <ostream>

// XDAQ Headers
#include "xdata/String.h"
#include "xdata/Bag.h"

namespace swatch {
namespace system {

/**
 * @class CrateStub
 * @brief Struct to hold the data that is required to construct a swatch::system::Crate object
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

#endif /* __SWATCH_SYSTEM_CRATESTUB_HPP__ */