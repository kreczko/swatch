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
#include "swatch/processor/Device.hpp"
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

SWATCH_REGISTER_CLASS(swatch::hardware::MP7Processor);


namespace swatch {
namespace hardware {


MP7Processor::MP7Processor(const std::string& id, const swatch::core::XParameterSet& aPars) :
    Processor(id, aPars),
    driver_(0x0) {
  
    registerCommand<MP7ResetCommand>("reset");
    registerCommand<MP7ConfigureLoopback>("loopback");
    
    registerOperation<MP7Configure>("configure");
    
    processor::ProcessorBag& desc = aPars.get<processor::ProcessorBag>("stub");
    crate_ = desc.bag.crate;
    slot_ = desc.bag.slot;

    uhal::HwInterface board = uhal::ConnectionManager::getDevice(id, desc.bag.uri, desc.bag.addressTable) ;
    
    driver_ = new mp7::MP7Controller(board);
    
    // Build subcomponents
    ttc_  = new MP7TTCInterface( driver_ ); 
    
    uint32_t nRx = driver_->getChannelIDs(mp7::kLinkIDs).channels().size();
    uint32_t nTx = driver_->getChannelIDs(mp7::kLinkIDs).channels().size();
    
    // Instantiate Rx ports
    for( uint32_t k(0); k<nRx; ++k) {
      std::ostringstream oss;
      oss << "rx" << std::setw(2) << std::setfill('0') << k;
      device()->addInput( new MP7RxPort(oss.str(), k, *this) );
    }
    
    // Instantiate Tx ports
    for( uint32_t k(0); k<nTx; ++k) {
      std::ostringstream oss;
      oss << "tx" << std::setw(2) << std::setfill('0') << k;
      device()->addOutput( new MP7TxPort(oss.str(), k, *this) );  
    }
    
    LOG(swlog::kNotice) << "MP7 Processor '" << this->id() << "' built: firmware 0x" << std::hex << firmwareVersion() << std::endl;
    
}

MP7Processor::~MP7Processor() {
    delete driver_;
}


const std::string&
MP7Processor::getCrateId() const {
    return crate_;
}


uint32_t
MP7Processor::getSlot() const {
    return slot_;
}

uint64_t
MP7Processor::firmwareVersion() const {
    uhal::ValWord<uint32_t> v = driver_->getCtrl().getNode("id.fwrev").read();
    driver_->hwInterface().dispatch();
    
    return v.value();
}

std::string
MP7Processor::firmwareInfo() const {
  return "";
}


} // namespace hardware
} // namespace swatch