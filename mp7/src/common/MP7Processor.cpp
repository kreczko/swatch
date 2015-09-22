/**
 * @file    MP7Processor.cpp
 * @author  Alessandro Thea
 * @brief   MP7 board processor implementation
 * @date    06/11/14
 */

#include "swatch/mp7/MP7Processor.hpp"


// SWATCH headers
#include "swatch/logger/Log.hpp"
#include "swatch/core/CommandSequence.hpp"
#include "swatch/core/Factory.hpp"
#include "swatch/processor/PortCollection.hpp"
#include "swatch/processor/ProcessorStub.hpp"

// SWATCH MP7 headers
#include "swatch/mp7/MP7Commands.hpp"
#include "swatch/mp7/MP7Operations.hpp"
#include "swatch/mp7/MP7Ports.hpp"
#include "swatch/mp7/MP7ReadoutInterface.hpp"
#include "swatch/mp7/MP7TTCInterface.hpp"

// MP7 Headers
#include "mp7/MP7Controller.hpp"

// uHAL Headers
#include "uhal/HwInterface.hpp"
#include "uhal/ConnectionManager.hpp"

// C++ Headers
#include <iomanip>


namespace swlog = swatch::logger;
namespace swpro = swatch::processor;

SWATCH_REGISTER_CLASS(swatch::mp7::MP7Processor);


namespace swatch {
namespace mp7 {


MP7Processor::MP7Processor(const swatch::core::AbstractStub& aStub) :
    Processor(aStub),
    driver_(0x0)
{
    // Add commands
    core::Command& resetCommand = registerFunctionoid<MP7ResetCommand>("reset");
//    core::Command& mgtsCommand = registerFunctionoid<MP7SetupMGTs>("mgts");
    core::Command& align = registerFunctionoid<MP7AlignLinks>("align");
    
//    registerFunctionoid<MP7ConfigureLoopback>("loopback");
    
    // Add command sequences
//    registerCommandSequence("resetThenMGTs", resetCommand).then(mgtsCommand).then(align);
    
    // Extract stub, and create driver
    const processor::ProcessorStub& stub = getStub();

    uhal::HwInterface board = uhal::ConnectionManager::getDevice(stub.id, stub.uri, stub.addressTable) ;
    driver_ = new ::mp7::MP7Controller(board);
    
    // Build subcomponents
    registerInterface( new MP7TTCInterface( *driver_ ) ); 
    registerInterface( new MP7ReadoutInterface(*driver_) );
    registerInterface( new swpro::PortCollection() );
    
    // Add input and output ports
    for(auto it = stub.rxPorts.begin(); it != stub.rxPorts.end(); it++)
      getPorts().addInput(new MP7RxPort(it->id, it->number, *this));
    for(auto it = stub.txPorts.begin(); it != stub.txPorts.end(); it++)
      getPorts().addOutput(new MP7TxPort(it->id, it->number, *this));

    LOG(swlog::kNotice) << "MP7 Processor '" << this->getId() << "' built: firmware 0x" << std::hex << retrieveFirmwareVersion() << std::endl;
}

MP7Processor::~MP7Processor() {
    delete driver_;
}


uint64_t
MP7Processor::retrieveFirmwareVersion() const {
    uhal::ValWord<uint32_t> v = driver_->getCtrl().getNode("id.fwrev").read();
    driver_->hw().dispatch();
    
    return v.value();
}

std::string
MP7Processor::firmwareInfo() const {
  return "";
}

void MP7Processor::retrieveMetricValues() {
  
  setMetricValue<>(metricFirmwareVersion_, retrieveFirmwareVersion());

}

} // namespace mp7
} // namespace swatch
