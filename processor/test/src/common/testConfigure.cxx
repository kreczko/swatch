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

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/uniform_real_distribution.hpp>
// Namespace resolution
using std::cout;
using std::endl;

struct TTCCountersIncrementer : public swatch::processor::test::WorkLoop {

    TTCCountersIncrementer(uint32_t millisec, uint32_t orbitsPerCycle = 10) :
    WorkLoop(millisec),
    orbitsPerCycle_(orbitsPerCycle),
    randGenerator_(std::time(0)),
    bxDistribution_(0, 0xdec),
    flatDistribution_(0, 1) {
    }

    virtual void operator()(uhal::HwInterface *hw) {

        uhal::ValWord<uint32_t> en = hw->getNode("ttc.ctrl.enable").read();
        uhal::ValWord<uint32_t> gbc0 = hw->getNode("ttc.ctrl.genBC0").read();
        uhal::ValWord<uint32_t> evc = hw->getNode("ttc.counters1.eventCntr").read();
        uhal::ValWord<uint32_t> orc = hw->getNode("ttc.counters2.orbitCntr").read();
        hw->dispatch();

        if ((uint32_t) en or (uint32_t) gbc0) {
            hw->getNode("ttc.counters2.orbitCntr").write(((uint32_t) orc + orbitsPerCycle_) % orbitMax);
            hw->getNode("ttc.counters.bunchCntr").write((uint32_t) bxDistribution_(randGenerator_));
        }

        if ((uint32_t) en and flatDistribution_(randGenerator_) > 0.3)
            hw->getNode("ttc.counters1.eventCntr").write((uint32_t) evc + 1);
        hw->dispatch();
    }

private:
    uint32_t orbitsPerCycle_;

    boost::mt19937 randGenerator_;
    boost::random::uniform_int_distribution<> bxDistribution_;
    boost::random::uniform_real_distribution<> flatDistribution_;

    static const uint32_t orbitMax = 0x1000000;
    static const uint32_t eventMax = 0x1000000;
};

struct TTCErrorGenerator : public swatch::processor::test::WorkLoop {

    TTCErrorGenerator(uint32_t millisec) : WorkLoop(millisec){
    }

    virtual void operator()(uhal::HwInterface *hw) {
        uhal::ValWord<uint32_t> sbec = hw->getNode("ttc.counters3.singleBitErrCntr").read();
        uhal::ValWord<uint32_t> dbec = hw->getNode("ttc.counters3.doubleBitErrCntr").read();
        hw->dispatch();

        uint32_t maxErrs = 0xffff;
        if (sbec < maxErrs) {
            uint32_t errs = ((uint32_t) sbec) + 0x1234;
            hw->getNode("ttc.counters3.singleBitErrCntr").write((errs < maxErrs ? errs : maxErrs));
        }
        if (sbec < maxErrs) {
            uint32_t errs = ((uint32_t) dbec) + 0x2345;
            hw->getNode("ttc.counters3.doubleBitErrCntr").write((errs < maxErrs ? errs : maxErrs));
        }
        hw->dispatch();
    }

};

void reset(swatch::processor::Processor *p, const swatch::core::ParameterSet &pset) {
    std::string clock = pset.get<std::string>("clock");
    std::string ttc = pset.get<std::string>("ttc");

    cout << "Found clock configurations:" << endl;

    BOOST_FOREACH(std::string c, p->ctrl()->clockConfigurations()) {
        cout << " - " << c << endl;
    }

    cout << "Found ttc configurations:" << endl;

    BOOST_FOREACH(std::string c, p->ttc()->configurations()) {
        cout << " - " << c << endl;
    }

    // Standard soft reset procedure
    p->ctrl()->softReset();

    // Change clock configuration
    // Every board must have 'internal' and 'external' modes
    p->ctrl()->configureClock(clock);

    // Enable/Disable TTC?
    // Doesn't this pertain to configuration?
    // Maybe not... hard to do anything w/o orbit signal configured
    // Should this be related to the clock mode?
    // Anyway, extrnal TTC signals are incompatible with internally generated orbits.
    p->ttc()->configure(ttc);

    std::cout << "Is TTC enabled? " << p->ttc()->isEnabled() << std::endl;
    p->ttc()->clearCounters();

}

void configure(swatch::processor::Processor *p, const swatch::core::ParameterSet &pset) {

}

//----------------------------------------------------------------------------//

int main(int argc, char const *argv[]) {
    using namespace swatch::processor;

    const std::string addrtab = "file://${SWATCH_ROOT}/processor/test/etc/dummy.xml";
    /* code */
    test::IpbusDummyHardware x("board-0", 50010, addrtab);
    x.start();
    std::cout << "Started? " << x.started() << std::endl;

    // Inject some data
    test::IpbusDummyHardware::RegisterMap poweron;
    poweron["ctrl.id.fwrev"] = 0x44332211;
    poweron["ctrl.id.magic"] = 0xdeadc0de;
    poweron["ctrl.infos.nTx"] = 4;
    poweron["ctrl.infos.nRx"] = 2;
    poweron["ttc.ctrl"] = 0;
    poweron["ttc.stat"] = 0;
    poweron["ttc.counters"] = 0;
    poweron["ttc.counters1"] = 0;
    poweron["ttc.counters2"] = 0;
    poweron["ttc.counters3"] = 0;
    x.load(poweron);

    // Increment ttc counters
    x.add(new TTCCountersIncrementer(10, 10));
    // Add error generator
    // x.add(new TTCErrorGenerator(10) );

    // x is ready for testing
    std::stringstream ssURI;
    ssURI << "ipbusudp-2.0://127.0.0.1:" << 50010;

    swatch::core::ParameterSet args;
    args.set("addrtab", addrtab);
    args.set("uri", ssURI.str());
    args.set("crate", "s2g20-10");
    args.set("slot", (uint32_t) 5);
    args.set("poweron", poweron);

    Processor *p0 = 0x0;
    try {
        p0 = new test::IpbusProcessor("test-board0", args);
    }    catch (uhal::exception::exception &e) {
        // Crap, let's get out here
        return -1;
    }
    cout << "Processor " << p0->id() << " fw: 0x" << std::hex << p0->info()->getFwVersion() << std::dec << endl;

    // reset?
    // configure?
    // cout << "Event counter: " <<  p0->ttc()->getEvtCount() << endl;
    // cout << "Bunch counter: " <<  p0->ttc()->getBunchCount() << endl;
    // cout << "Orbit counter: " <<  p0->ttc()->getOrbitCount() << endl;

    // - Step 0 ---
    swatch::core::ParameterSet mode;
    mode.insert("clock", "internal")("ttc", "internal");

    reset(p0, mode);
    sleep(1);
    cout << "Event counter: " << p0->ttc()->getEvtCount() << endl;
    cout << "Bunch counter: " << p0->ttc()->getBunchCount() << endl;
    cout << "Orbit counter: " << p0->ttc()->getOrbitCount() << endl;
    cout << "SBEC  counter: " << p0->ttc()->getSingleBitErrorCounter() << endl;
    cout << "DBEC  counter: " << p0->ttc()->getDoubleBitErrorCounter() << endl;

    // - Step 1 ---
    swatch::core::ParameterSet config;
    configure(p0, config);

    //    p0->ttc()->clearCounters();
    //    cout << "Event counter: " <<  p0->ttc()->getEvtCount() << endl;
    //    cout << "Bunch counter: " <<  p0->ttc()->getBunchCount() << endl;
    //    cout << "Orbit counter: " <<  p0->ttc()->getOrbitCount() << endl;
    cout << "---" << endl;
    return 0;
}