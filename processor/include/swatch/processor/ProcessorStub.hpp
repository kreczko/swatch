/**
 * @file    ProcessorStub.hpp
 * @author  Alessandro Thea
 * @brief   Brief description
 * @date    09/11/14
 */


#ifndef __SWATCH_PROCESSOR_PROCESSORSTUB_HPP__
#define __SWATCH_PROCESSOR_PROCESSORSTUB_HPP__

// C++ Headers
#include <ostream>
 
// XDAQ Headers
#include "xdata/String.h"
#include "xdata/Vector.h"
#include "xdata/UnsignedInteger.h"
#include "xdata/Bag.h"

namespace swatch {
namespace processor {

/**
 * @class ProcessorPortStub
 * @brief Structure to hold data to build a Processor port.
 * 
 */
struct ProcessorPortStub {

    void registerFields(xdata::Bag<ProcessorPortStub> *bag) {
        bag->addField("name", &name);
        bag->addField("number", &number);
    }

    //! Name of the port
    xdata::String  name;

    //! Port number
    xdata::UnsignedInteger number;
};


typedef xdata::Bag<ProcessorPortStub> ProcessorPortBag;

/**
 * @class ProcessorStub
 * @brief Struct to hold the data to construct a processor
 */

struct ProcessorStub {

    void registerFields(xdata::Bag<ProcessorStub> *bag) {
        bag->addField("name", &name);
        bag->addField("creator", &creator);
        bag->addField("crate", &crate);
        bag->addField("slot", &slot);
        bag->addField("uri", &uri);
        bag->addField("addressTable", &addressTable);
        bag->addField("rxPorts", &rxPorts);
        bag->addField("txPorts", &txPorts);
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
    xdata::String uri;

    //! Address table
    xdata::String addressTable;

    //! List of receiver port bags
    xdata::Vector<xdata::Bag<ProcessorPortStub> > rxPorts;

    //! List of transmitter port bags
    xdata::Vector<xdata::Bag<ProcessorPortStub> > txPorts;
};

typedef xdata::Bag<ProcessorStub> ProcessorBag;

std::ostream& operator<<(std::ostream& os, const ProcessorStub& pd );

} // namespace processor
} // namespace swatch

#endif /* __SWATCH_PROCESSOR_PROCESSORSTUB_HPP__ */