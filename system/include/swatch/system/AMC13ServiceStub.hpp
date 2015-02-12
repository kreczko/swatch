/**
 * @file    AMC13ServiceStub.hpp
 * @author  Alessandro Thea
 * @brief   Brief description
 * @date    11/11/14
 */

#ifndef SWATCH_SYSTEM_AMC13SERVICESTUB_HPP
#define SWATCH_SYSTEM_AMC13SERVICESTUB_HPP

// C++ Headers
#include <ostream>
#include <stdint.h>
#include <vector>

// XDAQ Headers
#include "xdata/String.h"
#include "xdata/Vector.h"
#include "xdata/UnsignedInteger.h"
#include "xdata/Bag.h"

namespace swatch {
namespace system {

/**
 * @class AMC13Stub
 * @brief Struct to hold the data to construct an AMC13
 */
struct AMC13ServiceStub {

    void registerFields(xdata::Bag<AMC13ServiceStub> *bag) {
        bag->addField("name", &name);
        bag->addField("creator", &creator);
        bag->addField("crate", &crate);
        bag->addField("slot", &slot);
        bag->addField("uriT1", &uriT1);
        bag->addField("addressTableT1", &addressTableT1);
        bag->addField("uriT2", &uriT2);
        bag->addField("addressTableT2", &addressTableT2);
    }

    //! Name of the Processor
    xdata::String name;

    //! Class to create the Processor object
    xdata::String creator;

    //! Id of the uTCA crate where the Processor is installed 
    xdata::String crate;

    //! Slot where the board is installed 
    xdata::UnsignedInteger slot;

    //! Uri to access the hardware resource
    xdata::String uriT1;

    //! Address table
    xdata::String addressTableT1;

    //! Uri to access the hardware resource
    xdata::String uriT2;

    //! Address table
    xdata::String addressTableT2;

};

std::ostream& operator<<(std::ostream& os, const AMC13ServiceStub& sd );

typedef xdata::Bag<AMC13ServiceStub> AMC13ServiceBag;

} // namespace system
} // namespace swatch

#endif /* SWATCH_SYSTEM_AMC13SERVICESTUB */