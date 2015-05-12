/**
 * @file    Metric.hpp
 * @author  Andy Rose
 * @brief   An object which exposes its Monitorables
 * @date    August 2014
 *
 */

#ifndef __SWATCH_TEST_Metric_HPP__
#define	__SWATCH_TEST_Metric_HPP__

// SWATCH Headers
#include "swatch/core/Object.hpp"

namespace swatch {
namespace core {

//! Base class for swatch classes. Inspired by uhal::Node
class Metric {
public:
    Metric( const std::string& aId );
    
    virtual ~Metric();
    
     
protected:


private:
  std::string mId;
};


} // namespace core
} // namespace swatch

#endif	/* __SWATCH_TEST_Metric_HPP__ */

