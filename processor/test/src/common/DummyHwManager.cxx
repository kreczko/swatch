/* 
 * File:   threadExample.cxx
 * Author: ale
 *
 * Created on August 23, 2014, 6:34 PM
 */


#include "swatch/processor/test/IpbusDummyHardware.hpp"

using std::cout;
using std::endl;


using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    using namespace swatch::processor::test;
    
    uhal::setLogLevelTo(uhal::Warning());

    IpbusDummyHardware x("ararar", 50010, "${SWATCH_ROOT}/processor/test/etc/dummy.xml");
    x.start();
    IpbusDummyHardware y("zzzzzz", 50011, "${SWATCH_ROOT}/processor/test/etc/dummy.xml");
    y.start();

    cout << "Do we have a pid? " << x.pid() << endl;

    std::cout << "Waiting for thread to do stuffs" << std::endl;
    for (int k(10); k > 0; --k) {
        cout << k << " pid " << x.pid() << "|" << y.pid() << endl;
        sleep(1);
    }

    x.terminate();
    return 0;
}

