/**
 * @file    ChannelBase.cpp
 * @author  Alessandro Thea
 * @brief   Brief description
 * @date    
 */

#ifndef SWATCH_PROCESSOR_CHANNELBASE_HPP
#define	SWATCH_PROCESSOR_CHANNELBASE_HPP


// C++ HEADERS
#include <stdint.h>
#include <string>
#include <vector>

// SWATCH HEADERS

// OTHER HEADERS

namespace swatch {

namespace processor {

class ChannelBase {
protected:

    /**
     * Constructor
     */
    ChannelBase() {}

    public:
        
    /**
     * Destructor
     */
    virtual ~ChannelBase() {}
    /**
     * Common buffer modes
     */
    enum BufferMode {
        Latency,
        Capture,
        Playback
    };

    /**
     * Channel 
     */
    enum State {
        OK,
        Warning,
        Error
    };
    
    /**
     * @brief Returns the size of the buffer associated with the channel
     */
    virtual uint32_t getBufferSize() const = 0;

    /**
     * @brief returns the masking state of the channel
     * @return True if the channel is masked, False otherwise
     */
    virtual bool isMasked() = 0;

    /**
     * @brief State of the channel
     * @details Returns the overall state of the channel.
     * @return State of the channel. Can be OK, Warning, Error
     */
    virtual State state() const = 0;

    /**
     * @brief [brief description]
     * @details [long description]
     * @return [description]
     */
    virtual std::string stateMessage() const = 0;

    /**
     * @brief Configure the buffer operation mode and bunch crossing range, whenever relevant
     * @details Configures buffer associated to the channel in one of the standard operation modes
     * 
     * @param mode Operation mode.
     */
//    virtual void setBufferMode( BufferMode mode ) = 0;
    virtual void configureBuffer( BufferMode mode, uint32_t firstBx = 0, uint32_t frames = 0 ) = 0;

    /**
     * @brief Set the buffer bunch crossing range.
     * @details Sets the buffer bunch crossing range, for the operation modes that require it
     * 
     * @param startbx First bunch crossing og the range
     * @param stopbx Last bunch crossing of the range
     */
//    virtual void setBufferBxRange( uint32_t startbx, uint32_t length=0 ) = 0;

    /**
     * @brief Mask the channel
     * @details [long description]
     * 
     * @param mask [description]
     */
    virtual void mask( bool mask = true ) = 0;

    /**
     * @brief Download the buffer content.
     * @return Vector of 64-bits words as large as the channel buffer
     */
    virtual std::vector<uint64_t> download() = 0;

    /**
     * @brief Upload pattern into the channel buffer
     * 
     * @param data vector of 64-bit words. It's length must not exceed the buffer size. 
     */
    virtual void upload(const std::vector<uint64_t>& data) = 0;

private:

};

} // namespace processor
} // namespace swatch

#endif	/* SWATCH_PROCESSOR_CHANNELBASE_HPP */

