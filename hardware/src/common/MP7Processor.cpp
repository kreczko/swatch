#include "swatch/hardware/MP7Processor.hpp"
#include "swatch/processor/ProcessorFactory.hpp"


// MP7 Headers
#include "mp7/MP7Controller.hpp"
// Temporary
#include "mp7/ClockingNode.hpp"
#include "mp7/ClockingXENode.hpp"

// uHAL Headers
#include "uhal/HwInterface.hpp"
#include "uhal/ConnectionManager.hpp"
#include "uhal/log/log.hpp"


// Boost Headers
#include <boost/algorithm/string/predicate.hpp>

// C++ Headers

namespace swatch {
namespace hardware {

SWATCH_PROCESSOR_REGISTER_CLASS(MP7Processor);

MP7Processor::MP7Processor(const std::string& id, const swatch::core::ParameterSet& params) :
    Processor(id, params) {

    std::string deviceID = "";
    std::string uri = "";
    std::string addrTable = "";
    
    uhal::setLogLevelTo(uhal::Warning());
    uhal::HwInterface board = uhal::ConnectionManager::getDevice(deviceID, uri, addrTable) ;
    
    
    // The following lines must be moved into the mp7 package 
    if ( dynamic_cast<const mp7::ClockingXENode*>( &(board.getNode("ctrl.clocking")) ) != 0x0 ) {
        driver_ = new mp7::MP7XEController(board);
    } else if ( dynamic_cast<const mp7::ClockingNode*>( &(board.getNode("ctrl.clocking")) ) != 0x0 ) {
        driver_ = new mp7::MP7R1Controller(board); 
    } else {
        // Need a dedicated exception
        throw std::runtime_error("Could not detect the MP7 model. Check your address table");
    }
    
}

MP7Processor::~MP7Processor() {

}

const std::string& MP7Processor::getCrateId() const {
    return crate_;
}

uint32_t MP7Processor::getSlot() const {
    return slot_;
}

std::vector<std::string> MP7Processor::clockModes() const {
    return clockModes_;
}

void MP7Processor::reset(const std::string& mode) {
    
}



} // namespace hardware
} // namespace swatch