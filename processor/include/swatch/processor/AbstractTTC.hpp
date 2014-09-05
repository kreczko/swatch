#ifndef SWATCH_PROCESSOR_ABSTRACTTTC_HPP
#define SWATCH_PROCESSOR_ABSTRACTTTC_HPP
// OTHER HEADERS

// SWATCH HEADERS
#include "swatch/processor/Component.hpp"

// C++ HEADERS
#include <stdint.h>

namespace swatch {
namespace processor {

//! A class derived from swatch::processor::Processor used as a base class for concrete type of uTCA processors

class AbstractTTC : public Component {
protected:

    /**
     * Constructor
     * @param id The name of the processor
     */
    AbstractTTC(Connection* connection) : Component(connection)  {}
public:
    /**
     * Destructor Made virtual to delegate this task to this class's children and avoid potential run time errors
     */
    virtual ~AbstractTTC() {}

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
    virtual uint32_t getSingleBitErrorCounter() = 0;
    virtual uint32_t getDoubleBitErrorCounter() = 0;
    //virtual void getTTChistory()   = 0;
    //virtual void getTTShistory()   = 0;
    virtual uint32_t getClk40lock() = 0;
    virtual uint32_t getClk40stopped() = 0;
    virtual uint32_t getBC0lock() = 0;


};

} // namespace processor
} // namespace swatch
#endif /* SWATCH_PROCESSOR_ABSTRACTTTC_HPP */
