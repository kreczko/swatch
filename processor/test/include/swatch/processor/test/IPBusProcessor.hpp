/**
 * @file    IPBusProcessor.hpp
 * @author  Alessandro Thea
 * @brief   Brief description
 * @date    2014
 */

#ifndef SWATCH_PROCESSOR_TEST_IPBUSPROCESSOR_HPP
#define	SWATCH_PROCESSOR_TEST_IPBUSPROCESSOR_HPP

// Swatch Headers
#include "swatch/processor/Processor.hpp"
#include "swatch/core/XParameterSet.hpp"

#include "swatch/processor/Controls.hpp"
#include "swatch/processor/AlgoInterface.hpp"
#include "swatch/processor/InputChannel.hpp"
#include "swatch/processor/OutputChannel.hpp"


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
    IPBusProcessor(const std::string& id, const swatch::core::XParameterSet& params);
    virtual ~IPBusProcessor();

    virtual uint32_t getSlot() const;
    virtual const std::string& getCrateId() const;

    uhal::HwInterface* hw() const;

private:

    uint32_t slot_;
    std::string crate_;

    std::set<std::string> clockModes_;
    //! 
    uhal::HwInterface* hw_;
};


class IPBusFakeAlgos : public swatch::processor::AlgoInterface, public IPBusComponent {
public:
    IPBusFakeAlgos(uhal::HwInterface* hw);
    virtual ~IPBusFakeAlgos();
    
    virtual void reset();

};

        
} // namespace test
} // namespace processor
} // namespace swatch

#endif	/* SWATCH_PROCESSOR_TEST_IPBUSPROCESSOR_HPP */

