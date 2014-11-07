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
#include "swatch/hardware/MP7Controls.hpp"
#include "swatch/hardware/MP7TTCInterface.hpp"

// Boost Headers
#include <boost/foreach.hpp>

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    
    const std::string addrtab = "file:///opt/cactus/etc/mp7/addrtab/mp7fw_v1_4_0_xe/mp7xe_infra.xml";
    const std::string uri = "chtcp-2.0://127.0.0.1:20304?target=192.168.2.95:50001";

    swatch::core::ParameterSet params;
    params.set("addrtab", addrtab);
    params.set("uri", uri);
    params.set("crate", "s2g20-10");
    params.set("slot", (uint32_t) 5);

    swatch::hardware::MP7Processor* mp1=0x0;
    try {
        mp1 = new swatch::hardware::MP7Processor("myfirstmp7", params);
    } catch (...) {
        std::cout << "Crap..." << std::endl;
    }
    
    BOOST_FOREACH( const std::string& mode, mp1->getModes() ) {
        std::cout << " - " << mode << std::endl;
    }
    
    mp1->reset("internal");

    std::cout << "Checking TTC Status" << std::endl;
    std::cout << mp1->ttc()->getBunchCounter() << std::endl;
    std::cout << mp1->ttc()->getOrbitCounter() << std::endl;
    std::cout << mp1->ttc()->getEventCounter() << std::endl;
    std::cout << mp1->ttc()->getSingleBitErrors() << std::endl;
    std::cout << mp1->ttc()->getDoubleBitErrors() << std::endl;

    return 0;
}

