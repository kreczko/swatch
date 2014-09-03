// OTHER HEADERS

// SWATCH HEADERS
#include "swatch/processor/AbstractReadoutCtrl.hpp"

// C++ HEADERS
#include <iostream>

using namespace std;

namespace swatch {
namespace processor {

AbstractReadoutCtrl::AbstractReadoutCtrl(Connection* connection) : Component(connection) {
    cout << "ReadoutCtrl::ReadoutCtrl --> ReadoutCtrl CTOR called." << endl;
}

AbstractReadoutCtrl::~AbstractReadoutCtrl() {
    cout << "ReadoutCtrl::~ReadoutCtrl --> ReadoutCtrl DTOR called." << endl;
}

} // end ns processor
} // end ns swatch
