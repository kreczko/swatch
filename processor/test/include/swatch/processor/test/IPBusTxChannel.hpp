/**
 * @file    IPBusTxChannel.hpp
 * @author  Alessandro Thea
 * @brief   Brief description
 * @date    
 */

#ifndef SWATCH_PROCESSOR_TEST_IPBUXTXCHANNEL_HPP
#define SWATCH_PROCESSOR_TEST_IPBUXTXCHANNEL_HPP

#include "swatch/processor/OutputChannel.hpp"
#include "swatch/processor/test/IPBusComponent.hpp"

// Swatch Headers
#include "swatch/core/XParameterSet.hpp"

namespace swatch {
namespace processor {
namespace test {

class IPBusTxChannel : public swatch::processor::OutputChannel, public IPBusComponent {
public:
    IPBusTxChannel(uhal::HwInterface* hw, const swatch::core::XParameterSet& params);
    virtual ~IPBusTxChannel();
    
    virtual uint32_t getBufferSize() const;

    virtual bool isMasked();
    
    virtual ChannelBase::State state() const;

    virtual std::string stateMessage() const;

    virtual void configureBuffer(ChannelBase::BufferMode mode, uint32_t firstBx, uint32_t frames);

    // virtual void setBufferMode(ChannelBase::BufferMode mode);

    // virtual void setBufferBxRange(uint32_t startbx, uint32_t stopbx);

    virtual void mask(bool mask);
    
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

#endif  /* SWATCH_PROCESSOR_TEST_IPBUXTXCHANNEL_HPP */
