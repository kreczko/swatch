/* 
 * File:   TTCInterface.hpp
 * Author: ale
 *
 * Created on September 21, 2015, 12:20 PM
 */

#ifndef SWATCH_AMC13_TTCINTERFACE_HPP
#define	SWATCH_AMC13_TTCINTERFACE_HPP

#include "swatch/core/MonitorableObject.hpp"

namespace swatch {
namespace amc13 {

class TTCInterface : core::MonitorableObject {
public:
    TTCInterface();
    virtual ~TTCInterface();
private:
    // TODO: Move monitorables here
};

    
} // namespace amc13
} // namespace swatch 



#endif	/* SWATCH_AMC13_TTCINTERFACE_HPP */

