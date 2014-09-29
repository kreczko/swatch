/* 
 * File:   DummyProcessor.cpp
 * Author: mguilbau
 * 
 * Created on 18 juillet 2014, 11:17
 */

#include "uhal/ConnectionManager.hpp"

// Swatch Headers
#include "swatch/processor/test/IPBusProcessor.hpp"
#include "swatch/processor/Connection.hpp"

// Boost Headers
#include <boost/assign.hpp>
#include <boost/foreach.hpp>

// C++ Headers
#include <iomanip>

// Namespace Resolution
using std::cout;
using std::endl;
using std::hex;
using std::dec;


namespace swatch {
namespace processor {
namespace test {

/*------------------------------------------------------------------------------
 * Dummy Processor implementation
 */
IPBusProcessor::IPBusProcessor(const std::string& id, const swatch::core::ParameterSet& params) : Processor(id, params) {
    using namespace swatch::core;
    crate_ = params.get<std::string>("crate");
    slot_ = params.get<uint32_t>("slot");


    // Build the objects
    uhal::HwInterface interface = uhal::ConnectionManager::getDevice(
        id,
        params.get<std::string>("uri"),
        params.get<std::string>("addrtab")
        );

    
    connection_ = swatch::processor::Connection::make(interface);
    info_ = new IPBusInfo( connection_ );
    ctrl_ = new IPBusCtrl( connection_, params );
    ttc_ = new IPBusTTC( connection_ );
    
    // build the list of links based on the firmware informations
    uint32_t nInputs  = info()->getNInputs();
    uint32_t nOutputs = info()->getNOutputs();

    inputChannels_.reserve(nInputs);
    for (size_t k(0); k<nInputs; ++k) {
        ParameterSet a, ctrl, buf;
        std::string path = "channels.rx"+boost::lexical_cast<std::string>(k);
        
        ctrl.insert("path",path);
        buf.insert("path",path);
        a.insert("ctrl",ctrl)("buffer",buf);

        inputChannels_.push_back(new IpbusChannel(connection_, a));
    }

    outputChannels_.reserve(nOutputs);
    for (size_t k(0); k<nOutputs; ++k) {
        ParameterSet a, ctrl, buf;
        std::string path = "channels.tx"+boost::lexical_cast<std::string>(k);
        
        ctrl.insert("path",path); // <- this is wrong
        buf.insert("path",path);
        a.insert("ctrl",ctrl)("buffer",buf);

        outputChannels_.push_back(new IpbusChannel(connection_, a));
    }

}

IPBusProcessor::~IPBusProcessor() {

    delete info_;
    delete ctrl_;
    delete ttc_;

    std::vector<AbstractChannel*>::iterator itTx;
    for (itTx = inputChannels_.begin(); itTx != inputChannels_.end(); ++itTx) delete (*itTx);
    inputChannels_.clear();

    std::vector<AbstractChannel*>::iterator itRx;
    for (itRx = outputChannels_.begin(); itRx != outputChannels_.end(); ++itRx) delete (*itRx);
    outputChannels_.clear();

}

const std::string& IPBusProcessor::getCrateId() const {
    return crate_;
}

uint32_t IPBusProcessor::getSlot() const {
    return slot_;
}


/*------------------------------------------------------------------------------
 * Dummy Info implementation
 */
IPBusInfo::IPBusInfo(swatch::processor::Connection* connection) : AbstractInfo(connection) {
    cout << "this is a very dummy info block" << endl;
}

IPBusInfo::~IPBusInfo() {
    cout << "dummy info block destroyed" << endl;
}

uint32_t
IPBusInfo::getFwVersion() {
    uhal::ValWord<uint32_t> fwv = hw()->getNode("ctrl.id.fwrev").read();
    hw()->dispatch();
    return fwv.value() ;
}

uint32_t 
IPBusInfo::getNInputs() {
    uhal::ValWord<uint32_t> n_rx = hw()->getNode("ctrl.infos.nRx").read();
    hw()->dispatch();
    return (uint32_t) n_rx;
}

uint32_t 
IPBusInfo::getNOutputs() {
    uhal::ValWord<uint32_t> n_tx = hw()->getNode("ctrl.infos.nTx").read();
    hw()->dispatch();
    return (uint32_t) n_tx;
}


/*------------------------------------------------------------------------------
 * Dummy Ctrl
 */
IPBusCtrl::IPBusCtrl(swatch::processor::Connection* connection, const swatch::core::ParameterSet& params) : AbstractCtrl(connection) {
    using namespace boost::assign;
    configs_ += "internal","external";

    poweron_ = params.get<RegisterMap>("poweron");
}

IPBusCtrl::~IPBusCtrl() {
}


void
IPBusCtrl::hardReset() {
    cout << "WARNING: Reloading firmware on the processor" << endl;
    sleep(1);
}

void
IPBusCtrl::softReset() {
    BOOST_FOREACH( RegisterMap::value_type& p, poweron_) {
        hw()->getNode(p.first).write(p.second);
    }
    hw()->dispatch();
}

//void
//IpbusCtrl::clk40Reset() {
//    cout << "Resetting clock 40" << endl;
//}

void
IPBusCtrl::configureClock(const std::string& config ) {
    // Let's assume it works and set
    hw()->getNode("ttc.stat.clk40Locked").write(true);
    hw()->dispatch();

}

/*------------------------------------------------------------------------------
 * Dummy TTC
 */
IPBusTTC::IPBusTTC(swatch::processor::Connection* connection) : AbstractTTC(connection) {
//    cout << "this is a very dummy TTC interface" << endl;
    using namespace boost::assign;
    // 2 simple modes, internal and external
    configs_ += "internal","external";

}

IPBusTTC::~IPBusTTC() {
//    cout << "Our dummy TTC interface is destroyed" << endl;
}


void IPBusTTC::configure(const std::string& config) {
    // Kind of an overkill, but let's try to do things properly
    if ( configs_.find(config) == configs_.end() )
        throw std::logic_error("Configuration "+config+" not found.");

    if ( config == "internal" ) {
        this->enable(false);
        this->generateInternalOrbit(true);
    } else if ( config == "extenal" ) {
        this->enable(true);
        this->generateInternalOrbit(false);  
    }

    hw()->getNode("ttc.stat.bc0Locked").write(true);
    hw()->dispatch();
}


void
IPBusTTC::enable(bool enable) {
    hw()->getNode("ttc.ctrl.enable").write(enable);
    hw()->dispatch();
}

void
IPBusTTC::generateInternalOrbit(bool generate /* = true */) {
    hw()->getNode("ttc.ctrl.genBC0").write(generate);
    hw()->dispatch();
}

void
IPBusTTC::sendSingleL1A() {
    uhal::ValWord<uint32_t> evc  = hw()->getNode("ttc.counters1.eventCntr").read();
    hw()->dispatch();
    hw()->getNode("ttc.counters1.eventCntr").write((uint32_t)evc+1);
    hw()->dispatch();
}

void
IPBusTTC::sendMultipleL1A(uint32_t nL1A) {
    uhal::ValWord<uint32_t> evc  = hw()->getNode("ttc.counters1.eventCntr").read();
    hw()->dispatch();
    hw()->getNode("ttc.counters1.eventCntr").write((uint32_t)evc+nL1A);
    hw()->dispatch();
}

void
IPBusTTC::clearCounters() {
    hw()->getNode("ttc.counters").write(0);
    hw()->getNode("ttc.counters1").write(0);
    hw()->getNode("ttc.counters2").write(0);
    hw()->getNode("ttc.counters3").write(0);
    hw()->dispatch();
}

void
IPBusTTC::clearErrors() {
    hw()->getNode("ttc.counters3").write(0x0);
    hw()->dispatch();
}

void
IPBusTTC::maskBC0Spy(bool mask /* = true */) {
    cout << "Mask BC0 from TTC history" << endl;
}

void
IPBusTTC::spy() {
    cout << "Capturing TTC..." << endl;
}

void
IPBusTTC::sendBGo(uint32_t command = 0) {
    hw()->getNode("ttc.ctrl.sendBGo").write(command);
    hw()->dispatch();
}

bool IPBusTTC::isEnabled() const {
    uhal::ValWord<uint32_t> enabled = hw()->getNode("ttc.ctrl.enable").read();
    hw()->dispatch();
    return (bool)enabled;
}

bool IPBusTTC::isGeneratingInternalBC0() const {
    uhal::ValWord<uint32_t> enabled = hw()->getNode("ttc.ctrl.genBC0").read();
    hw()->dispatch();
    return (bool)enabled;
}

bool IPBusTTC::isBC0SpyMasked() const {
    return false;
}


uint32_t
IPBusTTC::getBXCount() const {
    uhal::ValWord<uint32_t> bxctr = hw()->getNode("ttc.counters.bunchCntr").read();
    hw()->dispatch();
    return (uint32_t)bxctr;
}

uint32_t
IPBusTTC::getEvtCount() const {
    uhal::ValWord<uint32_t> evctr = hw()->getNode("ttc.counters1.eventCntr").read();
    hw()->dispatch();
    return (uint32_t)evctr;
}

uint32_t
IPBusTTC::getOrbitCount() const {
    uhal::ValWord<uint32_t> octr = hw()->getNode("ttc.counters2.orbitCntr").read();
    hw()->dispatch();
    return (uint32_t)octr;
}

uint32_t
IPBusTTC::getSingleBitErrors() const {
    uhal::ValWord<uint32_t> sbec = hw()->getNode("ttc.counters3.singleBitErrCntr").read();
    hw()->dispatch();
    return (uint32_t) sbec;
}

uint32_t
IPBusTTC::getDoubleBitErrors() const {
    uhal::ValWord<uint32_t> dbec = hw()->getNode("ttc.counters3.singleBitErrCntr").read();
    hw()->dispatch();
    return (uint32_t) dbec;
}

//void 
//DummyTTC::getTTChistory() const {
//
//    cout << "Getting TTC history" << endl;
//
//}


bool
IPBusTTC::isClock40Locked() const {

    uhal::ValWord<uint32_t> locked = hw()->getNode("ttc.stat.clk40Locked").read();
    hw()->dispatch();
    return (bool) locked;

}

bool
IPBusTTC::hasClock40Stopped() const {

    // cout << "Getting clock 40 stopped..." << endl;
    bool clk40stopped = 0;
    return clk40stopped;

}

bool
IPBusTTC::isOrbitLocked() const {

    uhal::ValWord<uint32_t> locked = hw()->getNode("ttc.stat.bc0Locked").read();
    hw()->dispatch();
    return (bool) locked;

}

bool
IPBusTTC::hasBC0Stopped() const {

    // cout << "Getting BC0 stopped..." << endl;
    bool BC0stopped = 0;
    return BC0stopped;

}

/*------------------------------------------------------------------------------
 * IPBus Channel
 */
IpbusChannel::IpbusChannel(swatch::processor::Connection* connection, const swatch::core::ParameterSet& params) : AbstractChannel(connection) {
    using namespace swatch::core;
    const ParameterSet& chanParams = params.get<ParameterSet>("ctrl");
    ctrl_ = new IPBusChanCtrl(connection, chanParams);
    const ParameterSet& bufParams = params.get<ParameterSet>("buffer");
    buffer_ = new IPBusChanBuffer(connection, bufParams);
}

IpbusChannel::~IpbusChannel() {
    delete ctrl_;
    delete buffer_;
    cout << "Our dummy channel is destroyed" << endl;
}

/*------------------------------------------------------------------------------
 * Dummy Channel Control
 */
IPBusChanCtrl::IPBusChanCtrl(swatch::processor::Connection* connection, const swatch::core::ParameterSet& params) : AbstractChanCtrl(connection) {
    cout << "this is a very dummy channel control interface" << endl;
}

IPBusChanCtrl::~IPBusChanCtrl() {
    cout << "Our dummy channel control is destroyed" << endl;
}

void
IPBusChanCtrl::reset() {
    cout << "Reset the channel..." << endl;
    sleep(1);
}

void
IPBusChanCtrl::setLoopback() {
    cout << "set loopback mode" << endl;
}

void
IPBusChanCtrl::resetCRCCounts() {
    cout << "reset CRC counters" << endl;
}

uint32_t
IPBusChanCtrl::getCRCCounts() {
    cout << "getting CRC counters" << endl;
    uint32_t CRCcntr = 1;
    return CRCcntr;
}

uint32_t
IPBusChanCtrl::getCRCErrorCounts() {
    cout << "getting CRC errors counters" << endl;
    uint32_t CRCerrCntr = 1;
    return CRCerrCntr;
}

bool
IPBusChanCtrl::isPLLLocked() {
    cout << "getting PLL lock status" << endl;
    bool PLLlock = true;
    return PLLlock;
}

bool
IPBusChanCtrl::isSync() {

    cout << "getting synchronization status" << endl;
    bool sync = true;
    return sync;

}

void
IPBusChanCtrl::configure(const swatch::core::ParameterSet& params) {
    cout << "configure the channel control with a given set of parameters " << endl;
}

/*------------------------------------------------------------------------------
 * Dummy Channel Buffer 
 */
IPBusChanBuffer::IPBusChanBuffer(swatch::processor::Connection* connection, const swatch::core::ParameterSet& params) : AbstractChanBuffer(connection) {
//    cout << "this is a very dummy channel buffer interface" << endl;
    
    std::string path = params.get<std::string>("path");
    // cout << ">>>>> path: " << path << endl;
    ctrlpath_ = path+".ctrl";
    datapath_ = path+".data";
    bufferSize_ = hw()->getNode(datapath_).getSize()/2.;
        
    // BOOST_FOREACH( const std::string& n, hw()->getNode(ctrlpath_).getNodes()) {
    //     cout << " * " << n << endl;
    // }
    cout << "IPBus buffer was built with a size: " << dec << size() << endl;
}

IPBusChanBuffer::~IPBusChanBuffer() {
    cout << "Our dummy channel buffer is destroyed" << endl;
}


void
IPBusChanBuffer::configure(BufferMode mode, uint32_t firstBx = 0, uint32_t frames = 0) {

    // Check for overflows

    cout << "Selected range is: bx = "  << std::dec << firstBx << " , frames = " << frames << endl;

    const uhal::Node& ctrl = hw()->getNode(ctrlpath_);
    ctrl.getNode("mode").write(mode);
    ctrl.getNode("firstBx").write(firstBx);
    ctrl.getNode("nFrames").write(frames);

    hw()->dispatch();
}

std::vector<uint64_t>
IPBusChanBuffer::download() {
    std::vector<uint64_t> data(bufferSize_);
    
    uhal::ValVector<uint32_t> block = hw()->getNode(datapath_).readBlock(2 * bufferSize_);
    hw()->dispatch();

    for( size_t k(0); k<data.size(); ++k) {
        data[k] = block[ 2 * k ];
        // cast to 64bits and shift by 32
        data[k] += ((uint64_t) block[2 * k + 1]) << 32;
    }
    return data;
}

void
IPBusChanBuffer::upload(const std::vector<uint64_t>& data) {
    
    // Need to mangle the 64 bits into 2x32 bits words to make it more realistic
    std::vector<uint32_t> block;
    block.reserve(2 * data.size());

    // std::vector<uint64_t>::const_iterator it;
    BOOST_FOREACH( const uint64_t& x, data ) {
        block.push_back( x & 0xffffffff );
        block.push_back( ( x>>32 ) & 0xffffffff );
    } 

    hw()->getNode(datapath_).writeBlock(block);
    hw()->dispatch();
}

}
}
}

