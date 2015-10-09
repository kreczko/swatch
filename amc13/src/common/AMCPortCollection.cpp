/* 
 * File:   AMCLinkCollection.cpp
 * Author: ale
 * 
 * Created on October 5, 2015, 4:57 PM
 */

#include "swatch/amc13/AMCPortCollection.hpp"
#include "swatch/amc13/AMCPort.hpp"

namespace swatch {
namespace amc13 {
    
AMCPortCollection::AMCPortCollection() :
core::MonitorableObject("amcports") {
}

AMCPortCollection::~AMCPortCollection() {
}

void 
AMCPortCollection::addPort(AMCPort* aAMCPort) {
    this->addObj(aAMCPort);
    mAMCPorts.push_back(aAMCPort);
}


} // namespace amc13
} // namespace swatch 