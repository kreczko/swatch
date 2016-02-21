/* 
 * File:   MP7TCCInterface.hpp
 * Author: Alessandro Thea
 *
 * Created on November 6, 2014, 4:50 PM
 */

#ifndef __SWATCH_MP7_MP7READOUTINTERFACE_HPP__
#define __SWATCH_MP7_MP7READOUTINTERFACE_HPP__


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

    virtual ~MP7ReadoutInterface();
    
protected:
    virtual void retrieveMetricValues();
    
private:
    ::mp7::MP7Controller& mDriver;
};
 
} // namespace mp7
} // namespace swatch 

#endif /* __SWATCH_MP7_MP7READOUTINTERFACE_HPP__ */
