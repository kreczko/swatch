#ifndef CACTUSCORE_SWATCH_READOUTCTRL_READOUTCTRL_H
#define CACTUSCORE_SWATCH_READOUTCTRL_READOUTCTRL_H

// OTHER HEADERS
#include "swatch/processor/Connection.hpp"

// SWATCH HEADERS
#include "swatch/processor/Processor.hpp"

// C++ HEADERS

namespace swatch {

// READOUTCTRL CLASS
namespace processor {

//! A class derived from swatch::processor::Processor used as a base class for concrete type of uTCA processors

class AbstractReadoutCtrl {
protected:

    /**
     * Constructor
     * @param id The name of the processor
     */
    AbstractReadoutCtrl(Connection* connection);

    Connection* connection() {
        return connection_;
    }

public:

    /**
     * Destructor Made virtual to delegate this task to this class's children and avoid potential run time errors
     */
    ~AbstractReadoutCtrl();

private:
    Connection* connection_;

};

} // end ns processor
} // end ns swatch
#endif
