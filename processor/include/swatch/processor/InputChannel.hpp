/**
 * @file    InputChannel.cpp
 * @author  Alessandro Thea
 * @brief   Brief description
 * @date    
 */

#ifndef SWATCH_PROCESSOR_INPUTCHANNEL_HPP
#define SWATCH_PROCESSOR_INPUTCHANNEL_HPP

// SWATCH HEADERS
#include "swatch/processor/ChannelBase.hpp"

// C++ HEADERS

// OTHER HEADERS

namespace swatch {

namespace processor {

class InputChannel : public ChannelBase {
protected:
    /**
     * Constructor
     * @param id The name of the processor
     */
    InputChannel() {}
public:
    virtual ~InputChannel() {};

    /**
     * @brief Status of the optical link.
     * @return True if the channel is optically locked, False otherwise.
     */
    virtual bool isLocked() const = 0;

    /**
     * @brief Status of protocol decoder.
     * @return True if the channel is operating, False otherwise
     */
    virtual bool isOperating() const = 0;


    virtual bool hasTranmissionErrors() const = 0;
    

    virtual void clearErrors() = 0;
private:

};

} // namespace processor
} // namespace swatch

#endif  /* SWATCH_PROCESSOR_INPUTCHANNEL_HPP */