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
#include "swatch/processor/LinkStub.hpp"
#include "swatch/processor/ProcessorStub.hpp"
#include "swatch/core/exception.hpp"

namespace swatch {
namespace processor {

ProcessorStub treeToProcessorStub(const boost::property_tree::ptree& aPTree);

void treeToLinkStub(const boost::property_tree::ptree& aPTree, std::vector<LinkStub>& aLinkStubs);

void pushBackPortStubs(std::vector<ProcessorPortStub>& aPortStubs, const std::string& aName, const std::string& aIndex);

void pushBackLinkStubs(std::vector<LinkStub>& aLinkStubs, const std::string& aName, const std::string& aSrc, const std::string& aDst);

DEFINE_SWATCH_EXCEPTION(FailedJSONParsing);

} // namespace processor
} // namespace swatch

#endif
