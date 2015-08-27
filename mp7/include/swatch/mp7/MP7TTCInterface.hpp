/* 
 * File:   MP7TCCInterface.hpp
 * Author: Alessandro Thea
 *
 * Created on November 6, 2014, 4:50 PM
 */

#ifndef SWATCH_MP7_MP7TTCINTERFACE_HPP
#define SWATCH_MP7_MP7TTCINTERFACE_HPP


// Swatch Headers
#include "swatch/processor/TTCInterface.hpp"


namespace mp7 {
class MP7Controller;
}


namespace swatch {
namespace mp7 {

class MP7TTCInterface : public swatch::processor::TTCInterface {
public:
    MP7TTCInterface( ::mp7::MP7Controller& );
    ~MP7TTCInterface();
    

    virtual void clearCounters();
    
    virtual void clearErrors();
    
    /*
    virtual uint32_t getBunchCounter() const;

    virtual uint32_t getOrbitCounter() const;

    virtual uint32_t getEventCounter() const;

    virtual uint32_t getSingleBitErrors() const;

    virtual uint32_t getDoubleBitErrors() const;

    virtual bool hasClock40Stopped() const;
    
    virtual bool isClock40Locked() const;

    virtual bool isBC0Locked() const;
    */
    
protected:
    virtual void retrieveMetricValues();
    
private:
    
    ::mp7::MP7Controller& driver_;

};
 
} // namespace mp7
} // namespace swatch 

#endif