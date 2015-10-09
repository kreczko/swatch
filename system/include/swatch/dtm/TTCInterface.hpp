/* 
 * File:   TTCInterface.hpp
 * Author: ale
 *
 * Created on September 21, 2015, 12:20 PM
 */

#ifndef __SWATCH_DTM_TTCINTERFACE_HPP___
#define	__SWATCH_DTM_TTCINTERFACE_HPP___

#include "swatch/core/MonitorableObject.hpp"

namespace swatch {
namespace dtm {

class TTCInterface : public core::MonitorableObject {
protected:
    TTCInterface();
public:
    virtual ~TTCInterface( );
    
protected:

    //! Metric for measured TTC clock frequency [Hz]
    core::Metric<double>& mClockFreq;
    //! Metric for TTC BC0 counter
    core::Metric<uint32_t>& mBC0Counter;
    //! Metric for TTC BC error counter
    core::Metric<uint32_t>& mBC0Errors;
    //! Metric for TTC single-bit error counter
    core::Metric<uint32_t>& mSingleBitErrors;
    //! Metric for TTC double-bit error counter
    core::Metric<uint32_t>& mDoubleBitErrors;
    

};

    
} // namespace amc13
} // namespace swatch 



#endif	/* __SWATCH_DTM_TTCINTERFACE_HPP___ */

