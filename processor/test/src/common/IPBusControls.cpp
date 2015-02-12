/**
 * @file    IPBusControls.cpp
 * @author  Alessandro Thea
 * @brief   Brief description
 * @date    
 */


#include "swatch/processor/test/IPBusControls.hpp"

// Boost Headers
#include <boost/assign.hpp>
#include <boost/foreach.hpp>


// C++ Headers
#include <iostream>

// Swatch Headers
#include "swatch/core/ParameterSet.hpp"

// Namespace resolution
using namespace std;

namespace swatch {
namespace processor {
namespace test {


/*------------------------------------------------------------------------------
 * Dummy Controls
 */
IPBusControls::IPBusControls(uhal::HwInterface* hwif, const swatch::core::XParameterSet& params) : IPBusComponent(hwif) {
    using namespace boost::assign;
    configs_ += "internal", "external";

    poweron_ = params.get<RegisterMap>("poweron");
}

IPBusControls::~IPBusControls() {
}

uint32_t
IPBusControls::firmwareVersion() {
    uhal::ValWord<uint32_t> fwv = hw()->getNode("ctrl.id.fwrev").read();
    hw()->dispatch();
    return fwv.value();
}

uint32_t
IPBusControls::numberOfInputs() {
    uhal::ValWord<uint32_t> n_rx = hw()->getNode("ctrl.infos.nRx").read();
    hw()->dispatch();
    return (uint32_t) n_rx;
}

uint32_t
IPBusControls::numberOfOutputs() {
    uhal::ValWord<uint32_t> n_tx = hw()->getNode("ctrl.infos.nTx").read();
    hw()->dispatch();
    return (uint32_t) n_tx;
}

void
IPBusControls::reloadFirmware(std::string version) {
    cout << "WARNING: Reloading firmware on the processor" << endl;
    sleep(1);
}

void
IPBusControls::softReset() {

    BOOST_FOREACH(RegisterMap::value_type& p, poweron_) {
        hw()->getNode(p.first).write(p.second);
    }
    hw()->dispatch();
}

//void
//IpbusCtrl::clk40Reset() {
//    cout << "Resetting clock 40" << endl;
//}

//void
//IPBusControls::configureClock(const std::string& config) {
//    // Let's assume it works and set
//    hw()->getNode("ttc.stat.clk40Locked").write(true);
//    hw()->dispatch();
//
//}

} // namespace test
} // namespace processor
} // namespace swatch
