/**
 * @file    Utilities.cpp
 * @author  Alessandro Thea
 * @brief   Processor service functions
 * @date    11/11/14
 */

#include "swatch/processor/Utilities.hpp"


// C++ headers
#include <iomanip>
#include <sstream>

// boost headers
#include "boost/foreach.hpp"
#include "boost/lexical_cast.hpp"
#include "boost/property_tree/ptree.hpp"
#include "boost/regex.hpp"

// SWATCH headers
#include "swatch/processor/ProcessorStub.hpp"
#include "swatch/core/toolbox/IdSliceParser.hpp"


namespace swatch {
namespace processor {

//swatch::core::XParameterSet
//treeToProcessorPars(const boost::property_tree::ptree& t) {
ProcessorStub
treeToProcessorStub(const boost::property_tree::ptree& aPTree)
{
  
    // Fill the stub with basic info
 
  
    ProcessorStub pStub(aPTree.get<std::string>("NAME"));
    pStub.creator      = aPTree.get<std::string>("CREATOR");
    pStub.hwtype       = aPTree.get<std::string>("HARDWARE TYPE");
    pStub.role         = aPTree.get<std::string>("ROLE");
    pStub.uri          = aPTree.get<std::string>("URI");
    pStub.addressTable = aPTree.get<std::string>("ADDRESS TABLE");
    pStub.crate        = aPTree.get<std::string>("CRATE NAME");
    pStub.slot         = aPTree.get<uint32_t>("CRATE SLOT");
    
    // Iterate over rx ports list
    BOOST_FOREACH(const boost::property_tree::ptree::value_type& rxPortInfo, aPTree.get_child("RX PORTS"))
    {
      try {
        pushBackPortStubs(pStub.rxPorts, rxPortInfo.second.get<std::string>("NAME"), rxPortInfo.second.get<std::string>("PID"));
      } catch ( const core::toolbox::FailedSliceParsing &e ) {
        std::ostringstream msg;
        msg << "Failed to parse processor '" << pStub.id << "' rx ports" << std::endl;
        msg << "Details: " << e.what();
        throw FailedJSONParsing( msg.str() );
      }
    }
    
    // Iterate over tx ports list
    BOOST_FOREACH(const boost::property_tree::ptree::value_type& txPortInfo, aPTree.get_child("TX PORTS"))
    {
      try { 
        pushBackPortStubs(pStub.txPorts, txPortInfo.second.get<std::string>("NAME"), txPortInfo.second.get<std::string>("PID"));
      } catch ( const core::toolbox::FailedSliceParsing &e ) {
        std::ostringstream msg;
        msg << "Failed to parse processor '" << pStub.id << "' tx ports" << std::endl;
        msg << "Details: " << e.what();
        throw FailedJSONParsing( msg.str() );
      }
    }
    
    return pStub;
}


void pushBackPortStubs(std::vector<ProcessorPortStub>& aPortStubs, const std::string& aName, const std::string& aIndex)
{
  std::vector<std::string> names = core::toolbox::IdSliceParser::parse(aName);
  std::vector<std::string> indices = core::toolbox::IdSliceParser::parse(aIndex);

  if (names.size() != indices.size())
    throw std::runtime_error(boost::lexical_cast<std::string>(names.size()) + " port names created from name \"" + aName + "\" using slice syntax, but " + boost::lexical_cast<std::string>(indices.size()) + " indices created from \"" + aIndex + "\"");

  for (size_t i = 0; i < names.size(); i++) {

    ProcessorPortStub b(names.at(i));
    b.number = boost::lexical_cast<unsigned>(indices.at(i));
    aPortStubs.push_back(b);
  }
}


} // namespace processor
} // namespace swatch
