// OTHER HEADERS

// SWATCH HEADERS
#include "swatch/processor/AbstractTTC.hpp"

// C++ HEADERS
#include <iostream>
#include <string>

using namespace std;

namespace swatch {
namespace processor {

AbstractTTC::AbstractTTC(Connection* connection) : connection_(connection) {
    cout << "TTCinterface::TTCinterface --> TTCinterface CTOR called." << endl;
}

AbstractTTC::~AbstractTTC() {
    cout << "TTCinterface::~TTCinterface --> TTCinterface DTOR called." << endl;
}

} // end ns processor
} // end ns swatch
