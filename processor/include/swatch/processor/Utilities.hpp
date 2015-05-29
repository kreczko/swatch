/**
 * @file    Utilities.cpp
 * @author  Alessandro Thea
 * @brief   Processor service functions
 * @date    11/11/14
 */

#ifndef SWATCH_PROCESSOR_UTILITIES_HPP
#define SWATCH_PROCESSOR_UTILITIES_HPP

// boost headers
#include <boost/property_tree/json_parser.hpp>

// SWATCH headers
#include "swatch/core/XParameterSet.hpp"
#include "swatch/processor/ProcessorStub.hpp"


namespace swatch {
namespace processor {

swatch::core::XParameterSet treeToProcessorPars(const boost::property_tree::ptree& t);


void expandPortSliceSyntax(const std::string& aName, const std::string& aIndex, std::vector<swatch::processor::ProcessorPortBag>& aPortStubVector);


std::vector<std::string> expandPortSliceSyntax(const std::string& aString);


} // namespace processor
} // namespace swatch

#endif
