/* 
 * File:   TTCInterface.cpp
 * Author: Tom Williams
 * 
 * Created May 2015
 */

#include "swatch/processor/TTCInterface.hpp"


namespace swatch {
namespace processor {

TTCInterface::TTCInterface():
core::MonitorableObject( "ttc" )
{
  this->registerMetric<bool,TTCInterface>("isClock40Locked", *this, &TTCInterface::isClock40Locked, true, true);
  this->registerMetric<bool,TTCInterface>("hasClock40Stopped", *this, &TTCInterface::hasClock40Stopped, false, false);
  this->registerMetric<bool,TTCInterface>("isBC0Locked", *this, &TTCInterface::isBC0Locked, true, true);
  this->registerMetric<uint32_t,TTCInterface>("singleBitErrors", *this, &TTCInterface::getSingleBitErrors, 0, 0);
  this->registerMetric<uint32_t,TTCInterface>("doubleBitErrors", *this, &TTCInterface::getDoubleBitErrors, 0, 0);
  
}


std::vector<std::string>
TTCInterface::getDefaultMetrics() {
  std::vector<std::string> metrics;
  metrics.push_back("isClock40Locked");
  metrics.push_back("hasClock40Stopped");
  metrics.push_back("isBC0Locked");
  metrics.push_back("singleBitErrors");
  metrics.push_back("doubleBitErrors");
  
  return metrics;
}


} // end ns core
} // end ns swatch
