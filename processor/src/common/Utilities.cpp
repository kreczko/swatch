/**
 * @file    Utilities.cpp
 * @author  Alessandro Thea
 * @brief   Processor service functions
 * @date    11/11/14
 */

#include "swatch/processor/Utilities.hpp"


// C++ headers
#include <iomanip>

// boost headers
#include "boost/foreach.hpp"
#include "boost/lexical_cast.hpp"
#include "boost/property_tree/ptree.hpp"
#include "boost/regex.hpp"

// SWATCH headers
#include "swatch/processor/ProcessorStub.hpp"
#include "swatch/processor/LinkStub.hpp"


namespace swatch {
namespace processor {

//swatch::core::XParameterSet
//treeToProcessorPars(const boost::property_tree::ptree& t) {
ProcessorStub
treeToProcessorStub(const boost::property_tree::ptree& t) {
  
    // Fill the stub with basic info
    /*
    ProcessorStub pstub;
    pstub.name         = t.get<std::string>("PROCESSOR NAME");
    pstub.hwtype       = t.get<std::string>("HARDWARE TYPE");
    pstub.creator      = t.get<std::string>("PROCESSOR CREATOR");
    pstub.uri          = t.get<std::string>("URI");
    pstub.addressTable = t.get<std::string>("ADDRESS TABLE");
    pstub.crate        = t.get<std::string>("CRATE NAME");
    pstub.slot         = t.get<uint32_t>("CRATE SLOT");
     */
  
  
    ProcessorStub pStub(t.get<std::string>("NAME"));
    pStub.creator      = t.get<std::string>("CREATOR");
    pStub.hwtype       = t.get<std::string>("HARDWARE TYPE");
    pStub.role         = t.get<std::string>("ROLE");
    pStub.uri          = t.get<std::string>("URI");
    pStub.addressTable = t.get<std::string>("ADDRESS TABLE");
    pStub.crate        = t.get<std::string>("CRATE NAME");
    pStub.slot         = t.get<uint32_t>("CRATE SLOT");
    
    // Iterate over rx ports list
    BOOST_FOREACH(const boost::property_tree::ptree::value_type& rxPortInfo, t.get_child("RX PORTS"))
    {
      expandPortSliceSyntax(rxPortInfo.second.get<std::string>("NAME"), rxPortInfo.second.get<std::string>("PID"), pStub.rxPorts);
    }
    
    // Iterate over tx ports list
    BOOST_FOREACH(const boost::property_tree::ptree::value_type& txPortInfo, t.get_child("TX PORTS"))
    {
      expandPortSliceSyntax(txPortInfo.second.get<std::string>("NAME"), txPortInfo.second.get<std::string>("PID"), pStub.txPorts);
    }
    
    /*
    // Finally, make the processor bag
    ProcessorBag pbag;
    pbag.bag = pstub;

    // Store the bag in the XParameterSet
    swatch::core::XParameterSet procSet;
    procSet.add("name", pbag.bag.name);
    procSet.add("class", pbag.bag.creator);
    procSet.add("stub", pbag);
    
    return procSet;
     */
    return pStub;
}


void treeToLinkStub(const boost::property_tree::ptree& t, std::vector<LinkStub>& linkBags) 
{
    const std::string name = t.get<std::string>("NAME");
    const std::string src = t.get<std::string>("FROM");
    const std::string dst = t.get<std::string>("TO");
  
    expandLinkSliceSyntax(name, src, dst, linkBags);
}


void expandLinkSliceSyntax(const std::string& aName, const std::string& aSrc, const std::string& aDst, std::vector<swatch::processor::LinkStub>& aLinkStubVector)
{
    std::vector<std::string> names = expandPortSliceSyntax(aName);
    std::vector<std::string> src = expandPortSliceSyntax(aSrc);
    std::vector<std::string> dst = expandPortSliceSyntax(aDst);
  
    if (names.size() != src.size() )
        throw std::runtime_error(boost::lexical_cast<std::string>(names.size())+" link names created from name \""+aName+"\" using slice syntax, but " +boost::lexical_cast<std::string>(src.size())+ " source IDs created from \""+aSrc+"\"");
    else if (names.size() != dst.size() )
        throw std::runtime_error(boost::lexical_cast<std::string>(names.size())+" link names created from name \""+aName+"\" using slice syntax, but " +boost::lexical_cast<std::string>(dst.size())+ " destination IDs created from \""+aDst+"\"");

    for(size_t i=0; i<names.size(); i++)
    {
        LinkStub b(names.at(i));
        b.src = src.at(i);
        b.dst = dst.at(i);
        aLinkStubVector.push_back(b);
    }
}


void expandPortSliceSyntax(const std::string& aName, const std::string& aIndex, std::vector<swatch::processor::ProcessorPortStub>& aPortStubVector)
{
    std::vector<std::string> names = expandPortSliceSyntax(aName);
    std::vector<std::string> indices = expandPortSliceSyntax(aIndex);
    
    if (names.size() != indices.size() )
        throw std::runtime_error(boost::lexical_cast<std::string>(names.size())+" port names created from name \""+aName+"\" using slice syntax, but " +boost::lexical_cast<std::string>(indices.size())+ " indices created from \""+aIndex+"\"");

    for(size_t i=0; i<names.size(); i++)
    {
      /* TO DELETE
      ProcessorPortStub b;
      b.name = names.at(i);
       */
      ProcessorPortStub b(names.at(i));
      b.number = boost::lexical_cast<unsigned>(indices.at(i));
      aPortStubVector.push_back(b);
    }
}


std::vector<std::string> expandPortSliceSyntax(const std::string& aString)
{
    const boost::regex sliceRegex("(.*)\\[(\\d+):(\\d+)(:(-?\\d+))?\\](.*)");
    
    // Perform the regex search; if regex doesn't match the argument, then it doesn't contain slice syntax, so return early
    boost::cmatch matchResults;
    if( ! boost::regex_match(aString.c_str(), matchResults, sliceRegex) )
    {
        return std::vector<std::string>(1, aString);
    }
    
    // Extract the relevant quantities from the match object
    const std::string prefix(matchResults[1]);
    const size_t startIdx = boost::lexical_cast<size_t>(matchResults[2]);
    const size_t stopIdx = boost::lexical_cast<size_t>(matchResults[3]);
    const int step = matchResults[5].matched ? boost::lexical_cast<int>(matchResults[5]) : 1;
    const std::string suffix(matchResults[6]);

    const size_t idxWidth = std::max(matchResults[2].second - matchResults[2].first, matchResults[3].second - matchResults[3].first);
    
    // Generate the result
    std::vector<std::string> results;
    for(size_t idx=startIdx; (step>0) ? (idx<stopIdx) : (idx>stopIdx); idx += step)
    {
        std::ostringstream oss;
        oss << prefix;
        oss << std::setw(idxWidth) << std::setfill('0') << idx;
        oss << suffix;
        results.push_back(oss.str());
    }
    return results;
}


} // namespace processor
} // namespace swatch
