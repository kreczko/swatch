/**
 * @file    IPBusProcessor.hpp
 * @author  Alessandro Thea
 * @brief   Brief description
 * @date    
 */

#include "uhal/ConnectionManager.hpp"

// Swatch Headers
#include "swatch/processor/ProcessorFactory.hpp"
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

SWATCH_PROCESSOR_REGISTER_CLASS(IPBusProcessor);

IPBusProcessor::IPBusProcessor(const std::string& id, const swatch::core::ParameterSet& params) :
    Processor(id, params) {
    using namespace swatch::core;
    using namespace boost::assign;
    
    clockModes_ += "internal","external";
    
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

std::set<std::string> IPBusProcessor::getModes() const {
    return clockModes_;
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

}
}
}

