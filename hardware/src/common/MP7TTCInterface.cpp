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
  driver_(controller)
{
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
    return driver_->getTTC().readBunchCounter();
}


uint32_t MP7TTCInterface::getOrbitCounter() const {
    return driver_->getTTC().readOrbitCounter();
}


uint32_t MP7TTCInterface::getEventCounter() const {
    return driver_->getTTC().readEventCounter();
}


void MP7TTCInterface::implementUpdateMetrics()
{
  setMetricValue<>(metricSingleBitErrors_, driver_->getTTC().readSingleBitErrorCounter());
  setMetricValue<>(metricDoubleBitErrors_, driver_->getTTC().readDoubleBitErrorCounter());
  setMetricValue<>(metricIsClock40Locked_, driver_->getCtrl().clock40Locked());
  setMetricValue<>(metricHasClock40Stopped_, driver_->getCtrl().clock40Stopped());
  setMetricValue<>(metricIsBC0Locked_, driver_->getTTC().readBC0Locked());

}

} // namespace hardware
} // namespace swatch
