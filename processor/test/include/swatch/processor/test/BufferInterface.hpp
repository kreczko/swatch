/**
 * @file    ChannelBase.cpp
 * @author  Alessandro Thea
 * @brief   Brief description
 * @date    
 */

#ifndef __SWATCH_PROCESSOR_TEST_BUFFERINTERFACE_HPP__
#define	__SWATCH_PROCESSOR_TEST_BUFFERINTERFACE_HPP__


// C++ HEADERS
#include <stdint.h>
#include <string>
#include <vector>

// SWATCH HEADERS

// OTHER HEADERS

namespace swatch {
namespace processor {
namespace test {

class BufferInterface {
protected:

    /**
     * Constructor
     */
    BufferInterface() {}

    public:
        
    /**
     * Destructor
     */
    virtual ~BufferInterface() {}
    /**
     * Common buffer modes
     */
    enum BufferMode {
        Latency,
        Capture,
        Playback
    };

    /**
     * @brief Returns the size of the buffer associated with the channel
     */
    virtual uint32_t getBufferSize() const = 0;

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

} // namespace test
} // namespace processor
} // namespace swatch

#endif	/* __SWATCH_PROCESSOR_TEST_BUFFERINTERFACE_HPP__ */

