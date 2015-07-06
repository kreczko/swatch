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


// IPBusProcessor Headers
#include "swatch/processor/test/IPBusDummyHardware.hpp"
#include "swatch/processor/test/IPBusProcessor.hpp"
#include "swatch/processor/test/BufferInterface.hpp"

// Swatch Headers
#include "swatch/processor/LinkInterface.hpp"
#include "swatch/core/xoperators.hpp"
#include "swatch/logger/Log.hpp"
#include "swatch/processor/ProcessorStub.hpp"
#include "swatch/processor/TTCInterface.hpp"
#include "swatch/core/test/DummyCommand.hpp"

// Boost Headers
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/uniform_real_distribution.hpp>

// XDAQ Headers
#include "xdata/Table.h"
#include "xdata/TableIterator.h"
#include "swatch/processor/test/IPBusRxChannel.hpp"
#include "swatch/processor/test/IPBusTxChannel.hpp"


// Namespace resolution
using std::cout;
using std::endl;

namespace swlog = swatch::logger;
namespace swpro = swatch::processor;
namespace swco = swatch::core;
namespace swct = swatch::core::test;

//---
struct TTCCountersIncrementer : public swpro::test::IPBusWorkLoop {

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
struct TTCErrorGenerator : public swpro::test::IPBusWorkLoop {

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
struct BufferCaptureEmulator : public swpro::test::IPBusWorkLoop {

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
            if ( m.second == swpro::test::BufferInterface::Capture ) {
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


void printStatus( swpro::TTCInterface* ttc ) {
/*    LOG(swlog::kInfo) << "Clock40 Locked: " << (ttc->isClock40Locked() ? "True" : "False");
    LOG(swlog::kInfo) << "Orbit Locked:   " << (ttc->isBC0Locked() ? "True" : "False" );
    LOG(swlog::kInfo) << "Event counter:  " << ttc->getEventCounter();
    LOG(swlog::kInfo) << "Bunch counter:  " << ttc->getBunchCounter();
    LOG(swlog::kInfo) << "Orbit counter:  " << ttc->getOrbitCounter();
    LOG(swlog::kInfo) << "SBEC  counter:  " << ttc->getSingleBitErrors();
    LOG(swlog::kInfo) << "DBEC  counter:  " << ttc->getDoubleBitErrors();
*/} 

void printStatus( swpro::Processor* p ) {
    LOG(swlog::kInfo) << "Processor " << p->id();
    LOG(swlog::kInfo) << ">> Info";
//    LOG(swlog::kInfo) << "Firmware version : 0x"  << std::hex << p->firmwareVersion() << std::dec;
    LOG(swlog::kInfo) << "Inputs  : " << p->linkInterface().getNumInputs();
    LOG(swlog::kInfo) << "Outputs : " << p->linkInterface().getNumOutputs();
    LOG(swlog::kInfo) << ">> TTC";
    printStatus( & p->ttc() );
}


//----------------------------------------------------------------------------//
int main(int argc, char const *argv[]) {
    // using namespace swatch::processor;
  
    swlog::Log::setLogThreshold(swlog::kDebug);
    
    const std::string addrtab = "file://${SWATCH_ROOT}/processor/test/etc/swatch/processor/test/dummy.xml";
    /* code */
    swpro::test::IPBusDummyHardware x("board-0", 50010, addrtab);
    x.start();
    LOG(swlog::kNotice) << "Started? " << x.started();

    // Inject some data

    uint32_t nRx(4), nTx(2);
    
    swco::XParameterSet xpoweron;

    // 'Scalars'
    xpoweron.add("ctrl.id.fwrev", xdata::UnsignedInteger(0x44332211) );
    xpoweron.add("ctrl.id.magic", xdata::UnsignedInteger(0xdeadc0de) );
    xpoweron.add("ctrl.infos.nRx", xdata::UnsignedInteger(nRx) );
    xpoweron.add("ctrl.infos.nTx", xdata::UnsignedInteger(nTx) );
    xpoweron.add("ttc.ctrl", xdata::UnsignedInteger(0) );
    xpoweron.add("ttc.stat", xdata::UnsignedInteger(0) );
    xpoweron.add("ttc.counters", xdata::UnsignedInteger(0) );
    xpoweron.add("ttc.counters1", xdata::UnsignedInteger(0) );
    xpoweron.add("ttc.counters2", xdata::UnsignedInteger(0) );
    xpoweron.add("ttc.counters3", xdata::UnsignedInteger(0) );

    // Channel's array
    for ( size_t i(0); i<nRx; ++i) {
        std::string chpath = "channels.rx" + boost::lexical_cast<std::string>(i);
        xpoweron.add( chpath+".ctrl.mode", xdata::UnsignedInteger(0) );
        xpoweron.add( chpath+".ctrl.firstBx", xdata::UnsignedInteger(0) );
        xpoweron.add( chpath+".ctrl.nFrames", xdata::UnsignedInteger(0) );
    }
    for ( size_t i(0); i<nTx; ++i) {
        std::string chpath = "channels.tx" + boost::lexical_cast<std::string>(i);
        xpoweron.add( chpath+".ctrl.mode", xdata::UnsignedInteger(0) );
        xpoweron.add( chpath+".ctrl.firstBx", xdata::UnsignedInteger(0) );
        xpoweron.add( chpath+".ctrl.nFrames", xdata::UnsignedInteger(0) );
    }    


    x.load(xpoweron);
    
     // Increment ttc counters
    x.add(new TTCCountersIncrementer(10, 10));
    x.add(new BufferCaptureEmulator(10));
    // Add error generator
    // x.add(new TTCErrorGenerator(10) );

        
    swpro::ProcessorStub stubTemplate("");
    stubTemplate.creator = "swatch::processor::test::IPBusProcessor";
    stubTemplate.addressTable = addrtab;
    stubTemplate.uri = "";
    stubTemplate.crate = "s2g20-10";
    stubTemplate.slot = 0;
    
    
    // x is ready for testing
    swpro::ProcessorStub p0stub(stubTemplate);
    p0stub.id = "Processor 0";
    
    std::stringstream ssURI;
    ssURI << "ipbusudp-2.0://127.0.0.1:" << 50010;
    p0stub.uri = ssURI.str();

    swatch::core::XParameterSet empty_params;
//    swatch::core::XParameterSet params;
//    params.add("name", xdata::String("Processor 0"));
//    params.add("class", p0bag.creator);
//    params.add("stub",p0bag);
//    params.add("poweron", xpoweron);


    // Fun starts here
    swpro::Processor *p0 = 0x0;
    try {
        p0 = new swpro::test::IPBusProcessor(p0stub);
        LOG(swlog::kNotice) << ">> Processor " << p0->id() << " created";

    } catch (uhal::exception::exception &e) {
        // Crap, let's get out here
        return -1;
    }
    
    LOG(swlog::kNotice) << "//_ Step 0 ___ Testing object __________________________________";
    printStatus(p0);

    LOG(swlog::kDebug) << "Input channels scan";
    // Test rx upload and download
    BOOST_FOREACH( swpro::InputPort* in, p0->linkInterface().getInputs() ) {        
        swpro::test::IPBusRxChannel* rx = dynamic_cast<swpro::test::IPBusRxChannel*>(in);
        std::vector<uint64_t> v(rx->getBufferSize(),0x5555);
        // cout << "v.size() = " << v.size() << endl;
        rx->upload(v);   

        // Download and check
        std::vector<uint64_t> data = rx->download();   
    
        LOG(swlog::kDebug) << "v == data? : " << (v == data);
    }

    LOG(swlog::kDebug) << "Output channels scan";
    // Test tx channels injection and 
    BOOST_FOREACH( swpro::OutputPort* out, p0->linkInterface().getOutputs() ) {        
        swpro::test::IPBusTxChannel* tx = dynamic_cast<swpro::test::IPBusTxChannel*>(out);

        std::vector<uint64_t> v(tx->getBufferSize(),0x5555);
        // cout << "v.size() = " << v.size() << endl;
        tx->upload(v);   

        // Download and check
        std::vector<uint64_t> data = tx->download();   
    
        LOG(swlog::kDebug) << "v == data? : " << (v == data);
    }

    LOG(swlog::kNotice) << "//_ Step 1 ___ Testing resets __________________________________";
    swatch::core::XParameterSet mode;
    
    LOG(swlog::kNotice) << ">> Resetting on internal clock";
    
    swct::DummyCommand* reset = (swct::DummyCommand*) p0->getCommand("reset");
    reset->registerParameter("mode", xdata::String("internal"));

    BOOST_FOREACH( const std::string& s, reset->getDefaultParams().keys() ) {
      LOG(swlog::kInfo) << " - " << s << ": " << reset->getDefaultParams()[s];
    }

    reset->exec(empty_params);
    do {
    } while ( (reset->getState() == swatch::core::Command::kScheduled) || (reset->getState() == swatch::core::Command::kRunning) );

    if ( reset->getState() == swco::Command::kError ) {  
      LOG(swlog::kError) << reset->getStatus().getStatusMsg();
      return -1;
    }
    
    LOG(swlog::kNotice) << ">> Take a nap (1 sec)";
    sleep(1);
    printStatus( & p0->ttc() );

    
    LOG(swlog::kNotice) << "//_ Step 2 ___ Testing config __________________________________";

    
    swct::DummyCommand* cfgCmd = (swct::DummyCommand*) p0->getCommand("configure");
    cfgCmd->registerParameter("mode", xdata::String("capture"));
    BOOST_FOREACH( const std::string& s, cfgCmd->getDefaultParams().keys() ) {
      LOG(swlog::kInfo) << " - " << s << ": " << cfgCmd->getDefaultParams()[s];
    }
    cfgCmd->exec(empty_params);
    do {
    } while ( (reset->getState() == swatch::core::Command::kScheduled) || (reset->getState() == swatch::core::Command::kRunning) );

    if ( cfgCmd->getState() == swco::Command::kError ) {  
      LOG(swlog::kError) << cfgCmd->getStatus().getStatusMsg();
      return -1;
    }
    
    p0->getCommand("capture")->exec(empty_params);

    LOG(swlog::kNotice) << "//_ Destruction ________________________________________________";
    

    delete p0;
    
    return 0;

}
