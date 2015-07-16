/* 
 * File:   MP7TCCInterface.hpp
 * Author: Alessandro Thea
 *
 * Created on November 6, 2014, 4:50 PM
 */

#ifndef SWATCH_MP7_MP7READOUTINTERFACE_HPP
#define SWATCH_MP7_MP7READOUTINTERFACE_HPP


// Swatch Headers
#include "swatch/processor/ReadoutInterface.hpp"


namespace mp7 {
class MP7Controller;
}


namespace swatch {
namespace mp7 {

class MP7ReadoutInterface : public swatch::processor::ReadoutInterface {
public:
    MP7ReadoutInterface( ::mp7::MP7Controller& );

    ~MP7ReadoutInterface();
    
protected:
    virtual void retrieveMetricValues();
    
private:
    ::mp7::MP7Controller& driver_;
};
 
} // namespace mp7
} // namespace swatch 

#endif
