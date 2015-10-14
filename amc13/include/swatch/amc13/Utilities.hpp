/* 
 * File:   Utilities.hpp
 * Author: ale
 *
 * Created on October 12, 2015, 12:21 PM
 */

#ifndef __SWATCH_AMC13_UTILITIES_HPP__
#define __SWATCH_AMC13_UTILITIES_HPP__

#include <stdint.h>

#include "amc13/AMC13.hh"

namespace swatch {
namespace amc13 {

uint64_t read64bCounter(::amc13::AMC13& aDriver, ::amc13::AMC13::Board aBoard, const std::string& aName );

} // namespace amc13
} // namespace swatch


#endif  /* __SWATCH_AMC13_UTILITIES_HPP__ */