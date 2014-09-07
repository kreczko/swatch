/* 
 * File:   DummyProcessor.hpp
 * Author: mguilbau
 *
 * Created on 18 juillet 2014, 11:17
 */
#ifndef DUMMYPROCESSOR_HPP
#define	DUMMYPROCESSOR_HPP

// OTHER HEADERS

// SWATCH HEADERS
#include "swatch/processor/Processor.hpp"
#include "swatch/processor/Connection.hpp"
#include "swatch/core/ParameterSet.hpp"
#include "swatch/processor/AbstractInfo.hpp"
#include "swatch/processor/AbstractTTC.hpp"
#include "swatch/processor/AbstractCtrl.hpp"

// C++ HEADERS

namespace swatch {
namespace processor {
namespace test {

typedef boost::unordered_map<std::string,uint32_t> RegisterMap;


//----------------------------------------------------------------------------//
class IpbusProcessor : public swatch::processor::Processor {
public:
    IpbusProcessor(const std::string& id, const swatch::core::Arguments& args);
    virtual ~IpbusProcessor();
    
    virtual uint32_t getSlot() const;
    virtual const std::string& getCrateId() const;
    
private:
    
    uint32_t slot_;
    std::string crate_;
    
    //!
};

//----------------------------------------------------------------------------//
class IpbusInfo : public swatch::processor::AbstractInfo {
public:
    
    IpbusInfo(swatch::processor::Connection* connection);
    virtual ~IpbusInfo(); 
    
    virtual uint32_t getFwVersion();

private:
    uhal::HwInterface hw() { return connection()->get<uhal::HwInterface>(); }

};

//----------------------------------------------------------------------------//
class IpbusTTC : public swatch::processor::AbstractTTC {
public:
    IpbusTTC(swatch::processor::Connection* connection);
    virtual ~IpbusTTC();

    virtual std::set<std::string> configurations() const { return configs_; } 
    virtual void configure(const std::string& config);

    //functionality
    virtual void enable(bool enable = true);
    virtual void generateInternalOrbit(bool generate = true);
    virtual void sendSingleL1A();
    virtual void sendMultipleL1A();
    virtual void clearCounters();
    virtual void clearErrCounters();
    virtual void spy();
    virtual void maskBC0Spy(bool mask = true);
    virtual void sendBGo(uint32_t command);

    virtual bool isEnabled() const;
    virtual bool isGeneratingInternalBC0() const;
    virtual bool isBC0SpyMasked() const;

    
    //monitoring
    virtual uint32_t getBunchCount() const;
    virtual uint32_t getEvtCount() const;
    virtual uint32_t getOrbitCount() const;
    virtual uint32_t getSingleBitErrorCounter() const;
    virtual uint32_t getDoubleBitErrorCounter() const;
    
    // virtual void getTTChistory() const;
    virtual bool isClock40Locked() const;
    virtual bool hasClock40Stopped() const;
    virtual bool isBC0Locked() const;
    virtual bool hasBC0Stopped() const;

private:
    uhal::HwInterface hw() const { return connection()->get<uhal::HwInterface>(); }

    std::set<std::string> configs_;

};

//----------------------------------------------------------------------------//
class IpbusCtrl : public swatch::processor::AbstractCtrl {
public:
    IpbusCtrl(swatch::processor::Connection* connection, const swatch::core::Arguments& args);
    virtual ~IpbusCtrl();

    virtual std::set<std::string> clockConfigurations() const { return configs_; } 
    virtual void configureClock(const std::string& config);
    
    virtual void hardReset();
    virtual void softReset();

//    virtual void clk40Reset();
//    virtual void configureClk(const swatch::core::ParameterSet& pset);


private:
    uhal::HwInterface hw()  { return connection()->get<uhal::HwInterface>(); }
    std::set<std::string> configs_;

    //! poweron values to fake resets
    RegisterMap poweron_;

};

//----------------------------------------------------------------------------//
class IpbusChannel : public swatch::processor::AbstractChannel {
public:
    IpbusChannel(swatch::processor::Connection* connection);
    virtual ~IpbusChannel();
};

//----------------------------------------------------------------------------//
class IpbusChanCtrl : public swatch::processor::AbstractChanCtrl {
public:
    IpbusChanCtrl(swatch::processor::Connection* connection);
    virtual ~IpbusChanCtrl();

    //functionalities
    virtual void reset();
    virtual void setLoopback();
    virtual void resetCRCcounts();

    //monitoring
    virtual uint32_t getCRCcounts();
    virtual uint32_t getCRCErrcounts();
    virtual bool isPLLLocked();
    virtual bool isSync();

    virtual void configure(const swatch::core::ParameterSet& pset);
};
//----------------------------------------------------------------------------//
class IpbusChanBuffer : public swatch::processor::AbstractChanBuffer {
public:
    IpbusChanBuffer(swatch::processor::Connection* connection);
    virtual ~IpbusChanBuffer();


    virtual uint32_t getBufferSize() { return bufferSize_; }
    virtual void configure(BufferMode aMode, uint32_t aFirstBx, uint32_t aLastBx);

    virtual std::vector<uint64_t> download();
    virtual void upload(const std::vector<uint64_t>& aPayload);
    
private:
    uhal::HwInterface hw()  { return connection()->get<uhal::HwInterface>(); }
    
    uint32_t bufferSize_;
    std::vector<uint64_t> payload_;

};

} // namespace test
} // namespace processor
} // namespace swatch

#endif	/* DUMMYPROCESSOR_HPP */

