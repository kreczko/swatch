/* 
 * File:   StatusFlag.cpp
 * Author: Tom Williams
 * Date:   May 2015
 */

#include "swatch/core/StatusFlag.hpp"


#include <iostream>
#include <stdexcept>


namespace swatch{
namespace core{


std::ostream& operator<<(std::ostream& aOut, swatch::core::StatusFlag aValue) {
  switch (aValue) {
    case kGood :
      return (aOut << "Good");
    case kWarning :  
      return (aOut << "Warning");
    case kError : 
      return (aOut << "Error");
    case kNoLimit : 
      return (aOut << "NoLimit");
    case kUnknown : 
      return (aOut << "Unknown");
    default : 
      throw std::runtime_error("Invalid StatusFlag enum value in ostream operator<<");
  }
}


swatch::core::StatusFlag operator& (const swatch::core::StatusFlag& flag1, const swatch::core::StatusFlag& flag2)
{
  if (flag1 == kNoLimit && flag2 == kNoLimit)
    return kNoLimit;
  else if (flag1 == kError || flag2 == kError)
    return kError;
  else if(flag1 == kUnknown || flag2 == kUnknown)
    return kUnknown;
  else if (flag1 == kWarning || flag2 == kWarning)
    return kWarning;
  else
    return kGood;
}


} //ns core
} //ns swatch
