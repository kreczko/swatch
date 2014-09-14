/* 
 * File:   DummyProcessor.hpp
 * Author: mguilbau
 *
 * Created on 18 juillet 2014, 11:17
 */
#ifndef DUMMYPROCESSOR_HPP
#define	DUMMYPROCESSOR_HPP

#define IPBUS_DEF(X) \
    uhal::HwInterface* hw() const { \
        return conn()->get<uhal::HwInterface>(); \
    } 



// Swatch Headers
#include "swatch/processor/Processor.hpp"
#include "swatch/processor/Connection.hpp"
#include "swatch/core/ParameterSet.hpp"
#include "swatch/processor/AbstractInfo.hpp"
#include "swatch/processor/AbstractTTC.hpp"
#include "swatch/processor/AbstractCtrl.hpp"
#include "swatch/processor/AbstractChannel.hpp"
#include "swatch/processor/AbstractChanCtrl.hpp"
#include "swatch/processor/AbstractChanBuffer.hpp"

// C++ Headers



namespace swatch {
namespace processor {
namespace test {

// Typedefs
typedef boost::unordered_map<std::string,uint32_t> RegisterMap;

//----------------------------------------------------------------------------//
class IPBusProcessor : public swatch::processor::Processor {
public:
    IPBusProcessor(const std::string& id, const swatch::core::Arguments& args);
    virtual ~IPBusProcessor();
    
    virtual uint32_t getSlot() const;
    virtual const std::string& getCrateId() const;
    
private:
    
    uint32_t slot_;
    std::string crate_;
    
    //!
};

//----------------------------------------------------------------------------//
class IPBusInfo : public swatch::processor::AbstractInfo {
public:
    
    IPBusInfo(swatch::processor::Connection* connection);
    virtual ~IPBusInfo(); 
    
    virtual uint32_t getFwVersion();

    virtual uint32_t getNInputs();

    virtual uint32_t getNOutputs();


private:
    IPBUS_DEF(IPBusInfo);
};

//----------------------------------------------------------------------------//
class IPBusTTC : public swatch::processor::AbstractTTC {
public:
    IPBusTTC(swatch::processor::Connection* connection);
    virtual ~IPBusTTC();

    virtual std::set<std::string> configurations() const { return configs_; } 
    virtual void configure(const std::string& config);

    //functionality
    virtual void enable(bool enable = true);
    virtual void generateInternalOrbit(bool generate = true);
    virtual void sendSingleL1A();
    virtual void sendMultipleL1A(uint32_t nL1A);
    virtual void clearCounters();
    virtual void clearErrors();
    virtual void spy();
    virtual void maskBC0Spy(bool mask = true);
    virtual void sendBGo(uint32_t command);

    virtual bool isEnabled() const;
    virtual bool isGeneratingInternalBC0() const;
    virtual bool isBC0SpyMasked() const;

    
    //monitoring
    virtual uint32_t getBXCount() const;
    virtual uint32_t getEvtCount() const;
    virtual uint32_t getOrbitCount() const;
    virtual uint32_t getSingleBitErrors() const;
    virtual uint32_t getDoubleBitErrors() const;
    
    // virtual void getTTChistory() const;
    virtual bool isClock40Locked() const;
    virtual bool hasClock40Stopped() const;
    virtual bool isOrbitLocked() const;
    virtual bool hasBC0Stopped() const;

private:
    IPBUS_DEF(IPBusTTC);

    std::set<std::string> configs_;

};

//----------------------------------------------------------------------------//
class IPBusCtrl : public swatch::processor::AbstractCtrl {
public:
    IPBusCtrl(swatch::processor::Connection* connection, const swatch::core::Arguments& args);
    virtual ~IPBusCtrl();

    virtual std::set<std::string> clockConfigurations() const { return configs_; } 
    virtual void configureClock(const std::string& config);
    
    virtual void hardReset();
    virtual void softReset();

//    virtual void clk40Reset();
//    virtual void configureClk(const swatch::core::ParameterSet& pset);


private:    
    std::set<std::string> configs_;

    //! poweron values to fake resets
    RegisterMap poweron_;

    IPBUS_DEF(IPBusCtrl);

};

//----------------------------------------------------------------------------//
class IpbusChannel : public swatch::processor::AbstractChannel {
public:
    IpbusChannel(swatch::processor::Connection* connection, const swatch::core::Arguments& args);
    virtual ~IpbusChannel();
};

//----------------------------------------------------------------------------//
class IPBusChanCtrl : public swatch::processor::AbstractChanCtrl {
public:
    IPBusChanCtrl(swatch::processor::Connection* connection, const swatch::core::Arguments& args);
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

    virtual void configure(const swatch::core::ParameterSet& pset);
private:

    IPBUS_DEF(IPBusChanCtrl);

};
//----------------------------------------------------------------------------//
class IPBusChanBuffer : public swatch::processor::AbstractChanBuffer {
public:
    IPBusChanBuffer(swatch::processor::Connection* connection, const swatch::core::Arguments& args);
    virtual ~IPBusChanBuffer();


    virtual uint32_t size() { return bufferSize_; }
    virtual void configure(BufferMode aMode, uint32_t aFirstBx, uint32_t aLastBx);

    virtual std::vector<uint64_t> download();
    virtual void upload(const std::vector<uint64_t>& aPayload);
    
private:

    std::string ctrlpath_;
    std::string datapath_;
    uint32_t bufferSize_;

    IPBUS_DEF(IPBusChanBuffer);
};

} // namespace test
} // namespace processor
} // namespace swatch

#endif	/* DUMMYPROCESSOR_HPP */

