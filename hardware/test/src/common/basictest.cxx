/* 
 * File:   basictest.cxx
 * Author: ale
 *
 * Created on November 2, 2014, 10:32 PM
 */

#include <cstdlib>

// Swatch Headers
#include "swatch/core/ParameterSet.hpp"

// MP7 Processor
#include "swatch/hardware/MP7Processor.hpp"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    
    const std::string addrtab = "file://${SWATCH_ROOT}/processor/test/etc/dummy.xml";

    std::stringstream ssURI;
    ssURI << "ipbusudp-2.0://127.0.0.1:" << 50010;

    swatch::core::ParameterSet params;
    params.set("addrtab", addrtab);
    params.set("uri", ssURI.str());
    params.set("crate", "s2g20-10");
    params.set("slot", (uint32_t) 5);

    try {
        swatch::hardware::MP7Processor("myfirstmp7", params);
    } catch (...) {
        std::cout << "Crap..." << std::endl;
    }
        return 0;
}

