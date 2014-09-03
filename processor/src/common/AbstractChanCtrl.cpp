// OTHER HEADERS

// SWATCH HEADERS
#include "swatch/processor/AbstractChanCtrl.hpp"

// C++ HEADERS
#include <iostream>

using namespace std;

namespace swatch {
namespace processor {

AbstractChanCtrl::AbstractChanCtrl(Connection* connection) : Component(connection) {
    cout << "AbstractChanCtrl::AbstractChanCtrl --> AbstractChanCtrl CTOR called: " << endl;
}

AbstractChanCtrl::~AbstractChanCtrl() {
    cout << "AbstractChanCtrl::~AbstractChanCtrl --> AbstractChanCtrl DTOR called: " << endl;
}

} // end ns processor
} // end ns swatch
