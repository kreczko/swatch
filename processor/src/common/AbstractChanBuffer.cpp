
// SWATCH Headers
#include "swatch/processor/AbstractChanBuffer.hpp"

// C++ Headers
#include <iostream>

using namespace std;

namespace swatch {
namespace processor {

AbstractChanBuffer::AbstractChanBuffer(Connection* connection) : Component(connection) {
    // cout << "AbstractChanBuffers::AbstractChanBuffers --> AbstractChanBuffers CTOR called." << endl;
}

AbstractChanBuffer::~AbstractChanBuffer() {
    // cout << "AbstractChanBuffers::~AbstractChanBuffers --> AbstractChanBuffers DTOR called." << endl;
}

} // end ns processor
} // end ns swatch
