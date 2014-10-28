/**
 * @file    OutputChannel.cpp
 * @author  Alessandro Thea
 * @brief   Brief description
 * @date    
 */

#ifndef SWATCH_PROCESSOR_OUTPUTCHANNEL_HPP
#define SWATCH_PROCESSOR_OUTPUTCHANNEL_HPP


// SWATCH HEADERS
#include "ChannelBase.hpp"

// C++ HEADERS

// OTHER HEADERS

namespace swatch {

namespace processor {

class OutputChannel : public ChannelBase {
protected:
    /**
     * Constructor
     * @param id The name of the processor
     */
    OutputChannel() {}
public:
    virtual ~OutputChannel() {}

private:

};

} // namespace processor
} // namespace swatch

#endif  /* SWATCH_PROCESSOR_OUTPUTCHANNEL_HPP */