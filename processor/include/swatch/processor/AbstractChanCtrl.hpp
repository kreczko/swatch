#ifndef CACTUSCORE_SWATCH_CHANNELCTRL_CHANNELCTRL_H
#define CACTUSCORE_SWATCH_CHANNELCTRL_CHANNELCTRL_H
// OTHER HEADERS

// SWATCH HEADERS
#include "swatch/processor/Connection.hpp"
#include "swatch/core/ParameterSet.hpp"

// C++ HEADERS
#include <stdint.h>

namespace swatch {

// CHANNELCTRL CLASS
namespace processor {

//! A class derived from swatch::processor::Processor used as a base class for concrete type of uTCA processors

class AbstractChanCtrl {
protected:

    /**
     * Constructor
     * @param id The name of the processor
     */
    AbstractChanCtrl(Connection* connection);

    Connection* connection() {
        return connection_;
    }

public:

    /**
     * Destructor Made virtual to delegate this task to this class's children and avoid potential run time errors
     */
    virtual ~AbstractChanCtrl();

    virtual void reset() = 0;
    virtual void setLoopback() = 0;
    virtual void resetCRCcounts() = 0;

    virtual uint32_t getCRCcounts() = 0;
    virtual uint32_t getCRCErrcounts() = 0;
    virtual bool isPLLlock() = 0;
    virtual bool isSync() = 0;

    virtual void configure(const swatch::core::ParameterSet& pset) = 0;


private:
    Connection* connection_;

};

} // end ns processor
} // end ns swatch
#endif
