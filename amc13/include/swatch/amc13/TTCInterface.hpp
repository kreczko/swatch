/* 
 * File:   TTCInterface.hpp
 * Author: ale
 *
 * Created on September 21, 2015, 12:20 PM
 */

#ifndef SWATCH_AMC13_TTCINTERFACE_HPP
#define	SWATCH_AMC13_TTCINTERFACE_HPP

#include "swatch/core/MonitorableObject.hpp"

namespace amc13 {
  class AMC13;
} // namespace amc13

namespace swatch {
namespace amc13 {

class TTCInterface : public core::MonitorableObject {
public:
    TTCInterface( ::amc13::AMC13& aDriver );
    virtual ~TTCInterface( );
    
protected:
    virtual void retrieveMetricValues();

private:
    ::amc13::AMC13& mDriver;

    //! Metric for measured TTC clock frequency [Hz]
    core::Metric<double>& mClockFreq;
    //! Metric for TTC BC0 counter
    core::Metric<uint32_t>& mBC0Counter;
    //! Metric for TTC BC error counter
    core::Metric<uint32_t>& BC0Errors;
    //! Metric for TTC single-bit error counter
    core::Metric<uint32_t>& mSingleBitErrors;
    //! Metric for TTC double-bit error counter
    core::Metric<uint32_t>& mDoubleBitErrors;
    

};

    
} // namespace amc13
} // namespace swatch 



#endif	/* SWATCH_AMC13_TTCINTERFACE_HPP */

