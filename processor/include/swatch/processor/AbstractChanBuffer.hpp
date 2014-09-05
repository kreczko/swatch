#ifndef SWATCH_PROCESSOR_ABSTRACTCHANBUFFER_HPP
#define SWATCH_PROCESSOR_ABSTRACTCHANBUFFER_HPP

// SWATCH Headers
#include "swatch/processor/Component.hpp"

// C++ Headers
#include <vector>
#include <stdint.h>

namespace swatch {

namespace processor {

class AbstractChanBuffer : public Component {
protected:

    /**
     * Constructor
     * @param id The name of the processor
     */
    AbstractChanBuffer(Connection* connection) : Component(connection) {}

public:

    enum BufferMode {
        Latency,
        Playback,
        Pattern,
        Capture,
        Zeros
    };

    /**
     * Destructor Made virtual to delegate this task to this class's children and avoid potential run time errors
     */
    virtual ~AbstractChanBuffer() {}

    virtual void configure(BufferMode aMode, uint32_t aFirstBx = 0, uint32_t aLastBx = 0) = 0;

    virtual std::vector<uint64_t> download() = 0;
    virtual void upload(const std::vector<uint64_t>& aPayload) = 0;

    virtual uint32_t getBufferSize() = 0;

};

} // namespace processor
} // namespace swatch

#endif /* SWATCH_PROCESSOR_ABSTRACTCHANBUFFER_HPP */
