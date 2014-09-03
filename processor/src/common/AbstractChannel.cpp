/* 
 * File:   AbstractChannel.cpp
 * Author: mguilbau
 * 
 * Created on 24 juillet 2014, 17:03
 */
// OTHER HEADERS

// SWATCH HEADERS
#include "swatch/processor/AbstractChannel.hpp"

// C++ HEADERS
#include <iostream>

using namespace std;

namespace swatch {
namespace processor {

AbstractChannel::AbstractChannel(Connection* connection) : Component(connection) {
    cout << "AbstractChannel::AbstractChannel --> AbstractChannel CTOR called: " << endl;
}

AbstractChannel::~AbstractChannel() {
    cout << "AbstractChannel::AbstractChannel --> AbstractChannel DTOR called: " << endl;
}

} // end ns processor
} // end ns swatch

