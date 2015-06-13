/**
 * @file    MP7Processor.cpp
 * @author  Alessandro Thea
 * @brief   MP7 board processor implementation
 * @date    06/11/14
 */

#include "swatch/hardware/MP7Processor.hpp"

// Swatch hardware Headers
#include "swatch/hardware/MP7TTCInterface.hpp"
#include "swatch/hardware/MP7Commands.hpp"
#include "swatch/hardware/MP7Operations.hpp"
#include "swatch/hardware/MP7Ports.hpp"

// Swatch Headers
#include "swatch/logger/Log.hpp"
#include "swatch/core/Factory.hpp"
#include "swatch/processor/LinkInterface.hpp"
#include "swatch/processor/ProcessorStub.hpp"

// MP7 Headers
#include "mp7/MP7Controller.hpp"

// uHAL Headers
#include "uhal/HwInterface.hpp"
#include "uhal/ConnectionManager.hpp"

// Boost Headers
#include <boost/assign.hpp>

// C++ Headers
#include <iomanip>


namespace swco = swatch::core;
namespace swlog = swatch::logger;
namespace swhw = swatch::hardware;
namespace swpro = swatch::processor;

SWATCH_REGISTER_CLASS(swatch::hardware::MP7Processor);


namespace swatch {
namespace hardware {


MP7Processor::MP7Processor(const std::string& id, const swatch::core::XParameterSet& aPars) :
    Processor(id, aPars),
    driver_(0x0)
{
    // Add commands
    Register<MP7ResetCommand>("reset");
    Register<MP7ConfigureLoopback>("loopback");
    // Add operations
    Register<MP7Configure>("configure");
    
    // Extract stub, and create driver
    processor::ProcessorStub& stub = aPars.get<processor::ProcessorBag>("stub").bag;

    uhal::HwInterface board = uhal::ConnectionManager::getDevice(id, stub.uri, stub.addressTable) ;
    driver_ = new mp7::MP7Controller(board);
    
    // Build subcomponents
    Add( new MP7TTCInterface( driver_ ) ); 
    Add( new swpro::LinkInterface() );
    
    // Add input and output ports
    std::vector<processor::ProcessorPortBag>::iterator it;
    for(it = stub.rxPorts.begin(); it != stub.rxPorts.end(); it++)
      linkInterface()->addInput(new MP7RxPort(it->bag.name, it->bag.number, *this));
    for(it = stub.txPorts.begin(); it != stub.txPorts.end(); it++)
      linkInterface()->addOutput(new MP7TxPort(it->bag.name, it->bag.number, *this));

    LOG(swlog::kNotice) << "MP7 Processor '" << this->id() << "' built: firmware 0x" << std::hex << retrieveFirmwareVersion() << std::endl;
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

void MP7Processor::implementUpdateMetrics() {
  
  setMetricValue<>(metricFirmwareVersion_, retrieveFirmwareVersion());

}

} // namespace hardware
} // namespace swatch
