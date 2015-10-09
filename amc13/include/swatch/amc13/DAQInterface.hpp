/* 
 * File:   DAQInterface.hpp
 * Author: ale
 *
 * Created on September 21, 2015, 12:21 PM
 */

#ifndef __SWATCH_AMC13_DAQINTERFACE_HPP__
#define	__SWATCH_AMC13_DAQINTERFACE_HPP__


#include "swatch/core/MonitorableObject.hpp"

namespace swatch {
namespace amc13 {

class DAQInterface : core::MonitorableObject {
public:
    DAQInterface();
    virtual ~DAQInterface();
private:

};

} // namespace amc13
} // namespace swatch


#endif	/* __SWATCH_AMC13_DAQINTERFACE_HPP__ */

