/* 
 * File:   MP7TCCInterface.hpp
 * Author: Alessandro Thea
 *
 * Created on November 6, 2014, 4:50 PM
 */

#ifndef __SWATCH_MP7_MP7TTCINTERFACE_HPP__
#define __SWATCH_MP7_MP7TTCINTERFACE_HPP__


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
  virtual ~MP7TTCInterface();
  
  virtual void clearCounters();
  
  virtual void clearErrors();
  
protected:
  virtual void retrieveMetricValues();
    
private:
    
  ::mp7::MP7Controller& mDriver;

  core::Metric<uint32_t>& mMetricBC0Counter;
  core::Metric<uint32_t>& mMetricEC0Counter;
  core::Metric<uint32_t>& mMetricOC0Counter;
  core::Metric<uint32_t>& mMetricResyncCounter;
  core::Metric<uint32_t>& mMetricStartCounter;
  core::Metric<uint32_t>& mMetricStopCounter;
  core::Metric<uint32_t>& mMetricTestCounter;

};
 
} // namespace mp7
} // namespace swatch 

#endif /* __SWATCH_MP7_MP7TTCINTERFACE_HPP__ */
