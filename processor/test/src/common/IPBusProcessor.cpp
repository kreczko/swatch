/**
 * @file    IPBusProcessor.hpp
 * @author  Alessandro Thea
 * @brief   Brief description
 * @date    
 */

#include "uhal/ConnectionManager.hpp"

// Swatch Headers
#include "swatch/logger/Log.hpp"
#include "swatch/processor/ProcessorFactory.hpp"
#include "swatch/processor/ProcessorStub.hpp"
#include "swatch/processor/test/IPBusProcessor.hpp"
#include "swatch/processor/test/IPBusControls.hpp"
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

namespace swatch {
namespace processor {
namespace test {

SWATCH_PROCESSOR_REGISTER_CLASS(IPBusProcessor);

IPBusProcessor::IPBusProcessor(const std::string& id, const swatch::core::XParameterSet& params) :
    Processor(id, params) {
    using namespace swatch::core;
    using namespace boost::assign;
    
    registerCommand<IPBusResetCommand>("reset");
    registerCommand<IPBusConfigureCommand>("configure");
    registerCommand<IPBusCapture>("capture");
    
    clockModes_ += "internal","external";

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
    ctrl_ = new IPBusControls(hw(), params);
    ttc_  = new IPBusTTC(hw());
    algo_ = new IPBusFakeAlgos(hw());

    // build the list of links based on the firmware informations
    uint32_t nInputs = ctrl()->numberOfInputs();
    uint32_t nOutputs = ctrl()->numberOfOutputs();

    LOG(swlog::kDebug) << "Detected " << nInputs << " rx and " << nOutputs << " tx channels.";

    inputChannels_.reserve(nInputs);
    for (size_t k(0); k < nInputs; ++k) {
        XParameterSet a;
        std::string path = "channels.rx" + boost::lexical_cast<std::string>(k);

        a.insert("path", xdata::String(path));

        inputChannels_.push_back(new IPBusRxChannel(hw(), a));

        LOG(swlog::kDebug) << "rx ch[" << k << "]: size " << inputChannels_[k]->getBufferSize();
    }

    outputChannels_.reserve(nOutputs);
    for (size_t k(0); k < nOutputs; ++k) {
        XParameterSet a, ctrl, buf;
        std::string path = "channels.tx" + boost::lexical_cast<std::string>(k);

        a.insert("path", xdata::String(path));

        outputChannels_.push_back(new IPBusTxChannel(hw(), a));

        LOG(swlog::kDebug) << "tx ch[" << k << "]: size " << outputChannels_[k]->getBufferSize();
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


/*------------------------------------------------------------------------------
 * IPBus Algos
 */

IPBusFakeAlgos::IPBusFakeAlgos(uhal::HwInterface* hwif) : IPBusComponent(hwif) {

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

