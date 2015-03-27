/**
 * @file    IPBusTTC.hpp
 * @author  Alessandro Thea
 * @brief   Brief description
 * @date    
 */

#ifndef __SWATCH_PROCESSOR_TEST_IPBUSTTC_HPP__
#define __SWATCH_PROCESSOR_TEST_IPBUSTTC_HPP__

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
    IPBusTTC(uhal::HwInterface& hw);
    virtual ~IPBusTTC();

    virtual void clearCounters();
    virtual void clearErrors();

    //monitoring
    virtual uint32_t getBunchCounter() const;
    virtual uint32_t getEventCounter() const;
    virtual uint32_t getOrbitCounter() const;
    virtual uint32_t getSingleBitErrors() const;
    virtual uint32_t getDoubleBitErrors() const;

    // virtual void getTTChistory() const;
    virtual bool isClock40Locked() const;
    virtual bool hasClock40Stopped() const;
    virtual bool isBC0Locked() const;

private:

    std::set<std::string> configs_;

};

} // namespace test
} // namespace processor
} // namespace swatch

#endif  /* __SWATCH_PROCESSOR_TEST_IPBUSTTC_HPP__ */