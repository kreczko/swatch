/**
Build a processor
How to compose a processor?
 * assing connection
 * assign components

- This be delegated to a creator?
- Do we need that flexibility?
- Can I use the IpbusProcessor to develop, actually?
- 
    
test configuration
 */


#include "swatch/core/ParameterSet.hpp"
#include "swatch/processor/test/IpbusDummyHardware.hpp"
#include "swatch/processor/test/IpbusProcessor.hpp"

// Namespace resolution
using std::cout;
using std::endl;

void reset( swatch::processor::Processor* p, const std::string& clockmode ) {
    // Standard soft reset procedure
    p->ctrl()->softReset();

    // Change clock configuration
    p->ctrl()->configureClock( clockmode );
    // Enable/Disable TTC?
    // Doesn't this pertain to configuration?
    // Maybe not... hard to do anything w/o orbit signal configured
    // Should this be related to the clock mode, as it is in the mp7?
    // Can force the reset sequence on other boards?
    // Do we need 
}

int main(int argc, char const *argv[]) {
    using namespace swatch::processor;

    const std::string addrtab = "file://${SWATCH_ROOT}/processor/test/etc/dummy.xml";
    /* code */
    test::IpbusDummyHardware x("board-0", 50010, addrtab);
    x.start();
    std::cout << "Started? " << x.started() << std::endl;

    // Inject some data
    test::IpbusDummyHardware::RegisterMap map;
    map["ctrl.id.fwrev"] = 0x44332211;
    map["ctrl.id.magic"] = 0xdeadc0de;
    map["ctrl.id.infos.n_rx"] = 4;
    map["ctrl.id.infos.n_tx"] = 2;
    x.load(map);

    // x is ready for testing

    std::stringstream ssURI;
    ssURI << "ipbusudp-2.0://127.0.0.1:" << 50010;

    swatch::core::ParameterSet args;
    args.set("addrtab", addrtab);
    args.set("uri", ssURI.str());
    args.set("crate", "s2g20-10");
    args.set("slot", (uint32_t) 5);

    Processor* p0 = 0x0;
    try {
        p0 = new test::IpbusProcessor("test-board0", args);
    } catch (uhal::exception::exception& e) {
        // Crap, let's get out here
        return -1;
    }
    cout << "Processor " << p0->id() << " fw: 0x" << std::hex << p0->info()->getFwVersion() << std::dec << endl;

    // reset?
    // configure?

    return 0;
}