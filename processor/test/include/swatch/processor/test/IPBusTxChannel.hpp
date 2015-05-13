/**
 * @file    IPBusTxChannel.hpp
 * @author  Alessandro Thea
 * @brief   Brief description
 * @date    
 */

#ifndef __SWATCH_PROCESSOR_TEST_IPBUXTXCHANNEL_HPP__
#define __SWATCH_PROCESSOR_TEST_IPBUXTXCHANNEL_HPP__

#include "swatch/processor/Port.hpp"
#include "swatch/processor/test/BufferInterface.hpp"
#include "swatch/processor/test/IPBusComponent.hpp"

// Swatch Headers
#include "swatch/core/XParameterSet.hpp"

namespace swatch {
namespace processor {
namespace test {

class IPBusTxChannel : public OutputPort, public IPBusComponent {
public:
    IPBusTxChannel(std::string aId, uhal::HwInterface& hw, const swatch::core::XParameterSet& params);
    virtual ~IPBusTxChannel();
    
    virtual uint32_t getBufferSize() const;

    virtual bool isEnabled() const;
    
    virtual bool isOperating() const;

    virtual void configureBuffer(BufferInterface::BufferMode mode, uint32_t firstBx = 0x0, uint32_t frames = 0x0);
    
    virtual std::vector<uint64_t> download();
    
    virtual void upload(const std::vector<uint64_t>& data);
private:

    std::string ctrlpath_;
    std::string datapath_;
    uint32_t bufferSize_;
};


} // namespace test
} // namespace processor
} // namespace swatch

#endif  /* __SWATCH_PROCESSOR_TEST_IPBUXTXCHANNEL_HPP__ */
