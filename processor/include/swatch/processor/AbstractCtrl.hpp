#ifndef CACTUSCORE_SWATCH_CTRLNODE_CTRLNODE_H
#define CACTUSCORE_SWATCH_CTRLNODE_CTRLNODE_H

// OTHER HEADERS
#include "swatch/processor/Connection.hpp"

// SWATCH HEADERS
#include "swatch/core/ParameterSet.hpp"

// C++ HEADERS

namespace swatch {

// CTRLNODE CLASS
namespace processor {

//! A class derived from swatch::processor::Processor used as a base class for concrete type of uTCA processors

class AbstractCtrl {
protected:
    /**
     * Constructor
     * @param id The name of the processor
     */
    AbstractCtrl(Connection* connection);

    Connection* connection() {
        return connection_;
    }

public:
    /**
     * Destructor Made virtual to delegate this task to this class's children and avoid potential run time errors
     */
    virtual ~AbstractCtrl();

    virtual void hardReset() = 0;
    virtual void softReset() = 0;

    virtual void clk40Reset() = 0;
    virtual void configureClk(const swatch::core::ParameterSet& pset) = 0;

private:
    Connection* connection_;

};

} // end ns processor
} // end ns swatch
#endif
