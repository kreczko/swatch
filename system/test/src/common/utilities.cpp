
#include "swatch/system/test/utilities.hpp"


#include <iomanip>
#include <sstream>

#include "swatch/processor/ProcessorStub.hpp"


namespace swatch{
namespace system{
namespace test{

void addRxTxPortStubs(swatch::processor::ProcessorStub& aProcStub)
{      
    for (size_t chan=0; chan < 4; ++chan) {
        std::ostringstream ss;
        ss << "rx" << std::setw(2) << std::setfill('0') << chan;
        swatch::processor::ProcessorPortStub portStub( ss.str() );
        portStub.number = chan;
        aProcStub.rxPorts.push_back(portStub);
    }
  
    for (size_t chan=0; chan < 2; ++chan) {
        std::ostringstream ss;
        ss << "tx" << std::setw(2) << std::setfill('0') << chan;
//        xdata::Bag<swatch::processor::ProcessorPortStub> portBag;
        swatch::processor::ProcessorPortStub portStub( ss.str() );
        portStub.number = chan;
        aProcStub.txPorts.push_back(portStub);
    }
}

}
}
}
