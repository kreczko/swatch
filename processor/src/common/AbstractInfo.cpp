/* 
 * File:   AbstractInfo.cpp
 * Author: mguilbau
 * 
 * Created on 7 août 2014, 11:35
 */
// OTHER HEADERS

// SWATCH HEADERS
#include "swatch/processor/AbstractInfo.hpp"

// C++ HEADERS
#include <iostream>

using namespace std;

namespace swatch {
namespace processor {

AbstractInfo::AbstractInfo(Connection* connection) : Component(connection) {
    cout << "AbstractInfo::AbstractInfo --> AbstractInfo CTOR called." << endl;
}

AbstractInfo::~AbstractInfo() {
    cout << "AbstractInfo::AbstractInfo --> AbstractInfo DTOR called." << endl;
}

} // end ns processor
} // end ns swatch

