/* 
 * File:   MP7TCCInterface.hpp
 * Author: Alessandro Thea
 *
 * Created on November 6, 2014, 4:50 PM
 */

#ifndef SWATCH_HARDWARE_MP7TCC
#define SWATCH_HARDWARE_MP7TCC

// Swatch Headers
#include "swatch/processor/TTCInterface.hpp"

namespace mp7 {
class MP7Controller;
}

namespace swatch {
namespace hardware {

class MP7TTCInterface : public swatch::processor::TTCInterface {
public:
    MP7TTCInterface( mp7::MP7Controller* );
    ~MP7TTCInterface();
    

    virtual void clearCounters();
    
    virtual void clearErrors();
    

    virtual uint32_t getBunchCounter() const;

    virtual uint32_t getOrbitCounter() const;

    virtual uint32_t getEventCounter() const;

    virtual uint32_t getSingleBitErrors() const;

    virtual uint32_t getDoubleBitErrors() const;

    virtual void sendBGo(uint32_t command);

    virtual bool hasClock40Stopped() const;
    
    virtual bool isClock40Locked() const;

    virtual bool isOrbitLocked() const;

    virtual bool hasBC0Stopped() const;

    virtual void maskBC0Spy(bool mask);

    virtual bool isBC0SpyMasked() const;

    virtual void spy();





    
    private:
    
    mp7::MP7Controller* driver_;

};
} // namespace hardware
} // namespace swatch 

#endif
