/**
 * @file    MP7Processor.cpp
 * @author  Alessandro Thea
 * @brief   MP7 board processor implementation
 * @date    06/11/14
 */


#include "swatch/hardware/MP7Processor.hpp"
#include "swatch/processor/ProcessorFactory.hpp"
#include "swatch/processor/ProcessorStub.hpp"

// Hardware Headers
#include "swatch/hardware/MP7Controls.hpp"
#include "swatch/hardware/MP7TTCInterface.hpp"

// MP7 Headers
#include "mp7/MP7Controller.hpp"

// Temporary
#include "mp7/ClockingNode.hpp"
#include "mp7/ClockingXENode.hpp"

// uHAL Headers
#include "uhal/HwInterface.hpp"
#include "uhal/ConnectionManager.hpp"


// Boost Headers
#include <boost/assign.hpp>

// C++ Headers

namespace swatch {
namespace hardware {

SWATCH_PROCESSOR_REGISTER_CLASS(MP7Processor);

MP7Processor::MP7Processor(const std::string& id, const swatch::core::ParameterSet& params) :
    Processor(id, params),
    driver_(0x0) {
    using namespace boost::assign;

    const processor::ProcessorStub& descriptor = params.get<processor::ProcessorStub>("descriptor");

    crate_ = descriptor.crate;
    slot_ = descriptor.slot;

    uhal::HwInterface board = uhal::ConnectionManager::getDevice(id, descriptor.uri, descriptor.addressTable) ;
    
    
    // The following lines must be moved into the mp7 package 
    if ( dynamic_cast<const mp7::ClockingXENode*>( &(board.getNode("ctrl.clocking")) ) != 0x0 ) {
        driver_ = new mp7::MP7XEController(board);

        clockModes_ ["internal"] = { "si570", false, true };
        clockModes_ ["external"] = { "si570", true, false };
        clockModes_ ["external_si5326"] = { "si5326", true, false };

        std::cout << "MP7XEController built" << std::endl;

    } else if ( dynamic_cast<const mp7::ClockingNode*>( &(board.getNode("ctrl.clocking")) ) != 0x0 ) {
        driver_ = new mp7::MP7R1Controller(board); 

        clockModes_ ["internal"] = { "", false, true };
        clockModes_ ["external"] = { "", true, false };
        
        std::cout << "MP7R1Controller built" << std::endl;
    } else {
        // Need a dedicated exception
        throw std::runtime_error("Could not detect the MP7 model. Check your address table");
    }
    
    
    // Build subcomponents
    ctrl_ = new MP7Controls( driver_ );
    ttc_  = new MP7TTCInterface( driver_ ); 
    
    std::cout << "MP7 Processor built: firmware 0x" << std::hex << ctrl_->firmwareVersion() << std::endl;
    
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


std::set<std::string>
MP7Processor::getModes() const {

    std::set<std::string> modes;
   
    boost::unordered_map<std::string, MP7ClockMode>::const_iterator it;
    for ( it = clockModes_.begin(); it != clockModes_.end(); ++it)
        modes.insert(it->first);
    
    return modes;
}


void
MP7Processor::reset(const std::string& mode) {

    boost::unordered_map<std::string, MP7ClockMode>::const_iterator it;

    if ( (it = clockModes_.find(mode)) == clockModes_.end() ) {
        throw std::runtime_error("Clock mode "+mode+" not found");
    }

    driver_->reset(
        it->second.clkOpt, 
        it->second.extClk40Src, 
        it->second.bc0Internal
        );
}



} // namespace hardware
} // namespace swatch