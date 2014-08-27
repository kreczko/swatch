#ifndef CACTUSCORE_SWATCH_CHANNELBUFFERS_CHANNELBUFFERS_H
#define CACTUSCORE_SWATCH_CHANNELBUFFERS_CHANNELBUFFERS_H

// OTHER HEADERS

// SWATCH HEADERS
#include "swatch/processor/Connection.hpp"

// C++ HEADERS
#include <vector>
#include <stdint.h>

namespace swatch {

// CHANNELBUFFERS CLASS
namespace processor {

//! A class derived from swatch::processor::Processor used as a base class for concrete type of uTCA processors

class AbstractChanBuffer {
protected:
    /**
     * Constructor
     * @param id The name of the processor
     */
    AbstractChanBuffer(Connection* connection);

    Connection* connection() {
        return connection_;
    }
    
    std::vector<uint64_t> payload_;

    void setBufferSize( uint32_t bsize ) { buffersize_ = bsize; }

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
    virtual ~AbstractChanBuffer();

    virtual void configure(BufferMode aMode, uint32_t aFirstBx = 0, uint32_t aLastBx = 0) = 0;

    virtual std::vector<uint64_t> download() = 0;
    virtual void upload(const std::vector<uint64_t>& aPayload) = 0;

    virtual uint32_t getBufferSize() { return buffersize_; }

private:
    Connection* connection_;
    uint32_t buffersize_;
    

};

} // end ns processor
} // end ns swatch
#endif
