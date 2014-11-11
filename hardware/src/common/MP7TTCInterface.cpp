/* 
 * File:   MP7TCCInterface.hpp
 * Author: Alessandro Thea
 *
 * Created on November 6, 2014, 4:50 PM
 */

#include "swatch/hardware/MP7TTCInterface.hpp"
// MP7 Headers
#include "mp7/MP7Controller.hpp"
#include "mp7/CtrlNode.hpp"
#include "mp7/TTCNode.hpp"

namespace swatch {
namespace hardware {

MP7TTCInterface::MP7TTCInterface( mp7::MP7Controller* controller ) :
driver_(controller) {

}

MP7TTCInterface::~MP7TTCInterface() {

}

void 
MP7TTCInterface::clearCounters() {
    driver_->getTTC().clear();
}


void MP7TTCInterface::clearErrors() {
    driver_->getTTC().clearErrors();
}


uint32_t MP7TTCInterface::getBunchCounter() const {
    return driver_->getTTC().getBunchCounter();
}


uint32_t MP7TTCInterface::getOrbitCounter() const {
    return driver_->getTTC().getOrbitCounter();
}


uint32_t MP7TTCInterface::getEventCounter() const {
    return driver_->getTTC().getEventCounter();
}


uint32_t MP7TTCInterface::getSingleBitErrors() const {
    return driver_->getTTC().getSingleBitErrorCounter();
}


uint32_t MP7TTCInterface::getDoubleBitErrors() const {
    return driver_->getTTC().getDoubleBitErrorCounter();
}


bool MP7TTCInterface::isClock40Locked() const {
    return driver_->getCtrl().clock40Locked();
}

bool MP7TTCInterface::hasClock40Stopped() const {
    return driver_->getCtrl().clock40Stopped();
}

bool MP7TTCInterface::isOrbitLocked() const {
    return driver_->getTTC().bc0Locked();
}

bool MP7TTCInterface::hasBC0Stopped() const {
    return driver_->getTTC().bc0Error();
}

bool MP7TTCInterface::isBC0SpyMasked() const {
    return false;
}

void MP7TTCInterface::maskBC0Spy(bool mask) {

}

void MP7TTCInterface::sendBGo(uint32_t command) {

}

void MP7TTCInterface::spy() {

}

} // namespace hardware
} // namespace swatch
