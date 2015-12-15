/* 
 * File:   AMCLinkCollection.cpp
 * Author: ale
 * 
 * Created on October 5, 2015, 4:57 PM
 */

#include "swatch/dtm/AMCPortCollection.hpp"
#include "swatch/dtm/AMCPort.hpp"

namespace swatch {
namespace dtm {
    
AMCPortCollection::AMCPortCollection() :
core::MonitorableObject("amcports") {
}

AMCPortCollection::~AMCPortCollection() {
}

void 
AMCPortCollection::addPort(AMCPort* aAMCPort) {
    this->addMonitorable(aAMCPort);
    mAMCPorts.push_back(aAMCPort);
}


} // namespace amc13
} // namespace swatch 