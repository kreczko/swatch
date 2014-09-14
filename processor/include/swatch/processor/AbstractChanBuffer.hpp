/**
 * @file    AbstractChanBuffer.cpp
 * @author  Alessandro Thea
 * @brief   Brief description
 * @date    
 */

#ifndef SWATCH_PROCESSOR_ABSTRACTALGO_HPP
#define SWATCH_PROCESSOR_ABSTRACTALGO_HPP

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
     * Destructor
     */
    virtual ~AbstractChanBuffer() {}

    virtual void configure(BufferMode mode, uint32_t firstBx = 0, uint32_t frames = 0) = 0;

    virtual std::vector<uint64_t> download() = 0;
    virtual void upload(const std::vector<uint64_t>& data) = 0;

    virtual uint32_t size() = 0;

};

} // namespace processor
} // namespace swatch

#endif /* SWATCH_PROCESSOR_ABSTRACTALGO_HPP */
