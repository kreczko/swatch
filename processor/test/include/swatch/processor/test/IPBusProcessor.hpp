/**
 * @file    IPBusProcessor.hpp
 * @author  Alessandro Thea
 * @brief   Brief description
 * @date    
 */

#ifndef SWATCH_PROCESSOR_TEST_IPBUSPROCESSOR_HPP
#define	SWATCH_PROCESSOR_TEST_IPBUSPROCESSOR_HPP

// Swatch Headers
#include "swatch/processor/Processor.hpp"
#include "swatch/processor/Connection.hpp"
#include "swatch/core/ParameterSet.hpp"

#include "swatch/processor/Controls.hpp"
#include "swatch/processor/AlgoInterface.hpp"
#include "swatch/processor/InputChannel.hpp"
#include "swatch/processor/OutputChannel.hpp"


#include "swatch/processor/AbstractChannel.hpp"
#include "swatch/processor/AbstractChanCtrl.hpp"
#include "swatch/processor/AbstractChanBuffer.hpp"

#include "swatch/processor/test/IPBusComponent.hpp"

// uHAL Headers
#include "uhal/HwInterface.hpp"

// C++ Headers

namespace swatch {
namespace processor {
namespace test {

//----------------------------------------------------------------------------//

class IPBusProcessor : public swatch::processor::Processor {
public:
    IPBusProcessor(const std::string& id, const swatch::core::ParameterSet& params);
    virtual ~IPBusProcessor();

    virtual uint32_t getSlot() const;
    virtual const std::string& getCrateId() const;

    uhal::HwInterface* hw() const;

    virtual std::vector<std::string> clockModes() const;

    virtual void reset( const std::string& mode );
private:

    uint32_t slot_;
    std::string crate_;

    std::vector<std::string> clockConfigs_;
    //! 
    uhal::HwInterface* hw_;
};


class IPBusFakeAlgos : public swatch::processor::AlgoInterface, public IPBusComponent {
public:
    IPBusFakeAlgos(uhal::HwInterface* hw);
    virtual ~IPBusFakeAlgos();
    
    virtual void reset();

};

// Old classes here
//----------------------------------------------------------------------------//
/*
class IpbusChannel : public swatch::processor::AbstractChannel, public IPBusComponent {
public:
    IpbusChannel(uhal::HwInterface* hw, const swatch::core::ParameterSet& params);
    virtual ~IpbusChannel();
};

//----------------------------------------------------------------------------//

class IPBusChanCtrl : public swatch::processor::AbstractChanCtrl, public IPBusComponent {
public:
    IPBusChanCtrl(uhal::HwInterface* hw, const swatch::core::ParameterSet& params);
    virtual ~IPBusChanCtrl();

    //functionalities
    virtual void reset();
    virtual void setLoopback();
    virtual void resetCRCCounts();

    //monitoring
    virtual uint32_t getCRCCounts();
    virtual uint32_t getCRCErrorCounts();
    virtual bool isPLLLocked();
    virtual bool isSync();

    virtual void configure(const swatch::core::ParameterSet& params);
private:

//    IPBUS_DEF(IPBusChanCtrl);

};
//----------------------------------------------------------------------------//

class IPBusChanBuffer : public swatch::processor::AbstractChanBuffer, public IPBusComponent {
public:
    IPBusChanBuffer(uhal::HwInterface* hw, const swatch::core::ParameterSet& params);
    virtual ~IPBusChanBuffer();

    virtual uint32_t size() {
        return bufferSize_;
    }
    virtual void configure(BufferMode aMode, uint32_t aFirstBx, uint32_t aLastBx);

    virtual std::vector<uint64_t> download();
    virtual void upload(const std::vector<uint64_t>& aPayload);

private:

    std::string ctrlpath_;
    std::string datapath_;
    uint32_t bufferSize_;

//    IPBUS_DEF(IPBusChanBuffer);
};
*/
        
} // namespace test
} // namespace processor
} // namespace swatch

#endif	/* SWATCH_PROCESSOR_TEST_IPBUSPROCESSOR_HPP */

