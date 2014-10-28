/**
 * @file    IPBusRxChannel.hpp
 * @author  Alessandro Thea
 * @brief   Brief description
 * @date    
 */

#ifndef SWATCH_PROCESSOR_TEST_IPBUXRXCHANNEL_HPP
#define SWATCH_PROCESSOR_TEST_IPBUXRXCHANNEL_HPP

#include "swatch/processor/InputChannel.hpp"
#include "swatch/processor/test/IPBusComponent.hpp"

// Swatch Headers
#include "swatch/core/ParameterSet.hpp"

namespace swatch {
namespace processor {
namespace test {

class IPBusRxChannel : public swatch::processor::InputChannel, public IPBusComponent {
public:
    IPBusRxChannel(uhal::HwInterface* hw, const swatch::core::ParameterSet& params);
    virtual ~IPBusRxChannel();
    
    // Basic methods

    virtual uint32_t getBufferSize() const;

    virtual bool isMasked();
    
    virtual ChannelBase::State state() const;

    virtual std::string stateMessage() const;

    virtual void configureBuffer(ChannelBase::BufferMode mode, uint32_t firstBx, uint32_t frames);

//    virtual void setBufferMode(BufferMode mode);

//    virtual void setBufferBxRange(uint32_t startbx, uint32_t stopbx);

    virtual void mask(bool mask);
    
    virtual std::vector<uint64_t> download();
    
    virtual void upload(const std::vector<uint64_t>& data);

   
    // InputChannel speific methods
    
    virtual bool isLocked() const;

    virtual bool isOperating() const;
    
    virtual bool hasTranmissionErrors() const;
    
    virtual void clearErrors();

private:
    std::string ctrlpath_;
    std::string datapath_;
    uint32_t bufferSize_;
};

} // namespace test
} // namespace processor
} // namespace swatch

#endif  /* SWATCH_PROCESSOR_TEST_IPBUXRXCHANNEL_HPP */
