#ifndef SWATCH_PROCESSOR_ABSTRACTCHANNELCTRL_H
#define SWATCH_PROCESSOR_ABSTRACTCHANNELCTRL_H


// Swatch Headers
#include "swatch/core/ParameterSet.hpp"
#include "swatch/processor/Component.hpp"

// C++ Headers
#include <stdint.h>

namespace swatch {
namespace processor {

class AbstractChanCtrl : public Component {
protected:

    /**
     * Constructor
     * @param id The name of the processor
     */
    AbstractChanCtrl(Connection* connection) : Component( connection_ ) {}

public:

    /**
     * Destructor Made virtual to delegate this task to this class's children and avoid potential run time errors
     */
    virtual ~AbstractChanCtrl() {}

    virtual void reset() = 0;
    virtual void setLoopback() = 0;
    virtual void resetCRCcounts() = 0;

    virtual uint32_t getCRCcounts() = 0;
    virtual uint32_t getCRCErrcounts() = 0;
    virtual bool isPLLLocked() = 0;
    virtual bool isSync() = 0;

    virtual void configure(const swatch::core::ParameterSet& pset) = 0;
};

} // namespace processor
} // namespace swatch
#endif /* SWATCH_PROCESSOR_ABSTRACTCHANNELCTRL_H */
