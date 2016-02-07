/* 
 * File:   AMCLink.hpp
 * Author: ale
 *
 * Created on October 5, 2015, 5:00 PM
 */

#ifndef __SWATCH_DTM_AMCPORT_HPP__
#define __SWATCH_DTM_AMCPORT_HPP__

#include "swatch/core/MaskableObject.hpp"

namespace swatch {
namespace dtm {

class AMCPort : public core::MaskableObject {
protected:

  AMCPort( uint32_t aSlot );

public:
  virtual ~AMCPort();

  uint32_t getSlot() const;

protected:
  const uint32_t mSlotId;
};

} // namespace amc13
} // namespace swatch 

#endif	/* __SWATCH_DTM_AMCPORT_HPP__ */

