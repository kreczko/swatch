#include "swatch/processor/test/IPBusTTC.hpp"

// Boost Headers
#include <boost/assign.hpp>

// C++ Headers
#include <iostream>

// Namespace resolution
using namespace std;

namespace swatch {
namespace processor {
namespace test {

IPBusTTC::IPBusTTC(uhal::HwInterface& hwif) :
IPBusComponent(hwif) {
    using namespace boost::assign;
    configs_ += "internal", "external";
}

IPBusTTC::~IPBusTTC() {
    //    cout << "Our dummy TTC interface is destroyed" << endl;
}


void
IPBusTTC::clearCounters() {
    hw()->getNode("ttc.counters").write(0);
    hw()->getNode("ttc.counters1").write(0);
    hw()->getNode("ttc.counters2").write(0);
    hw()->getNode("ttc.counters3").write(0);
    hw()->dispatch();
}

void
IPBusTTC::clearErrors() {
    hw()->getNode("ttc.counters3").write(0x0);
    hw()->dispatch();
}

void
IPBusTTC::maskBC0Spy(bool mask /* = true */) {
    cout << "Mask BC0 from TTC history" << endl;
}

void
IPBusTTC::spy() {
    cout << "Capturing TTC..." << endl;
}

void
IPBusTTC::sendBGo(uint32_t command = 0) {
    hw()->getNode("ttc.ctrl.sendBGo").write(command);
    hw()->dispatch();
}

//bool IPBusTTC::isEnabled() const {
//    uhal::ValWord<uint32_t> enabled = hw()->getNode("ttc.ctrl.enable").read();
//    hw()->dispatch();
//    return (bool)enabled;
//}
//
//bool IPBusTTC::isGeneratingInternalBC0() const {
//    uhal::ValWord<uint32_t> enabled = hw()->getNode("ttc.ctrl.genBC0").read();
//    hw()->dispatch();
//    return (bool)enabled;
//}

bool IPBusTTC::isBC0SpyMasked() const {
    return false;
}

uint32_t
IPBusTTC::getBunchCounter() const {
    uhal::ValWord<uint32_t> bxctr = hw()->getNode("ttc.counters.bunchCntr").read();
    hw()->dispatch();
    return (uint32_t) bxctr;
}

uint32_t
IPBusTTC::getEventCounter() const {
    uhal::ValWord<uint32_t> evctr = hw()->getNode("ttc.counters1.eventCntr").read();
    hw()->dispatch();
    return (uint32_t) evctr;
}

uint32_t
IPBusTTC::getOrbitCounter() const {
    uhal::ValWord<uint32_t> octr = hw()->getNode("ttc.counters2.orbitCntr").read();
    hw()->dispatch();
    return (uint32_t) octr;
}

uint32_t
IPBusTTC::getSingleBitErrors() const {
    uhal::ValWord<uint32_t> sbec = hw()->getNode("ttc.counters3.singleBitErrCntr").read();
    hw()->dispatch();
    return (uint32_t) sbec;
}

uint32_t
IPBusTTC::getDoubleBitErrors() const {
    uhal::ValWord<uint32_t> dbec = hw()->getNode("ttc.counters3.singleBitErrCntr").read();
    hw()->dispatch();
    return (uint32_t) dbec;
}

//void 
//DummyTTC::getTTChistory() const {
//
//    cout << "Getting TTC history" << endl;
//
//}

bool
IPBusTTC::isClock40Locked() const {

    uhal::ValWord<uint32_t> locked = hw()->getNode("ttc.stat.clk40Locked").read();
    hw()->dispatch();
    return (bool) locked;

}

bool
IPBusTTC::hasClock40Stopped() const {

    // cout << "Getting clock 40 stopped..." << endl;
    bool clk40stopped = 0;
    return clk40stopped;

}

bool
IPBusTTC::isOrbitLocked() const {

    uhal::ValWord<uint32_t> locked = hw()->getNode("ttc.stat.bc0Locked").read();
    hw()->dispatch();
    return (bool) locked;

}

bool
IPBusTTC::hasBC0Stopped() const {

    // cout << "Getting BC0 stopped..." << endl;
    bool BC0stopped = 0;
    return BC0stopped;

}

} // namespace test
} // namespace processor
} // namespace swatch
