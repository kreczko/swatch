/**
 * @file    IPBusProcessor.hpp
 * @author  Alessandro Thea
 * @brief   Brief description
 * @date    2014
 */

#ifndef __SWATCH_PROCESSOR_TEST_IPBUSPROCESSOR_HPP__
#define	__SWATCH_PROCESSOR_TEST_IPBUSPROCESSOR_HPP__


// SWATCH headers
#include "swatch/core/XParameterSet.hpp"
#include "swatch/processor/AlgoInterface.hpp"
#include "swatch/processor/PortCollection.hpp"
#include "swatch/processor/Processor.hpp"
#include "swatch/processor/test/IPBusComponent.hpp"

// uHAL headers
#include "uhal/HwInterface.hpp"


namespace swatch {
namespace processor {
namespace test {

class IPBusTTC;
    
//----------------------------------------------------------------------------//
class IPBusProcessor : public swatch::processor::Processor {
public:
  // TO DELETE
//    IPBusProcessor(const std::string& id, const swatch::core::XParameterSet& params);
    IPBusProcessor( const swatch::core::AbstractStub& aStub );
    virtual ~IPBusProcessor();

    virtual uint64_t firmwareVersion() const;

    virtual std::string firmwareInfo() const;

    uhal::HwInterface& hw() const;

    virtual IPBusTTC& getTTC();
    
protected:
    virtual void retrieveMetricValues() {}
    
private:

    uhal::HwInterface* hw_;
};


class IPBusFakeAlgos : public swatch::processor::AlgoInterface, public IPBusComponent {
public:
    IPBusFakeAlgos(uhal::HwInterface& hw);
    virtual ~IPBusFakeAlgos();
    
    virtual void reset();

};

        
} // namespace test
} // namespace processor
} // namespace swatch

#endif	/* __SWATCH_PROCESSOR_TEST_IPBUSPROCESSOR_HPP__ */

