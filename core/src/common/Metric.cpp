/* 
 * File:   Metric.cpp
 * Author: Tom Williams
 * Date: May 2015
 */

#include "swatch/core/Metric.hpp"


namespace swatch {
namespace core {


template<>
std::string convertMetricDataToString<bool>(bool aData)
{
    std::ostringstream oss;
    oss << std::boolalpha << aData;
    return oss.str();
}


}
}
