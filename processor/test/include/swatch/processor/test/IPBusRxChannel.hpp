/**
 * @file    IPBusRxChannel.hpp
 * @author  Alessandro Thea
 * @brief   Object to simulate a Processor input channels
 * @date    Some time ago
 */

#ifndef SWATCH_PROCESSOR_TEST_IPBUXRXCHANNEL_HPP
#define SWATCH_PROCESSOR_TEST_IPBUXRXCHANNEL_HPP

#include "swatch/core/Port.hpp"
#include "swatch/processor/test/BufferInterface.hpp"
#include "swatch/processor/test/IPBusComponent.hpp"

// Swatch Headers
#include "swatch/core/XParameterSet.hpp"

namespace swatch {
namespace processor {
namespace test {

class IPBusRxChannel : public core::InputPort, public BufferInterface, public IPBusComponent {
public:
    IPBusRxChannel(std::string aId, uhal::HwInterface& hw, const swatch::core::XParameterSet& params);
    virtual ~IPBusRxChannel();
    
    // Input Port interface
    
    virtual bool isEnabled() const;
        
    virtual bool isLocked() const;
    
    virtual bool isAligned() const;

    virtual uint32_t getCRCErrors() const;
    
    virtual void clearErrors();

    virtual uint32_t getBufferSize() const;
    
    virtual void configureBuffer(BufferInterface::BufferMode mode, uint32_t firstBx = 0x0, uint32_t frames = 0x0);

    virtual std::vector<uint64_t> download();
    
    virtual void upload(const std::vector<uint64_t>& data);

   
    // InputChannel specific methods
        


private:
    std::string ctrlpath_;
    std::string datapath_;
    uint32_t bufferSize_;
};

} // namespace test
} // namespace processor
} // namespace swatch

#endif  /* SWATCH_PROCESSOR_TEST_IPBUXRXCHANNEL_HPP */
