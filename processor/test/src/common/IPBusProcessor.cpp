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

/* TO DELETE 
IPBusProcessor::IPBusProcessor(const std::string& id, const swatch::core::XParameterSet& params) :
    Processor(id, params) {
    using namespace swatch::core;
    using namespace boost::assign;
    
    // Add commands
    Register<IPBusResetCommand>("reset");
    Register<IPBusConfigureCommand>("configure");
    Register<IPBusCapture>("capture");
    
    // Extract stub
    swpro::ProcessorStub& stub = params.get<swpro::ProcessorBag>("stub").bag;

    // Build the driver and interfaces
    hw_ = new uhal::HwInterface( uhal::ConnectionManager::getDevice(id, stub.uri, stub.addressTable) );
//    Add( new IPBusTTC(hw()) );
//    Add( new IPBusFakeAlgos(hw()) );
    Add( new processor::LinkInterface() );

    // build the list of links based on the firmware informations
    uhal::ValWord<uint32_t> n_rx = hw().getNode("ctrl.infos.nRx").read();
    uhal::ValWord<uint32_t> n_tx = hw().getNode("ctrl.infos.nTx").read();
    hw().dispatch();

    uint32_t nInputs = n_rx;
    uint32_t nOutputs = n_tx;

    LOG(swlog::kDebug) << "Detected " << nInputs << " rx and " << nOutputs << " tx channels.";

    for (size_t k(0); k < nInputs; ++k) {
        XParameterSet a;
        std::string path = "channels.rx" + boost::lexical_cast<std::string>(k);

        a.insert("path", xdata::String(path));
    }

    for (size_t k(0); k < nOutputs; ++k) {
        XParameterSet a, ctrl, buf;
        std::string path = "channels.tx" + boost::lexical_cast<std::string>(k);

        a.insert("path", xdata::String(path));

    }
    
    
}

 */

IPBusProcessor::IPBusProcessor(const swatch::core::AbstractStub& aStub ) :
    Processor(aStub) {
    using namespace swatch::core;
    using namespace boost::assign;
    
    // Add commands
    Register<IPBusResetCommand>("reset");
    Register<IPBusConfigureCommand>("configure");
    Register<IPBusCapture>("capture");
    
    // Extract stub
    const swpro::ProcessorStub& stub = getStub();

    // Build the driver and interfaces
    hw_ = new uhal::HwInterface( uhal::ConnectionManager::getDevice(stub.id, stub.uri, stub.addressTable) );
    Add( new processor::LinkInterface() );

    // build the list of links based on the firmware informations
    uhal::ValWord<uint32_t> n_rx = hw().getNode("ctrl.infos.nRx").read();
    uhal::ValWord<uint32_t> n_tx = hw().getNode("ctrl.infos.nTx").read();
    hw().dispatch();

    uint32_t nInputs = n_rx;
    uint32_t nOutputs = n_tx;

    LOG(swlog::kDebug) << "Detected " << nInputs << " rx and " << nOutputs << " tx channels.";

    for (size_t k(0); k < nInputs; ++k) {
        ReadWriteXParameterSet a;
        std::string path = "channels.rx" + boost::lexical_cast<std::string>(k);

        a.add("path", xdata::String(path));
    }

    for (size_t k(0); k < nOutputs; ++k) {
        ReadWriteXParameterSet a, ctrl, buf;
        std::string path = "channels.tx" + boost::lexical_cast<std::string>(k);

        a.add("path", xdata::String(path));
    }
    
    
}




IPBusProcessor::~IPBusProcessor() {
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

IPBusTTC& IPBusProcessor::ttc() {
  return dynamic_cast<IPBusTTC&>(Processor::ttc());
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

