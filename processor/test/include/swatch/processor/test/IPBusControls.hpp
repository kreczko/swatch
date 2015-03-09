/**
 * @file    IPBusControls.hpp
 * @author  Alessandro Thea
 * @brief   Brief description
 * @date    
 */

#ifndef __SWATCH_PROCESSOR_TEST_IPBUSCONTROLS_HPP__
#define __SWATCH_PROCESSOR_TEST_IPBUSCONTROLS_HPP__

#include "swatch/processor/Controls.hpp"
#include "swatch/processor/test/IPBusComponent.hpp"
#include "swatch/core/XParameterSet.hpp"

// C++ Headers
#include <string>
#include <set>


namespace swatch {
namespace processor {
namespace test {

class IPBusControls : public swatch::processor::Controls, public IPBusComponent {
public:
    IPBusControls(uhal::HwInterface& hw, const swatch::core::XParameterSet& params);
    virtual ~IPBusControls();

    virtual uint32_t firmwareVersion();

    virtual uint32_t numberOfInputs();

    virtual uint32_t numberOfOutputs();

//    virtual std::set<std::string> clockConfigs() const {
//        return configs_;
//    }
//    virtual void configureClock(const std::string& config);

    virtual void reloadFirmware(std::string version);
    virtual void softReset();

    //    virtual void clk40Reset();
    //    virtual void configureClk(const swatch::core::ParameterSet& params);


private:
    std::set<std::string> configs_;

    //! poweron values to fake resets
    core::XParameterSet poweron_;

};

} // namespace test
} // namespace processor
} // namespace swatch

#endif  /* __SWATCH_PROCESSOR_TEST_IPBUSCONTROLS_HPP__ */
