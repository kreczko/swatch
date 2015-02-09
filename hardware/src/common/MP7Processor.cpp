/**
 * @file    MP7Processor.cpp
 * @author  Alessandro Thea
 * @brief   MP7 board processor implementation
 * @date    06/11/14
 */


#include "swatch/hardware/MP7Processor.hpp"
#include "swatch/logger/Log.hpp"
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

namespace swco = swatch::core;
namespace swlog = swatch::logger;
namespace swhw = swatch::hardware;

namespace swatch {
namespace hardware {

SWATCH_PROCESSOR_REGISTER_CLASS(MP7Processor);

MP7Processor::MP7Processor(const std::string& id, const swatch::core::ParameterSet& params) :
    Processor(id, params),
    driver_(0x0) {
    using namespace boost::assign;

    const processor::ProcessorStub& desc = params.get<processor::ProcessorStub>("descriptor");

    crate_ = desc.crate;
    slot_ = desc.slot;

    uhal::HwInterface board = uhal::ConnectionManager::getDevice(id, desc.uri, desc.addressTable) ;
    
    driver_ = new mp7::MP7Controller(board);
    
    // The following lines must be moved into the mp7 package 
    if ( driver_->kind() == mp7::MP7Controller::kMP7XE ) {

        clockModes_ ["internal"] = { "internal", "internal", "internal" };
        clockModes_ ["external"] = { "external", "external", "external" };
//        clockModes_ ["external_si5326"] = { "si5326", true, false };

    } else if ( driver_->kind() == mp7::MP7Controller::kMP7R1 ) {

        clockModes_ ["internal"] = { "internal", "internal", "internal" };
        clockModes_ ["external"] = { "external", "external", "external" };
        
    } else {
        // Need a dedicated exception
        throw std::runtime_error("Could not detect the MP7 model. Check your address table");
    }
    
    
    // Build subcomponents
    ctrl_ = new MP7Controls( driver_ );
    ttc_  = new MP7TTCInterface( driver_ ); 
    
    LOG(swlog::kNotice) << "MP7 Processor built: firmware 0x" << std::hex << ctrl_->firmwareVersion() << std::endl;
    
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
        it->second.clkCfg, 
        it->second.clk40Src, 
        it->second.ttcCfg
        );
}



} // namespace hardware
} // namespace swatch