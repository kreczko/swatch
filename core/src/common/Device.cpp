/* 
 * File:   Device.cpp
 * Author: ale
 * 
 * Created on July 11, 2014, 12:51 PM
 */

#include "swatch/core/Device.hpp"

using namespace std;

namespace swatch {
namespace core {

Device::Device(const std::string& aId, const Arguments& aArguments) :
    Object(aId) {
}

Device::~Device() {
}

void
Device::addInput(InputPort* aInput) {
    this->addObj(aInput);
    inputs_.push_back(aInput);
}

void
Device::addOutput(OutputPort* aOutput) {
    this->addObj(aOutput);
    outputs_.push_back(aOutput);
}

}
}
