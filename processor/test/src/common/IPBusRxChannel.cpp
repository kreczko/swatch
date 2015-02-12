/**
 * @file    IPBusRxChannel.cpp
 * @author  Alessandro Thea
 * @brief   Brief description
 * @date    
 */

#include "swatch/processor/test/IPBusRxChannel.hpp"

// Swatch Headers
#include "swatch/core/ParameterSet.hpp"
 
// Boost Headers
#include <boost/foreach.hpp>

// XDAQ Headers
#include <xdata/String.h>

namespace swatch {
namespace processor {
namespace test {

IPBusRxChannel::IPBusRxChannel(uhal::HwInterface* hwif, const swatch::core::XParameterSet& params) : IPBusComponent(hwif) {
    // cout << "Create Rx Channels" << endl;
    
    std::string path = params.get<xdata::String>("path");

    ctrlpath_ = path + ".ctrl";
    datapath_ = path + ".data";
    bufferSize_ = hw()->getNode(datapath_).getSize() / 2.;
}

IPBusRxChannel::~IPBusRxChannel() {

}

uint32_t
IPBusRxChannel::getBufferSize() const {
    return bufferSize_;
}

bool
IPBusRxChannel::isMasked() {
    return false;
}

ChannelBase::State
IPBusRxChannel::state() const {
    return ChannelBase::OK;
}

std::string
IPBusRxChannel::stateMessage() const {
    return "";
}

void
IPBusRxChannel::configureBuffer(ChannelBase::BufferMode mode, uint32_t firstBx, uint32_t frames) {
    const uhal::Node& ctrl = hw()->getNode(ctrlpath_);
    ctrl.getNode("mode").write(mode);
    ctrl.getNode("firstBx").write(firstBx);
    ctrl.getNode("nFrames").write(frames);
    hw()->dispatch();
}

// void
// IPBusRxChannel::setBufferMode(ChannelBase::BufferMode mode) {
//     const uhal::Node& ctrl = hw()->getNode(ctrlpath_);
    
//     ctrl.getNode("mode").write(mode);
//     hw()->dispatch();
// }

// void
// IPBusRxChannel::setBufferMode(ChannelBase::BufferMode mode) {
//     const uhal::Node& ctrl = hw()->getNode(ctrlpath_);

//     ctrl.getNode("mode").write(mode);
//     hw()->dispatch();
// }

// void
// IPBusRxChannel::setBufferBxRange(uint32_t startbx, uint32_t length) {
//     const uhal::Node& ctrl = hw()->getNode(ctrlpath_);
//     ctrl.getNode("firstBx").write(startbx);
//     ctrl.getNode("nFrames").write(length);

//     hw()->dispatch();
// }

void
IPBusRxChannel::mask(bool mask) {

}


std::vector<uint64_t>
IPBusRxChannel::download() {
    std::vector<uint64_t> data(bufferSize_);

    uhal::ValVector<uint32_t> block = hw()->getNode(datapath_).readBlock(2 * bufferSize_);
    hw()->dispatch();

    for (size_t k(0); k < data.size(); ++k) {
        data[k] = block[ 2 * k ];
        // cast to 64bits and shift by 32
        data[k] += ((uint64_t) block[2 * k + 1]) << 32;
    }
    return data;
}


void
IPBusRxChannel::upload(const std::vector<uint64_t>& data) {
    // Need to mangle the 64 bits into 2x32 bits words to make it more realistic
    std::vector<uint32_t> block;
    block.reserve(2 * data.size());

    // std::vector<uint64_t>::const_iterator it;

    BOOST_FOREACH(const uint64_t& x, data) {
        block.push_back(x & 0xffffffff);
        block.push_back((x >> 32) & 0xffffffff);
    }

    hw()->getNode(datapath_).writeBlock(block);
    hw()->dispatch();
}

bool
IPBusRxChannel::isLocked() const {
    return true;
}

bool
IPBusRxChannel::isOperating() const {
    return true;
}

bool
IPBusRxChannel::hasTranmissionErrors() const {
    return false;
}

void
IPBusRxChannel::clearErrors() {
    
}

} // namespace test
} // namespace processor
} // namespace swatch
