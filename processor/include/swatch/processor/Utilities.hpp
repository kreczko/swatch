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


namespace swatch {
namespace processor {

//swatch::core::XParameterSet treeToProcessorPars(const boost::property_tree::ptree& t);
ProcessorStub treeToProcessorStub(const boost::property_tree::ptree& aPTree);


void treeToLinkStub(const boost::property_tree::ptree& aPTree, std::vector<LinkStub>& aLinkStubs);


//void expandLinkSliceSyntax(const std::string& aName, const std::string& aSrc, const std::string& aDst, std::vector<LinkStub>& aLinkStubs);


//void expandPortSliceSyntax(const std::string& aName, const std::string& aIndex, std::vector<ProcessorPortStub>& aPortStubs);


void pushBackPortStubs(std::vector<ProcessorPortStub>& aPortStubs, const std::string& aName, const std::string& aIndex);


void pushBackLinkStubs(std::vector<LinkStub>& aLinkStubs, const std::string& aName, const std::string& aSrc, const std::string& aDst);


//std::vector<std::string> expandPortSliceSyntax(const std::string& aString);


} // namespace processor
} // namespace swatch

#endif
