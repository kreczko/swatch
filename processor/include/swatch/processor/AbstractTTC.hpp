#ifndef CACTUSCORE_SWATCH_TTCINTERFACE_TTCINTERFACE_H
#define CACTUSCORE_SWATCH_TTCINTERFACE_TTCINTERFACE_H
// OTHER HEADERS

// SWATCH HEADERS
#include "swatch/processor/Connection.hpp"

// C++ HEADERS
#include <stdint.h>

namespace swatch {

// TTCNODE CLASS
namespace processor {

//! A class derived from swatch::processor::Processor used as a base class for concrete type of uTCA processors

class AbstractTTC {
protected:

    /**
     * Constructor
     * @param id The name of the processor
     */
    AbstractTTC(Connection* connection);

    Connection* connection() {
        return connection_;
    }

public:
    /**
     * Destructor Made virtual to delegate this task to this class's children and avoid potential run time errors
     */
    virtual ~AbstractTTC();

    //functionality
    virtual void enableTTC() = 0;
    virtual void enableBC0() = 0;
    virtual void sendSingleL1A() = 0;
    virtual void sendMultipleL1A() = 0;
    virtual void clearCounters() = 0;
    virtual void clearErrCounters() = 0;
    virtual void maskBC0fromTTC() = 0;
    virtual void captureTTC() = 0;

    virtual void sendBGo(uint32_t command) = 0;

    //monitoring
    virtual uint32_t getBunchCount() = 0;
    virtual uint32_t getEvtCount() = 0;
    virtual uint32_t getOrbitCount() = 0;
    virtual uint32_t getSBEC() = 0;
    virtual uint32_t getDBEC() = 0;
    //virtual void getTTChistory()   = 0;
    //virtual void getTTShistory()   = 0;
    virtual uint32_t getClk40lock() = 0;
    virtual uint32_t getClk40stopped() = 0;
    virtual uint32_t getBC0lock() = 0;
    virtual uint32_t getBC0stopped() = 0;

private:
    Connection* connection_;

};

} // end ns processor
} // end ns swatch
#endif
