/* 
 * File:   DummyProcessor.cpp
 * Author: mguilbau
 * 
 * Created on 18 juillet 2014, 11:17
 */
// OTHER HEADERS
#include "uhal/ConnectionManager.hpp"

// SWATCH HEADERS
#include "swatch/processor/test/IpbusProcessor.hpp"
#include "swatch/processor/Connection.hpp"

#include <iomanip>

// Namespace Resolution
using std::cout;
using std::endl;
using std::hex;
using std::dec;


namespace swatch {
namespace processor {
namespace test {

/**
 * Dummy Processor implementation
 */
IpbusProcessor::IpbusProcessor(const std::string& id, const swatch::core::ParameterSet& pset) : Processor(id, pset) {

    uhal::HwInterface dummyhw = uhal::ConnectionManager("file://${SWATCH_TEST}/etc/connections.xml").getDevice("DummyProcessor");

    uhal::ValWord<uint32_t> magic = dummyhw.getNode("ctrl.id.magic").read();
    dummyhw.dispatch();

    std::cout << "Magic Number: 0x" << std::hex << magic.value() << std::endl;

    int nTx = pset.get<int>("nTx");
    int nRx = pset.get<int>("nRx");

    std::cout << "nTx=" << std::dec << nTx << "   nRx=" << nRx << std::endl;

    connection_ = swatch::processor::Connection::make(dummyhw);
    ctrl_ = new IpbusCtrl(connection());
    ttc_ = new IpbusTTC(connection());

    //    return;
    tx_.resize(nTx);
    std::vector<AbstractChannel*>::iterator itTx;
    for (itTx = tx_.begin(); itTx != tx_.end(); ++itTx) (*itTx) = new IpbusChannel(connection());

    rx_.resize(nRx);
    std::vector<AbstractChannel*>::iterator itRx;
    for (itRx = rx_.begin(); itRx != rx_.end(); ++itRx) (*itRx) = new IpbusChannel(connection());

}

IpbusProcessor::~IpbusProcessor() {

    delete ctrl_;
    delete ttc_;

    std::vector<AbstractChannel*>::iterator itTx;
    for (itTx = tx_.begin(); itTx != tx_.end(); ++itTx) delete (*itTx);
    tx_.clear();

    std::vector<AbstractChannel*>::iterator itRx;
    for (itRx = rx_.begin(); itRx != rx_.end(); ++itRx) delete (*itRx);
    rx_.clear();

}

/**
 * Dummy Info implementation
 */
IpbusInfo::IpbusInfo(swatch::processor::Connection* connection) : AbstractInfo(connection) {
    cout << "this is a very dummy info block" << endl;

    uhal::HwInterface& dummyhw = this->connection()->get<uhal::HwInterface>();

    uhal::ValWord<uint32_t> fwv   = dummyhw.getNode("ctrl.id.fwrev").read();
    uhal::ValWord<uint32_t> magic = dummyhw.getNode("ctrl.id.magic").read();
    dummyhw.dispatch();
    
    fwversion_ = fwv.value() ;
    magicNumber_ =  magic.value() ;
    cout << "A dummy info block was built with: " << endl;
    cout << "   - fw version  : 0x" << hex << fwVersion()   << endl;
    cout << "   - magic number: 0x" << hex << magicNumber() << endl;    
}

IpbusInfo::~IpbusInfo() {
    cout << "dummy info block destroyed" << endl;
}

/**
 * Dummy Ctrl
 */
IpbusCtrl::IpbusCtrl(swatch::processor::Connection* connection) : AbstractCtrl(connection) {
    cout << "this is a very dummy control block" << endl;
}

IpbusCtrl::~IpbusCtrl() {
    cout << "dummy control block destroyed" << endl;
}

void
IpbusCtrl::hardReset() {
    cout << "WARNING: Reloading firmware on the processor" << endl;
    sleep(1);
}

void
IpbusCtrl::softReset() {
    cout << "Resetting processor registers to startup values" << endl;
}

void
IpbusCtrl::clk40Reset() {
    cout << "Resetting clock 40" << endl;
}

void
IpbusCtrl::configureClk(const swatch::core::ParameterSet& pset) {
    cout << "Configure clocking of the processor" << endl;
}


/**
 * Dummy TTC
 */
IpbusTTC::IpbusTTC(swatch::processor::Connection* connection) : AbstractTTC(connection) {
    cout << "this is a very dummy TTC interface" << endl;
}

IpbusTTC::~IpbusTTC() {
    cout << "Our dummy TTC interface is destroyed" << endl;
}

//functionality

void
IpbusTTC::enableTTC() {
    cout << "enable TTC" << endl;
}

void
IpbusTTC::enableBC0() {
    cout << "enable BC0" << endl;
}

void
IpbusTTC::sendSingleL1A() {
    cout << "Sending a single L1A" << endl;
    sleep(1);
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
    cout << "Clear all the counters" << endl;
}

void
IpbusTTC::clearErrCounters() {
    cout << "Clear the error counters" << endl;
}

void
IpbusTTC::maskBC0fromTTC() {
    cout << "Mask BC0 from TTC history" << endl;
}

void
IpbusTTC::captureTTC() {
    cout << "Capturing TTC..." << endl;
}

void
IpbusTTC::sendBGo(uint32_t command = 0) {
    cout << "Sending BGo with command: " << command << endl;
}

uint32_t
IpbusTTC::getBunchCount() {
    cout << "Getting bunch count..." << endl;
    uint32_t bunchCount = 0;
    return bunchCount;
}

uint32_t
IpbusTTC::getEvtCount() {
    cout << "Getting event count..." << endl;
    uint32_t eventCount = 0;
    return eventCount;
}

uint32_t
IpbusTTC::getOrbitCount() {
    cout << "Getting orbit count..." << endl;
    uint32_t orbitCount = 0;
    return orbitCount;
}

uint32_t
IpbusTTC::getSBEC() {
    cout << "Getting single bit error count..." << endl;
    uint32_t SBECount = 0;
    return SBECount;
}

uint32_t
IpbusTTC::getDBEC() {
    cout << "Getting double bit error count..." << endl;
    uint32_t DBECount = 0;
    return DBECount;
}

//void 
//DummyTTC::getTTChistory() {
//
//    cout << "Getting TTC history" << endl;
//
//}

//void 
//DummyTTC::getTTShistory() {
//
//    cout << "Getting TTC history" << endl;
//
//}

uint32_t
IpbusTTC::getClk40lock() {

    cout << "Getting clock 40 lock..." << endl;
    uint32_t clk40lock = 0;
    return clk40lock;

}

uint32_t
IpbusTTC::getClk40stopped() {

    cout << "Getting clock 40 stopped..." << endl;
    uint32_t clk40stopped = 0;
    return clk40stopped;

}

uint32_t
IpbusTTC::getBC0lock() {

    cout << "Getting BC0 lock..." << endl;
    uint32_t BC0lock = 0;
    return BC0lock;

}

uint32_t
IpbusTTC::getBC0stopped() {

    cout << "Getting BC0 stopped..." << endl;
    uint32_t BC0stopped = 0;
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

/**
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
IpbusChanCtrl::isPLLlock() {
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

/**
 * Dummy Channel Buffer 
 */
IpbusChanBuffer::IpbusChanBuffer(swatch::processor::Connection* connection) : AbstractChanBuffer(connection) {
    cout << "this is a very dummy channel buffer interface" << endl;

    uhal::HwInterface& dummyhw = this->connection()->get<uhal::HwInterface>();

    uhal::ValWord<uint32_t> bsize = dummyhw.getNode("buffer.stat.size").read();
    dummyhw.dispatch();
    
    setBufferSize(bsize.value());
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

