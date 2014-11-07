#include "swatch/hardware/MP7Controls.hpp"

// Swatch Headers
#include "swatch/hardware/MP7Processor.hpp"

// MP7 Headers
#include "mp7/MP7Controller.hpp"
#include "mp7/CtrlNode.hpp"


// uHAL Headers
#include "uhal/HwInterface.hpp"

namespace swatch {
namespace hardware {
MP7Controls::MP7Controls(mp7::MP7Controller* controller) :
driver_( controller ){

}


MP7Controls::~MP7Controls() {

}

uint32_t
MP7Controls::firmwareVersion() {
    uhal::ValWord<uint32_t> v = driver_->getCtrl().getNode("id.fwrev").read();
    driver_->hwInterface().dispatch();
    
    return v.value();
}


uint32_t
MP7Controls::numberOfInputs() {
    return 0;
}


uint32_t
MP7Controls::numberOfOutputs() {
    return 0;
}


void MP7Controls::reloadFirmware(std::string version) {
    return;
}


void MP7Controls::softReset() {
    driver_->getCtrl().softReset();
}


} //namespace hardware 
} //namespace swatch 


