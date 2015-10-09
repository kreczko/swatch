/* 
 * File:   AMCLink.hpp
 * Author: ale
 *
 * Created on October 5, 2015, 5:00 PM
 */

#ifndef __SWATCH_AMC13_AMCPORT_HPP__
#define	__SWATCH_AMC13_AMCPORT_HPP__

#include "swatch/core/MonitorableObject.hpp"

#include "swatch/dtm/AMCPort.hpp"
#include "amc13/AMC13.hh"

// namespace amc13 {
//   class AMC13;
// } // namespace amc13


namespace swatch {
namespace amc13 {

class AMCPort : public dtm::AMCPort {
public:
  AMCPort( uint32_t aSlot, ::amc13::AMC13& aDriver );
  virtual ~AMCPort();


protected:
    virtual void retrieveMetricValues();

  uint64_t read64bCounter( ::amc13::AMC13::Board aBoard, const std::string& aName );

  ::amc13::AMC13& mDriver;
  
};

} // namespace amc13
} // namespace swatch 

#endif	/* __SWATCH_AMC13_AMCPORT_HPP__ */

