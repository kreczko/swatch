/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Utilities.hpp
 * Author: ale
 *
 * Created on November 17, 2015, 4:49 PM
 */

#ifndef __SWATCH_MP7_UTILITIES_HPP__
#define __SWATCH_MP7_UTILITIES_HPP__

// Boost Headers
#include <boost/foreach.hpp>

// C++ Headers
#include <deque>
#include <string>
#include <vector>

// XDAQ Headers
#include "xdata/UnsignedInteger.h"

#include "swatch/core/exception.hpp"

// Forward declaration
namespace mp7 {
namespace orbit {
class Metric;
} // namespace orbit
} // namespace mp7

 // Orbit related functions
// TODO: move them into a separate library?

namespace swatch {
namespace mp7 {
namespace orbit {

bool isValid( const xdata::UnsignedInteger& aBx, const xdata::UnsignedInteger& aCycle, const ::mp7::orbit::Metric& aMetric );
bool isGood( const xdata::UnsignedInteger& aBx, const xdata::UnsignedInteger& aCycle, const ::mp7::orbit::Metric& aMetric );
bool isNull( const xdata::UnsignedInteger& aBx, const xdata::UnsignedInteger& aCycle);

DEFINE_SWATCH_EXCEPTION(PointError);

}

} // namespace mp7
} // namespace swatch


#endif /* __SWATCH_MP7_UTILITIES_HPP__ */

