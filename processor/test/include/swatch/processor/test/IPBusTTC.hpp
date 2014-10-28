/**
 * @file    IPBusTTC.hpp
 * @author  Alessandro Thea
 * @brief   Brief description
 * @date    
 */

#ifndef SWATCH_PROCESSOR_TEST_IPBUSTTC_HPP
#define SWATCH_PROCESSOR_TEST_IPBUSTTC_HPP

#include "swatch/processor/TTCInterface.hpp"
#include "swatch/processor/test/IPBusComponent.hpp"

// C++ Headers
#include <string>
#include <set>

namespace swatch {
namespace processor {
namespace test {

class IPBusTTC : public swatch::processor::TTCInterface, public IPBusComponent {
public:
    IPBusTTC(uhal::HwInterface* hw);
    virtual ~IPBusTTC();

//    virtual std::set<std::string> configurations() const {
//        return configs_;
//    }
//    virtual void configure(const std::string& config);

    //functionality
//    virtual void enable(bool enable = true);
//    virtual void generateInternalOrbit(bool generate = true);
//    virtual void sendSingleL1A();
//    virtual void sendMultipleL1A(uint32_t nL1A);
    virtual void clearCounters();
    virtual void clearErrors();
    virtual void spy();
    virtual void maskBC0Spy(bool mask = true);
    virtual void sendBGo(uint32_t command);

//    virtual bool isEnabled() const;
//    virtual bool isGeneratingInternalBC0() const;
    virtual bool isBC0SpyMasked() const;


    //monitoring
    virtual uint32_t getBunchCounter() const;
    virtual uint32_t getEventCounter() const;
    virtual uint32_t getOrbitCounter() const;
    virtual uint32_t getSingleBitErrors() const;
    virtual uint32_t getDoubleBitErrors() const;

    // virtual void getTTChistory() const;
    virtual bool isClock40Locked() const;
    virtual bool hasClock40Stopped() const;
    virtual bool isOrbitLocked() const;
    virtual bool hasBC0Stopped() const;

private:

    std::set<std::string> configs_;

};

} // namespace test
} // namespace processor
} // namespace swatch

#endif  /* SWATCH_PROCESSOR_TEST_IPBUSTTC_HPP */