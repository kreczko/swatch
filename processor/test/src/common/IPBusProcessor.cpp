/**
 * @file    IPBusProcessor.hpp
 * @author  Alessandro Thea
 * @brief   Brief description
 * @date    
 */

#include "uhal/ConnectionManager.hpp"

// Swatch Headers
#include "swatch/logger/Log.hpp"
#include "swatch/core/Factory.hpp"
#include "swatch/processor/ProcessorStub.hpp"
#include "swatch/processor/test/IPBusProcessor.hpp"
#include "swatch/processor/test/IPBusTTC.hpp"
#include "swatch/processor/test/IPBusRxChannel.hpp"
#include "swatch/processor/test/IPBusTxChannel.hpp"
#include "swatch/processor/test/IPBusProcessorCommands.hpp"
 

// XDAQ Headers
#include "xdata/String.h"

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

namespace swlog = swatch::logger;
namespace swpro = swatch::processor;

SWATCH_REGISTER_CLASS(swatch::processor::test::IPBusProcessor);


namespace swatch {
namespace processor {
namespace test {


IPBusProcessor::IPBusProcessor(const std::string& id, const swatch::core::XParameterSet& params) :
    Processor(id, params) {
    using namespace swatch::core;
    using namespace boost::assign;
    
    registerCommand<IPBusResetCommand>("reset");
    registerCommand<IPBusConfigureCommand>("configure");
    registerCommand<IPBusCapture>("capture");
    
    swpro::ProcessorBag& stub = params.get<swpro::ProcessorBag>("stub");

    crate_ = stub.bag.crate;
    slot_ = stub.bag.slot;

    // Build the objects
    hw_ = new uhal::HwInterface(
            uhal::ConnectionManager::getDevice(
                id,
                stub.bag.uri,
                stub.bag.addressTable)
            );



    //    connection_ = swatch::processor::Connection::make(interface);
    ttc_  = new IPBusTTC(hw());
    algo_ = new IPBusFakeAlgos(hw());
    links_ = new processor::LinkInterface();


    uhal::ValWord<uint32_t> n_rx = hw().getNode("ctrl.infos.nRx").read();
    uhal::ValWord<uint32_t> n_tx = hw().getNode("ctrl.infos.nTx").read();
    hw().dispatch();
    
    // build the list of links based on the firmware informations
    uint32_t nInputs = n_rx;
    uint32_t nOutputs = n_tx;

    LOG(swlog::kDebug) << "Detected " << nInputs << " rx and " << nOutputs << " tx channels.";

//    inputChannels_.reserve(nInputs);
    for (size_t k(0); k < nInputs; ++k) {
        XParameterSet a;
        std::string path = "channels.rx" + boost::lexical_cast<std::string>(k);

        a.insert("path", xdata::String(path));

        IPBusRxChannel* rx = new IPBusRxChannel(strPrintf("rx%02d", k), hw(), a);
        linkInterface()->addInput(rx);
//        inputChannels_.push_back(new IPBusRxChannel(hw(), a));

        LOG(swlog::kDebug) << "rx ch[" << k << "]: size " << rx->getBufferSize();
    }

//    outputChannels_.reserve(nOutputs);
    for (size_t k(0); k < nOutputs; ++k) {
        XParameterSet a, ctrl, buf;
        std::string path = "channels.tx" + boost::lexical_cast<std::string>(k);

        a.insert("path", xdata::String(path));

        
        IPBusTxChannel* tx = new IPBusTxChannel(strPrintf("tx%02d", k), hw(), a);
        linkInterface()->addOutput(tx);
//        outputChannels_.push_back(new IPBusTxChannel(hw(), a));

        LOG(swlog::kDebug) << "tx ch[" << k << "]: size " << tx->getBufferSize();
    }
    
    
}


IPBusProcessor::~IPBusProcessor() {

    delete ttc_;

    delete hw_;

}


const std::string& IPBusProcessor::getCrateId() const {
    return crate_;
}


uint32_t IPBusProcessor::getSlot() const {
    return slot_;
}

uint64_t
IPBusProcessor::firmwareVersion() const {
    uhal::ValWord<uint32_t> fwv = hw().getNode("ctrl.id.fwrev").read();
    hw().dispatch();
    return fwv.value();
}

std::string
IPBusProcessor::firmwareInfo() const {
  return "Some useful informations";
}


uhal::HwInterface& IPBusProcessor::hw() const {
    return *hw_;
}


/*------------------------------------------------------------------------------
 * IPBus Algos
 */

IPBusFakeAlgos::IPBusFakeAlgos(uhal::HwInterface& hwif) : IPBusComponent(hwif) {

}

IPBusFakeAlgos::~IPBusFakeAlgos() {

}

void
IPBusFakeAlgos::reset() {
     LOG(swlog::kDebug) << "Algorithms were reset";
}

}
}
}

