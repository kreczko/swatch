/* 
 * File:   DummyProcessor.cpp
 * Author: mguilbau
 * 
 * Created on 18 juillet 2014, 11:17
 */

#include "uhal/ConnectionManager.hpp"

// Swatch Headers
#include "swatch/processor/test/IpbusProcessor.hpp"
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
IpbusProcessor::IpbusProcessor(const std::string& id, const swatch::core::Arguments& args) : Processor(id, args) {

    crate_ = args.get<std::string>("crate");
    slot_ = args.get<uint32_t>("slot");


    // Build the objects
    uhal::HwInterface interface = uhal::ConnectionManager::getDevice(
        id,
        args.get<std::string>("uri"),
        args.get<std::string>("addrtab")
        );

    
    connection_ = swatch::processor::Connection::make(interface);
    info_ = new IpbusInfo( connection_ );
    ctrl_ = new IpbusCtrl( connection_, args );
    ttc_ = new IpbusTTC( connection_ );
    
    // build the list of links based on the firmware informations
    uhal::ValWord<uint32_t> n_rx = interface.getNode("ctrl.infos.nRx").read();
    uhal::ValWord<uint32_t> n_tx = interface.getNode("ctrl.infos.nTx").read();
    interface.dispatch();
    
    //    return;
    // tx_.resize(nTx);
    // std::vector<AbstractChannel*>::iterator itTx;
    // for (itTx = tx_.begin(); itTx != tx_.end(); ++itTx) (*itTx) = new IpbusChannel(connection());

    // rx_.resize(nRx);
    // std::vector<AbstractChannel*>::iterator itRx;
    // for (itRx = rx_.begin(); itRx != rx_.end(); ++itRx) (*itRx) = new IpbusChannel(connection());

}

IpbusProcessor::~IpbusProcessor() {

    delete info_;
    delete ctrl_;
    delete ttc_;

    std::vector<AbstractChannel*>::iterator itTx;
    for (itTx = tx_.begin(); itTx != tx_.end(); ++itTx) delete (*itTx);
    tx_.clear();

    std::vector<AbstractChannel*>::iterator itRx;
    for (itRx = rx_.begin(); itRx != rx_.end(); ++itRx) delete (*itRx);
    rx_.clear();

}

const std::string& IpbusProcessor::getCrateId() const {
    return crate_;
}

uint32_t IpbusProcessor::getSlot() const {
    return slot_;
}


/*------------------------------------------------------------------------------
 * Dummy Info implementation
 */
IpbusInfo::IpbusInfo(swatch::processor::Connection* connection) : AbstractInfo(connection) {
    cout << "this is a very dummy info block" << endl;
}

IpbusInfo::~IpbusInfo() {
    cout << "dummy info block destroyed" << endl;
}

uint32_t
IpbusInfo::getFwVersion() {
    
//    uhal::HwInterface& hw = connection()->get<uhal::HwInterface>();
    uhal::ValWord<uint32_t> fwv = hw().getNode("ctrl.id.fwrev").read();
    hw().dispatch();
    return fwv.value() ;
}


/*------------------------------------------------------------------------------
 * Dummy Ctrl
 */
IpbusCtrl::IpbusCtrl(swatch::processor::Connection* connection, const swatch::core::Arguments& args) : AbstractCtrl(connection) {
    using namespace boost::assign;
    configs_ += "internal","external";

    poweron_ = args.get<RegisterMap>("poweron");
}

IpbusCtrl::~IpbusCtrl() {
}


void
IpbusCtrl::hardReset() {
    cout << "WARNING: Reloading firmware on the processor" << endl;
    sleep(1);
}

void
IpbusCtrl::softReset() {
    BOOST_FOREACH( RegisterMap::value_type p, poweron_) {
        hw().getNode(p.first).write(p.second);
    }
    hw().dispatch();
}

//void
//IpbusCtrl::clk40Reset() {
//    cout << "Resetting clock 40" << endl;
//}

void
IpbusCtrl::configureClock(const std::string& config ) {
    cout << "Configure clocking of the processor" << endl;
}

/*------------------------------------------------------------------------------
 * Dummy TTC
 */
IpbusTTC::IpbusTTC(swatch::processor::Connection* connection) : AbstractTTC(connection) {
//    cout << "this is a very dummy TTC interface" << endl;
    using namespace boost::assign;
    // 2 simple modes, internal and external
    configs_ += "internal","external";

}

IpbusTTC::~IpbusTTC() {
//    cout << "Our dummy TTC interface is destroyed" << endl;
}


void IpbusTTC::configure(const std::string& config) {
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
}


void
IpbusTTC::enable(bool enable) {
    hw().getNode("ttc.ctrl.enable").write(enable);
    hw().dispatch();
}

void
IpbusTTC::generateInternalOrbit(bool generate /* = true */) {
    hw().getNode("ttc.ctrl.genBC0").write(generate);
    hw().dispatch();
}

void
IpbusTTC::sendSingleL1A() {
    uhal::ValWord<uint32_t> evc  = hw().getNode("ttc.counters1.eventCntr").read();
    hw().dispatch();
    hw().getNode("ttc.counters1.eventCntr").write((uint32_t)evc+1);
    hw().dispatch();
}

void
IpbusTTC::sendMultipleL1A() {
    cout << "Sending multiple L1A" << endl;
    sleep(1);
    cout << "1...";
    sleep(1);
    cout << "2...";
    sleep(3);
    cout << "3..." << endl;
}

void
IpbusTTC::clearCounters() {
    hw().getNode("ttc.counters").write(0);
    hw().getNode("ttc.counters1").write(0);
    hw().getNode("ttc.counters2").write(0);
    hw().getNode("ttc.counters3").write(0);
    hw().dispatch();
}

void
IpbusTTC::clearErrCounters() {
    hw().getNode("ttc.counters3").write(0x0);
    hw().dispatch();
}

void
IpbusTTC::maskBC0Spy(bool mask /* = true */) {
    cout << "Mask BC0 from TTC history" << endl;
}

void
IpbusTTC::spy() {
    cout << "Capturing TTC..." << endl;
}

void
IpbusTTC::sendBGo(uint32_t command = 0) {
    cout << "Sending BGo with command: " << command << endl;
}

bool IpbusTTC::isEnabled() const {
    uhal::ValWord<uint32_t> enabled = hw().getNode("ttc.ctrl.enable").read();
    hw().dispatch();
    return (bool)enabled;
}

bool IpbusTTC::isGeneratingInternalBC0() const {
    uhal::ValWord<uint32_t> enabled = hw().getNode("ttc.ctrl.genBC0").read();
    hw().dispatch();
    return (bool)enabled;
}

bool IpbusTTC::isBC0SpyMasked() const {
    return false;
}


uint32_t
IpbusTTC::getBunchCount() const {
    uhal::ValWord<uint32_t> bxctr = hw().getNode("ttc.counters.bunchCntr").read();
    hw().dispatch();
    return (uint32_t)bxctr;
}

uint32_t
IpbusTTC::getEvtCount() const {
    uhal::ValWord<uint32_t> evctr = hw().getNode("ttc.counters1.eventCntr").read();
    hw().dispatch();
    return (uint32_t)evctr;
}

uint32_t
IpbusTTC::getOrbitCount() const {
    uhal::ValWord<uint32_t> octr = hw().getNode("ttc.counters2.orbitCntr").read();
    hw().dispatch();
    return (uint32_t)octr;
}

uint32_t
IpbusTTC::getSingleBitErrorCounter() const {
        uhal::ValWord<uint32_t> sbec = hw().getNode("ttc.counters3.singleBitErrCntr").read();
        hw().dispatch();
        return (uint32_t) sbec;
}

uint32_t
IpbusTTC::getDoubleBitErrorCounter() const {
        uhal::ValWord<uint32_t> dbec = hw().getNode("ttc.counters3.singleBitErrCntr").read();
        hw().dispatch();
        return (uint32_t) dbec;
}

//void 
//DummyTTC::getTTChistory() const {
//
//    cout << "Getting TTC history" << endl;
//
//}


bool
IpbusTTC::isClock40Locked() const {

    cout << "Getting clock 40 lock..." << endl;
    bool clk40lock = 0;
    return clk40lock;

}

bool
IpbusTTC::hasClock40Stopped() const {

    cout << "Getting clock 40 stopped..." << endl;
    bool clk40stopped = 0;
    return clk40stopped;

}

bool
IpbusTTC::isBC0Locked() const {

    cout << "Getting BC0 lock..." << endl;
    bool BC0lock = 0;
    return BC0lock;

}

bool
IpbusTTC::hasBC0Stopped() const {

    cout << "Getting BC0 stopped..." << endl;
    bool BC0stopped = 0;
    return BC0stopped;

}

/**
 */
IpbusChannel::IpbusChannel(swatch::processor::Connection* connection) : AbstractChannel(connection) {
    cout << "this is a very dummy channel interface" << endl;
    ctrl_ = new IpbusChanCtrl(connection);
    buffer_ = new IpbusChanBuffer(connection);
}

IpbusChannel::~IpbusChannel() {
    delete ctrl_;
    delete buffer_;
    cout << "Our dummy channel is destroyed" << endl;
}

/*------------------------------------------------------------------------------
 * Dummy Channel Control
 */
IpbusChanCtrl::IpbusChanCtrl(swatch::processor::Connection* connection) : AbstractChanCtrl(connection) {
    cout << "this is a very dummy channel control interface" << endl;
}

IpbusChanCtrl::~IpbusChanCtrl() {
    cout << "Our dummy channel control is destroyed" << endl;
}

void
IpbusChanCtrl::reset() {
    cout << "Reset the channel..." << endl;
    sleep(1);
}

void
IpbusChanCtrl::setLoopback() {
    cout << "set loopback mode" << endl;
}

void
IpbusChanCtrl::resetCRCcounts() {
    cout << "reset CRC counters" << endl;
}

uint32_t
IpbusChanCtrl::getCRCcounts() {
    cout << "getting CRC counters" << endl;
    uint32_t CRCcntr = 1;
    return CRCcntr;
}

uint32_t
IpbusChanCtrl::getCRCErrcounts() {
    cout << "getting CRC errors counters" << endl;
    uint32_t CRCerrCntr = 1;
    return CRCerrCntr;
}

bool
IpbusChanCtrl::isPLLLocked() {
    cout << "getting PLL lock status" << endl;
    bool PLLlock = true;
    return PLLlock;
}

bool
IpbusChanCtrl::isSync() {

    cout << "getting synchronistation status" << endl;
    bool sync = true;
    return sync;

}

void
IpbusChanCtrl::configure(const swatch::core::ParameterSet& pset) {
    cout << "configure the channel control with a given set of parameters " << endl;
}

/*------------------------------------------------------------------------------
 * Dummy Channel Buffer 
 */
IpbusChanBuffer::IpbusChanBuffer(swatch::processor::Connection* connection) : AbstractChanBuffer(connection) {
    cout << "this is a very dummy channel buffer interface" << endl;

//    uhal::HwInterface dummyhw = he;

    uhal::ValWord<uint32_t> bsize = hw().getNode("buffer.stat.size").read();
    hw().dispatch();
    
    bufferSize_ = bsize;
        
    cout << "A dummy buffer was built with a size: " << dec << getBufferSize() << endl;
}

IpbusChanBuffer::~IpbusChanBuffer() {
    cout << "Our dummy channel buffer is destroyed" << endl;
}


void
IpbusChanBuffer::configure(BufferMode aMode, uint32_t aFirstBx = 0, uint32_t aLastBx = 0) {
    cout << "Configure the buffer in the following mode: " << endl;

    switch (aMode) {
        case 0:
            cout << "Buffer mode configuration: Latency" << endl;
            break;
        case 1:
            cout << "Buffer mode configuration: Playback" << endl;
            break;
        case 2:
            cout << "Buffer mode configuration: Pattern" << endl;
            break;
        case 3:
            cout << "Buffer mode configuration: Capture" << endl;
            break;
        case 4:
            cout << "Buffer mode configuration: Zeros" << endl;
            break;
        default:
            cout << "Buffer configure with non-valid mode" << endl;
            break;
    }

    cout << "Selected range is: bx = [ "  << std::dec << aFirstBx << " , " << aLastBx << " ]" << endl;
}

std::vector<uint64_t>
IpbusChanBuffer::download() {
    cout << "Downloading the payload " << &payload_ << endl;
    return payload_;
}

void
IpbusChanBuffer::upload(const std::vector<uint64_t>& aPayload) {
    cout << "Uploading the payload ---> " << &aPayload << endl;
    sleep(1);
    payload_ = aPayload;
}

}
}
}

