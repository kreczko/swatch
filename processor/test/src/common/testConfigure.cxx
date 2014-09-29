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
#include "swatch/processor/test/IPBusDummyHardware.hpp"
#include "swatch/processor/test/IPBusProcessor.hpp"

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/uniform_real_distribution.hpp>
// Namespace resolution
using std::cout;
using std::endl;

//---
struct TTCCountersIncrementer : public swatch::processor::test::IPBusWorkLoop {

    TTCCountersIncrementer(uint32_t millisec, uint32_t orbitsPerCycle = 10) :
        IPBusWorkLoop(millisec),
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

//---
struct TTCErrorGenerator : public swatch::processor::test::IPBusWorkLoop {

    TTCErrorGenerator(uint32_t millisec) : IPBusWorkLoop(millisec){
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

//---
struct BufferCaptureEmulator : public swatch::processor::test::IPBusWorkLoop {

    typedef boost::unordered_map<std::string, uhal::ValWord<uint32_t> > ValMap;

    BufferCaptureEmulator(uint32_t millisec) : IPBusWorkLoop(millisec), 
        randGenerator_(std::time(0)),
        flatDistribution_(0,0xffffffff) {
    }

    virtual void operator()(uhal::HwInterface *hw) {
        uhal::ValWord<uint32_t> bgo = hw->getNode("ttc.ctrl.sendBGo").read();
        hw->dispatch();

        if ( (uint32_t)bgo == 0xc ) {
            cout << "BGo 12 detected" << endl;
        
            capture(hw, "channels.rx\\d");
            capture(hw, "channels.tx\\d");
        }

        if ( (uint32_t)bgo != 0x0 ) {
            hw->getNode("ttc.ctrl.sendBGo").write(0x0);
            hw->dispatch();
        }
    }
    
private:
    
    void capture( uhal::HwInterface *hw, const std::string& regex ) {
        // Get the list of buffers
        std::vector<std::string> rxs = hw->getNodes(regex);
        ValMap modes, firsts, frames;

        BOOST_FOREACH(std::string& s, rxs) {
            // cout << s <<  endl;
            modes[s]  = hw->getNode(s).getNode("ctrl.mode").read();
            firsts[s] = hw->getNode(s).getNode("ctrl.firstBx").read(); 
            frames[s] = hw->getNode(s).getNode("ctrl.nFrames").read();
            }
        hw->dispatch();

        BOOST_FOREACH(ValMap::value_type& m, modes) {
            if ( m.second == swatch::processor::AbstractChanBuffer::Capture ) {
                // cout << m.first << " : " << m.second << endl;

                uint32_t bsize = hw->getNode(m.first).getNode("data").getSize();
                // write a vector of 32-bits randoms into the buffer
                std::vector<uint32_t> data(bsize);
                for( uint32_t i(0); i<bsize/2; ++i)
                    data[2*i] = flatDistribution_(randGenerator_);
                    // data[2*i+1] = 0x0;

                hw->getNode(m.first).getNode("data").writeBlock(data);
            }
        }
        hw->dispatch();        
    }

    boost::mt19937 randGenerator_;
    boost::random::uniform_int_distribution<uint32_t> flatDistribution_;
};

//---------------------------------------------------------------------------//
void reset(swatch::processor::Processor *p, const swatch::core::ParameterSet &params) {
    std::string clock = params.get<std::string>("clock");
    std::string ttc = params.get<std::string>("ttc");

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

    std::cout << ">> Is TTC enabled? " << p->ttc()->isEnabled() << std::endl;
    p->ttc()->clearCounters();

    // Check clock locked, BC0 lock


}

void configure(swatch::processor::Processor *p, const swatch::core::ParameterSet &params) {
    using namespace swatch::core;
    using namespace swatch::processor;
    // Claim exclusive use of the board
    // TODO

    // Based on reset
    // Standard soft reset procedure
    p->ctrl()->softReset();

    // Change clock configuration
    // Every board must have 'internal' and 'external' modes
    p->ctrl()->configureClock("internal");

    // Enable/disable ttc
    p->ttc()->configure("internal");

    // Check presence of TTC clock and signals
    if ( not p->ttc()->isClock40Locked() ) {
        cout << "Clock 40 NOT locked!" << endl;
    }
    if ( not p->ttc()->isOrbitLocked() ) {
        cout << "Orbit (BC0) NOT locked!" << endl;
    }

    // And clear all counters
    p->ttc()->clearCounters();

    // Configure input and output channels and buffers

    // Set MGTs up first
    // Rx channels
    BOOST_FOREACH( AbstractChannel* c, p->inputChannels() ) {
        // Reset
        // c->ctrl()->reset();
        // Configure
        // c->ctrl()->configure();
        // Clear
        // c->ctrl()->clearCRCs();
    }
    // Tx Channels 
    BOOST_FOREACH( AbstractChannel* c, p->outputChannels() ) {
        // Reset
        // c->ctrl()->reset();
        // Configure
        // c->ctrl()->configure();
    }
    
    // And then buffers
    BOOST_FOREACH( AbstractChannel* c, p->inputChannels() ) {
        c->buffer()->configure(AbstractChanBuffer::Latency);
    }
    BOOST_FOREACH( AbstractChannel* c, p->outputChannels() ) {
        c->buffer()->configure(AbstractChanBuffer::Latency);
    }


    // Configure board DAQ block
    

    // Configure board’s algorithm block with scalers and LUTs
    

    // Post-configuration checks (if needed)
    

    // Check links alignment (?)
}

//----------------------------------------------------------------------------//
void printStatus( std::ostream& out, swatch::processor::AbstractInfo* info ) {
    out << "Firmware version : 0x"  << std::hex << info->getFwVersion() << std::dec << endl;
    out << "Inputs  : " << info->getNInputs() << endl;
    out << "Outputs : " << info->getNOutputs() << endl;
} 

void printStatus( std::ostream& out, swatch::processor::AbstractTTC* ttc ) {
    out << "Clock40 Locked: " << (ttc->isClock40Locked() ? "True" : "False")  << endl;
    out << "Orbit Locked:   " << (ttc->isOrbitLocked() ? "True" : "False" )<< endl;
    out << "Event counter:  " << ttc->getEvtCount() << endl;
    out << "Bunch counter:  " << ttc->getBXCount() << endl;
    out << "Orbit counter:  " << ttc->getOrbitCount() << endl;
    out << "SBEC  counter:  " << ttc->getSingleBitErrors() << endl;
    out << "DBEC  counter:  " << ttc->getDoubleBitErrors() << endl;
} 

void printStatus( std::ostream& out, swatch::processor::Processor* p ) {
    out << "Processor " << p->id() << endl;
    out << ">> Info" << endl;
    printStatus(cout,  p->info() );
    out << ">> TTC" << endl;
    printStatus(cout,  p->ttc() );
}

//----------------------------------------------------------------------------//
int main(int argc, char const *argv[]) {
    using namespace swatch::processor;

    const std::string addrtab = "file://${SWATCH_ROOT}/processor/test/etc/dummy.xml";
    /* code */
    test::IPBusDummyHardware x("board-0", 50010, addrtab);
    x.start();
    std::cout << "Started? " << x.started() << std::endl;

    // Inject some data

    uint32_t nRx(4), nTx(4);

    test::IPBusDummyHardware::RegisterMap poweron;
    
    // 'Scalars'
    poweron["ctrl.id.fwrev"] = 0x44332211;
    poweron["ctrl.id.magic"] = 0xdeadc0de;
    poweron["ctrl.infos.nRx"] = nRx;
    poweron["ctrl.infos.nTx"] = nTx;
    poweron["ttc.ctrl"] = 0;
    poweron["ttc.stat"] = 0;
    poweron["ttc.counters"] = 0;
    poweron["ttc.counters1"] = 0;
    poweron["ttc.counters2"] = 0;
    poweron["ttc.counters3"] = 0;

    // Channel's array
    for ( size_t i(0); i<nRx; ++i) {
        std::string chpath = "channels.rx" + boost::lexical_cast<std::string>(i);
        poweron[chpath+".ctrl.mode"] = 0;
        poweron[chpath+".ctrl.firstBx"] = 0;
        poweron[chpath+".ctrl.nFrames"] = 0;
    }
    for ( size_t i(0); i<nTx; ++i) {
        std::string chpath = "channels.tx" + boost::lexical_cast<std::string>(i);
        poweron[chpath+".ctrl.mode"] = 0;
        poweron[chpath+".ctrl.firstBx"] = 0;
        poweron[chpath+".ctrl.nFrames"] = 0;
    }

    x.load(poweron);

    // Increment ttc counters
    x.add(new TTCCountersIncrementer(10, 10));
    x.add(new BufferCaptureEmulator(10));
    // Add error generator
    // x.add(new TTCErrorGenerator(10) );

    // x is ready for testing
    std::stringstream ssURI;
    ssURI << "ipbusudp-2.0://127.0.0.1:" << 50010;

    swatch::core::ParameterSet params;
    params.set("addrtab", addrtab);
    params.set("uri", ssURI.str());
    params.set("crate", "s2g20-10");
    params.set("slot", (uint32_t) 5);
    params.set("poweron", poweron);

    Processor *p0 = 0x0;
    try {
        p0 = new test::IPBusProcessor("test-board0", params);
        cout << ">> Processor " << p0->id() << " created" << endl;

    } catch (uhal::exception::exception &e) {
        // Crap, let's get out here
        return -1;
    }

    cout << "//_ Step 0 ___ Testing object __________________________________" << endl;
    printStatus(cout, p0);

    cout << "Input channels scan" << endl;
    // Test rx upload and download
    BOOST_FOREACH( AbstractChannel* in, p0->inputChannels() ) {        

        std::vector<uint64_t> v(in->buffer()->size(),0x5555);
        // cout << "v.size() = " << v.size() << endl;
        in->buffer()->upload(v);   

        // Download and check
        std::vector<uint64_t> data = in->buffer()->download();   
    
        cout << "v == data? : " << (v == data) << endl;
    }

    cout << "Output channels scan" << endl;
    // Test tx channels injection and 
    BOOST_FOREACH( AbstractChannel* out, p0->outputChannels() ) {        

        std::vector<uint64_t> v(out->buffer()->size(),0x5555);
        // cout << "v.size() = " << v.size() << endl;
        out->buffer()->upload(v);   

        // Download and check
        std::vector<uint64_t> data = out->buffer()->download();   
    
        cout << "v == data? : " << (v == data) << endl;
    }


    cout << "//_ Step 1 ___ Testing resets __________________________________" << endl;
    swatch::core::ParameterSet mode;
    mode.insert("clock", "internal")("ttc", "internal");

    
    cout << ">> Resetting on internal clock" << endl;
    reset(p0, mode);
    cout << ">> Take a nap (1 sec)" << endl;
    sleep(1);
    printStatus( cout, p0->ttc() );

    cout << ">> Sending 5 L1s" << endl;
    p0->ttc()->sendMultipleL1A(5);
    printStatus( cout, p0->ttc() );

    cout << "//_ Step 2 ___ Testing config __________________________________" << endl;
    swatch::core::ParameterSet config;
    configure(p0, config);

    // p0->inputChannel(0)->buffer()->configure(swatch::processor::AbstractChanBuffer::Capture, 0, 1);
    // p0->inputChannel(2)->buffer()->configure(swatch::processor::AbstractChanBuffer::Capture, 0, 1);

    cout << ">> Sending test BGo" << endl;
    p0->ttc()->sendBGo(0xc);

    cout << ">> Take a nap (1 sec)" << endl;
    sleep(1);

    const std::vector<swatch::processor::AbstractChannel*>& rxs = p0->inputChannels();
    std::vector< std::vector<uint64_t> > data(rxs.size());
    for ( size_t k(0); k < rxs.size(); ++k ) {
        data[k].swap(p0->inputChannel(k)->buffer()->download());
    }

    for ( size_t j(0); j<data[0].size(); ++j ) {
        cout << "Frame " <<  std::dec << std::setfill('0') << std::setw(4) << j << " : ";
        for ( size_t k(0); k<data.size(); ++k )
            cout << "0x" << std::hex << std::setfill('0') << std::setw(8) << data[k][j] << "   ";
        cout << endl;
    }
    cout << endl << std::dec;

    // std::vector<uint64_t> data = p0->inputChannel(2)->buffer()->download();

    cout << "//_ Destruction ________________________________________________" << endl;
    // delete p0;
    return 0;
}