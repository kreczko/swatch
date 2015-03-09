/**
 * @file    MP7Processor.cpp
 * @author  Alessandro Thea
 * @brief   MP7 board processor implementation
 * @date    06/11/14
 */

#include "swatch/hardware/MP7Processor.hpp"

// Hardware Headers
#include "swatch/hardware/MP7Controls.hpp"
#include "swatch/hardware/MP7TTCInterface.hpp"
#include "swatch/hardware/MP7Commands.hpp"

// Swatch Headers
#include "swatch/logger/Log.hpp"
#include "swatch/processor/ProcessorFactory.hpp"
#include "swatch/processor/ProcessorStub.hpp"

// MP7 Headers
#include "mp7/MP7Controller.hpp"

// Temporary
#include "mp7/ClockingNode.hpp"
#include "mp7/ClockingXENode.hpp"

// uHAL Headers
#include "uhal/HwInterface.hpp"
#include "uhal/ConnectionManager.hpp"
#include "swatch/hardware/MP7Ports.hpp"

#include <iomanip>

// Boost Headers
#include <boost/assign.hpp>

// C++ Headers

namespace swco = swatch::core;
namespace swlog = swatch::logger;
namespace swhw = swatch::hardware;

namespace swatch {
namespace hardware {

SWATCH_PROCESSOR_REGISTER_CLASS(MP7Processor);

MP7Processor::MP7Processor(const std::string& id, const swatch::core::XParameterSet& aPars) :
    Processor(id, aPars),
    driver_(0x0) {
  
    registerCommand<MP7ResetCommand>("reset");
    registerCommand<MP7ConfigureLoopback>("loopback");
    
    processor::ProcessorBag& desc = aPars.get<processor::ProcessorBag>("descriptor");
    crate_ = desc.bag.crate;
    slot_ = desc.bag.slot;

    uhal::HwInterface board = uhal::ConnectionManager::getDevice(id, desc.bag.uri, desc.bag.addressTable) ;
    
    driver_ = new mp7::MP7Controller(board);
    
    // Build subcomponents
    ctrl_ = new MP7Controls( driver_ );
    ttc_  = new MP7TTCInterface( driver_ ); 
    
    uint32_t nRx = driver_->getChannelIDs(mp7::kLinkIDs).channels().size();
    uint32_t nTx = driver_->getChannelIDs(mp7::kLinkIDs).channels().size();
    
    // Instantiate Rx ports
    for( uint32_t k(0); k<nRx; ++k) {
      std::ostringstream oss;
      oss << "rx" << std::setw(2) << std::setfill('0') << k;
      addInput( new MP7RxPort(oss.str(), k, *this) );
    }
    
    // Instantiate Tx ports
    for( uint32_t k(0); k<nTx; ++k) {
      std::ostringstream oss;
      oss << "tx" << std::setw(2) << std::setfill('0') << k;
      addOutput( new MP7TxPort(oss.str(), k, *this) );  
    }
    
    LOG(swlog::kNotice) << "MP7 Processor '" << this->id() << "' built: firmware 0x" << std::hex << ctrl_->firmwareVersion() << std::endl;
    
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

} // namespace hardware
} // namespace swatch