// OTHER HEADERS

// SWATCH HEADERS
#include "swatch/processor/AbstractCtrl.hpp"

// C++ HEADERS
#include <iostream>

using namespace std;

namespace swatch {
namespace processor {

AbstractCtrl::AbstractCtrl(Connection* connection) : Component(connection) {
    cout << "AbstractCtrl::AbstractCtrl --> AbstractCtrl CTOR called." << endl;
}

AbstractCtrl::~AbstractCtrl() {
    cout << "AbstractCtrl::~AbstractCtrl --> AbstractCtrl DTOR called." << endl;
}

} // end ns processor
} // end ns swatch
