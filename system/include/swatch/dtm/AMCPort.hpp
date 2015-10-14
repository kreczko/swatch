/* 
 * File:   AMCLink.hpp
 * Author: ale
 *
 * Created on October 5, 2015, 5:00 PM
 */

#ifndef __SWATCH_DTM_AMCPORT_HPP__
#define	__SWATCH_DTM_AMCPORT_HPP__

#include "swatch/core/MonitorableObject.hpp"

namespace swatch {
namespace dtm {

class AMCPort : public core::MonitorableObject {
protected:

  AMCPort( uint32_t aSlot );

public:
  virtual ~AMCPort();


protected:

  uint32_t mSlotId;
  
};

} // namespace amc13
} // namespace swatch 

#endif	/* __SWATCH_AMC13_AMCPORT_HPP__ */

