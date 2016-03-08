/**
 * @file    MP7AbstractProcessor.cpp
 * @author  Alessandro Thea
 * @brief   MP7 board processor implementation
 * @date    February 2016
 */

#include "swatch/mp7/MP7AbstractProcessor.hpp"

namespace swatch {
namespace mp7 {

const channel::DescriptorMap_t& MP7AbstractProcessor::getRxDescriptors() const
{
  return mRxDescriptors;
}


const channel::DescriptorMap_t& MP7AbstractProcessor::getTxDescriptors() const
{
  return mTxDescriptors;
}

} // namespace mp7
} // namespace swatch
