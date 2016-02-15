/**
 * @file    Utilities.cpp
 * @author  Alessandro Thea
 * @brief   Processor service functions
 * @date    11/11/14
 */

#ifndef SWATCH_PROCESSOR_UTILITIES_HPP
#define SWATCH_PROCESSOR_UTILITIES_HPP

// boost headers
#include "boost/property_tree/ptree_fwd.hpp"

// SWATCH headers
#include "swatch/processor/ProcessorStub.hpp"
#include "swatch/core/exception.hpp"

namespace swatch {
namespace processor {

ProcessorStub treeToProcessorStub(const boost::property_tree::ptree& aPTree);

void pushBackPortStubs(std::vector<ProcessorPortStub>& aPortStubs, const std::string& aName, const std::string& aIndex);

} // namespace processor
} // namespace swatch

#endif
