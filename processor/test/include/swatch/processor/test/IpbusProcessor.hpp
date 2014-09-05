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
    
};

//----------------------------------------------------------------------------//
class IpbusInfo : public swatch::processor::AbstractInfo {
public:
    
    IpbusInfo(swatch::processor::Connection* connection);
    virtual ~IpbusInfo(); 
    
    virtual uint32_t getFwVersion();

};

//----------------------------------------------------------------------------//
class IpbusTTC : public swatch::processor::AbstractTTC {
public:
    IpbusTTC(swatch::processor::Connection* connection);
    virtual ~IpbusTTC();

    //functionality
    virtual void enableTTC();
    virtual void enableBC0();
    virtual void sendSingleL1A();
    virtual void sendMultipleL1A();
    virtual void clearCounters();
    virtual void clearErrCounters();
    virtual void maskBC0fromTTC();
    virtual void captureTTC();

    virtual void sendBGo(uint32_t command);

    //monitoring
    uint32_t getBunchCount();
    uint32_t getEvtCount();
    uint32_t getOrbitCount();
    uint32_t getSingleBitErrorCounter();
    uint32_t getDoubleBitErrorCounter();
    //void getTTChistory();
    //void getTTShistory();
    uint32_t getClk40lock();
    uint32_t getClk40stopped();
    uint32_t getBC0lock();
    uint32_t getBC0stopped();

};

//----------------------------------------------------------------------------//
class IpbusCtrl : public swatch::processor::AbstractCtrl {
public:
    IpbusCtrl(swatch::processor::Connection* connection);
    virtual ~IpbusCtrl();

    virtual std::vector<std::string> clockConfigurations() const;
    
    virtual void hardReset();
    virtual void softReset();

//    virtual void clk40Reset();
//    virtual void configureClk(const swatch::core::ParameterSet& pset);

    virtual void configureClock(const std::string& config);

private:
    std::set<std::string> configs_;
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
    virtual bool isPLLlock();
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
    
    uint32_t bufferSize_;
    std::vector<uint64_t> payload_;

};

} // namespace test
} // namespace processor
} // namespace swatch

#endif	/* DUMMYPROCESSOR_HPP */

