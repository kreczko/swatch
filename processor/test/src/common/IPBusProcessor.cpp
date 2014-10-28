/**
 * @file    IPBusProcessor.hpp
 * @author  Alessandro Thea
 * @brief   Brief description
 * @date    
 */

#include "uhal/ConnectionManager.hpp"

// Swatch Headers
#include "swatch/processor/test/IPBusProcessor.hpp"
#include "swatch/processor/test/IPBusControls.hpp"
#include "swatch/processor/test/IPBusTTC.hpp"
#include "swatch/processor/test/IPBusRxChannel.hpp"
#include "swatch/processor/test/IPBusTxChannel.hpp"
 
// Boost Headers
#include <boost/assign.hpp>
#include <boost/foreach.hpp>

// C++ Headers
#include <iomanip>

// Namespace Resolution
using std::cout;
using std::endl;
using std::hex;
using std::dec;


namespace swatch {
namespace processor {
namespace test {

IPBusProcessor::IPBusProcessor(const std::string& id, const swatch::core::ParameterSet& params) : Processor(id, params) {
    using namespace swatch::core;
    using namespace boost::assign;
    
    clockConfigs_ += "internal","external";
    
    crate_ = params.get<std::string>("crate");
    slot_ = params.get<uint32_t>("slot");


    // Build the objects
    hw_ = new uhal::HwInterface(
            uhal::ConnectionManager::getDevice(
            id,
            params.get<std::string>("uri"),
            params.get<std::string>("addrtab")
            )
            );



    //    connection_ = swatch::processor::Connection::make(interface);
    ctrl_ = new IPBusControls(hw(), params);
    ttc_ = new IPBusTTC(hw());
    algo_ = new IPBusFakeAlgos(hw());

    // build the list of links based on the firmware informations
    uint32_t nInputs = ctrl()->numberOfInputs();
    uint32_t nOutputs = ctrl()->numberOfOutputs();

    cout << "Detected " << nInputs << " rx and " << nOutputs << " tx channels." << endl;

    inputChannels_.reserve(nInputs);
    for (size_t k(0); k < nInputs; ++k) {
        ParameterSet a;
        std::string path = "channels.rx" + boost::lexical_cast<std::string>(k);

        a.insert("path", path);

        inputChannels_.push_back(new IPBusRxChannel(hw(), a));

        cout << "rx ch[" << k << "]: size " << inputChannels_[k]->getBufferSize() << endl;
    }

    outputChannels_.reserve(nOutputs);
    for (size_t k(0); k < nOutputs; ++k) {
        ParameterSet a, ctrl, buf;
        std::string path = "channels.tx" + boost::lexical_cast<std::string>(k);

        a.insert("path", path);

        outputChannels_.push_back(new IPBusTxChannel(hw(), a));

        cout << "tx ch[" << k << "]: size " << outputChannels_[k]->getBufferSize() << endl;
    }

}


IPBusProcessor::~IPBusProcessor() {

    delete ctrl_;
    delete ttc_;

    std::vector<InputChannel*>::iterator itTx;
    for (itTx = inputChannels_.begin(); itTx != inputChannels_.end(); ++itTx) delete (*itTx);
    inputChannels_.clear();

    std::vector<OutputChannel*>::iterator itRx;
    for (itRx = outputChannels_.begin(); itRx != outputChannels_.end(); ++itRx) delete (*itRx);
    outputChannels_.clear();

    delete hw_;

}


const std::string& IPBusProcessor::getCrateId() const {
    return crate_;
}


uint32_t IPBusProcessor::getSlot() const {
    return slot_;
}


uhal::HwInterface* IPBusProcessor::hw() const {
    return hw_;
}

void IPBusProcessor::reset(const std::string& config) {
    // std::string clock = params.get<std::string>("clock");
    // std::string ttc = params.get<std::string>("ttc");

    // cout << "Found clock configurations:" << endl;

    // BOOST_FOREACH(std::string c, p->ctrl()->clockConfigurations()) {
    //     cout << " - " << c << endl;
    // }

    // cout << "Found ttc configurations:" << endl;

    // BOOST_FOREACH(std::string c, p->ttc()->configurations()) {
    //     cout << " - " << c << endl;
    // }

    // // Standard soft reset procedure
    // p->ctrl()->softReset();

    // // Change clock configuration
    // // Every board must have 'internal' and 'external' modes
    // p->ctrl()->configureClock(clock);

    // // Enable/Disable TTC?
    // // Doesn't this pertain to configuration?
    // // Maybe not... hard to do anything w/o orbit signal configured
    // // Should this be related to the clock mode?
    // // Anyway, extrnal TTC signals are incompatible with internally generated orbits.
    // p->ttc()->configure(ttc);

    // std::cout << ">> Is TTC enabled? " << p->ttc()->isEnabled() << std::endl;
    // p->ttc()->clearCounters();

    // Check clock locked, BC0 lock
    
    // do a soft reset
    this->ctrl()->softReset();

    // p->ctrl()->configureClock(clock); 
    hw()->getNode("ttc.stat.clk40Locked").write(true);
    hw()->dispatch();

    // p->ttc()->configure(ttc);
    
    if ( config == "internal" ) {
        // Disable ttc, enable internal generator
        hw()->getNode("ttc.ctrl.enable").write(0x0);
        hw()->getNode("ttc.ctrl.genBC0").write(0x1);
    } else if ( config == "extenal" ) {
        // Disable ttc, enable internal generator
        hw()->getNode("ttc.ctrl.enable").write(0x0);
        hw()->getNode("ttc.ctrl.genBC0").write(0x1); 
    }

    hw()->getNode("ttc.stat.bc0Locked").write(true);
    hw()->dispatch();


    this->ttc()->clearErrors();
    this->ttc()->clearCounters();

}

std::vector<std::string> IPBusProcessor::clockModes() const {
    return clockConfigs_;
}

/*------------------------------------------------------------------------------
 * IPBus Algos
 */

IPBusFakeAlgos::IPBusFakeAlgos(uhal::HwInterface* hwif) : IPBusComponent(hwif) {

}

IPBusFakeAlgos::~IPBusFakeAlgos() {

}

void
IPBusFakeAlgos::reset() {
     cout << "Algorithms were reset" << endl;
}

/*------------------------------------------------------------------------------
 * Dummy Channel Control
 */
 /*
IPBusChanCtrl::IPBusChanCtrl(uhal::HwInterface* hwif, const swatch::core::ParameterSet& params) : IPBusComponent(hwif) {
    cout << "this is a very dummy channel control interface" << endl;
}

IPBusChanCtrl::~IPBusChanCtrl() {
    cout << "Our dummy channel control is destroyed" << endl;
}

void
IPBusChanCtrl::reset() {
    cout << "Reset the channel..." << endl;
    sleep(1);
}

void
IPBusChanCtrl::setLoopback() {
    cout << "set loopback mode" << endl;
}

void
IPBusChanCtrl::resetCRCCounts() {
    cout << "reset CRC counters" << endl;
}

uint32_t
IPBusChanCtrl::getCRCCounts() {
    cout << "getting CRC counters" << endl;
    uint32_t CRCcntr = 1;
    return CRCcntr;
}

uint32_t
IPBusChanCtrl::getCRCErrorCounts() {
    cout << "getting CRC errors counters" << endl;
    uint32_t CRCerrCntr = 1;
    return CRCerrCntr;
}

bool
IPBusChanCtrl::isPLLLocked() {
    cout << "getting PLL lock status" << endl;
    bool PLLlock = true;
    return PLLlock;
}

bool
IPBusChanCtrl::isSync() {

    cout << "getting synchronization status" << endl;
    bool sync = true;
    return sync;

}

void
IPBusChanCtrl::configure(const swatch::core::ParameterSet& params) {
    cout << "configure the channel control with a given set of parameters " << endl;
}
*/
/*------------------------------------------------------------------------------
 * Dummy Channel Buffer 
 */
 /*
IPBusChanBuffer::IPBusChanBuffer(uhal::HwInterface* hwif, const swatch::core::ParameterSet& params) : IPBusComponent(hwif) {
    //    cout << "this is a very dummy channel buffer interface" << endl;

    std::string path = params.get<std::string>("path");
    // cout << ">>>>> path: " << path << endl;
    ctrlpath_ = path + ".ctrl";
    datapath_ = path + ".data";
    bufferSize_ = hw()->getNode(datapath_).getSize() / 2.;

    // BOOST_FOREACH( const std::string& n, hw()->getNode(ctrlpath_).getNodes()) {
    //     cout << " * " << n << endl;
    // }
    cout << "IPBus buffer was built with a size: " << dec << size() << endl;
}

IPBusChanBuffer::~IPBusChanBuffer() {
    cout << "Our dummy channel buffer is destroyed" << endl;
}

void
IPBusChanBuffer::configure(BufferMode mode, uint32_t firstBx = 0, uint32_t frames = 0) {

    // Check for overflows

    cout << "Selected range is: bx = " << std::dec << firstBx << " , frames = " << frames << endl;

    const uhal::Node& ctrl = hw()->getNode(ctrlpath_);
    ctrl.getNode("mode").write(mode);
    ctrl.getNode("firstBx").write(firstBx);
    ctrl.getNode("nFrames").write(frames);

    hw()->dispatch();
}

std::vector<uint64_t>
IPBusChanBuffer::download() {
    std::vector<uint64_t> data(bufferSize_);

    uhal::ValVector<uint32_t> block = hw()->getNode(datapath_).readBlock(2 * bufferSize_);
    hw()->dispatch();

    for (size_t k(0); k < data.size(); ++k) {
        data[k] = block[ 2 * k ];
        // cast to 64bits and shift by 32
        data[k] += ((uint64_t) block[2 * k + 1]) << 32;
    }
    return data;
}

void
IPBusChanBuffer::upload(const std::vector<uint64_t>& data) {

    // Need to mangle the 64 bits into 2x32 bits words to make it more realistic
    std::vector<uint32_t> block;
    block.reserve(2 * data.size());

    // std::vector<uint64_t>::const_iterator it;

    BOOST_FOREACH(const uint64_t& x, data) {
        block.push_back(x & 0xffffffff);
        block.push_back((x >> 32) & 0xffffffff);
    }

    hw()->getNode(datapath_).writeBlock(block);
    hw()->dispatch();
}
*/
}
}
}

