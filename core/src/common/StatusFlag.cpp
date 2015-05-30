/* 
 * File:   StatusFlag.cpp
 * Author: Tom Williams
 * Date: May 2015
 */

#include "swatch/core/StatusFlag.hpp"


#include <iostream>
#include <stdexcept>

namespace swatch{
namespace core{


  std::ostream& operator<<(std::ostream& aOut, swatch::core::StatusFlag aValue) {
    if (aValue == swatch::core::kUnknown)
        return (aOut << "UNKNOWN");
    else if (aValue == swatch::core::kGood)
        return (aOut << "GOOD");
    else if (aValue == swatch::core::kError)
        return (aOut << "ERROR");
    throw std::runtime_error("Invalid StatusFlag enum value in ostream operator<<");
}


swatch::core::StatusFlag operator& (const swatch::core::StatusFlag& flag1, const swatch::core::StatusFlag& flag2)
{
    if(flag1 == swatch::core::kUnknown || flag2 == swatch::core::kUnknown)
        return swatch::core::kUnknown;
    else if (flag1 == swatch::core::kError || flag2 == swatch::core::kError)
        return swatch::core::kError;
    else
        return swatch::core::kGood;
}


} //ns core
} //ns swatch
