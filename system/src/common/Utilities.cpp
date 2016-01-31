/**
 * @file    Utilities.cpp
 * @author  Alessandro Thea
 * @brief   Processor service functions
 * @date    11/11/14
 */

#include "swatch/system/Utilities.hpp"


// SWATCH headers
#include "swatch/processor/Utilities.hpp"
#include "swatch/processor/ProcessorStub.hpp"
#include "swatch/dtm/DaqTTCStub.hpp"
#include "swatch/system/CrateStub.hpp"
#include "swatch/core/toolbox/IdSliceParser.hpp"

// boost headers
#include "boost/foreach.hpp"
#include "boost/lexical_cast.hpp"

namespace swatch {
namespace dtm {

//---
swatch::dtm::DaqTTCStub
treeToDaqTTCStub(const boost::property_tree::ptree& aPTree)
{
    DaqTTCStub aStub(aPTree.get<std::string>("NAME"));

    aStub.creator        = aPTree.get<std::string>("CREATOR");
    aStub.role           = aPTree.get<std::string>("ROLE");
    aStub.uriT1          = aPTree.get<std::string>("URI T1");
    aStub.addressTableT1 = aPTree.get<std::string>("ADDRESS TABLE T1"); // FIXME
    aStub.uriT2          = aPTree.get<std::string>("URI T2");
    aStub.addressTableT2 = aPTree.get<std::string>("ADDRESS TABLE T2"); // FIXME
    aStub.crate          = aPTree.get<std::string>("CRATE NAME");
    aStub.slot           = aPTree.get<uint32_t>("CRATE SLOT");
    aStub.fedId          = aPTree.get<uint16_t>("FED ID");

    return aStub;
}

} // namespace dtm
} // namespace swatch


namespace swatch {
namespace system {


//---
swatch::system::CrateStub
treeToCrateStub(const boost::property_tree::ptree& aPTree) {

    CrateStub astub(aPTree.get<std::string>("NAME"));
    
    astub.location = aPTree.get<std::string>("LOCATION");
    astub.description = aPTree.get<std::string>("DESCRIPTION");
    
    return astub;
}
  

//---
swatch::system::SystemStub 
treeToSystemStub( const boost::property_tree::ptree& aPTree )
{
    using boost::property_tree::ptree;
    using boost::property_tree::json_parser::read_json;


    const ptree &pt_system = aPTree.get_child("SYSTEM");
    SystemStub aStub(pt_system.get<std::string>("NAME"));
    aStub.loggerName = aStub.id;
    aStub.creator = pt_system.get<std::string>("CREATOR");

    BOOST_FOREACH( const ptree::value_type &v, pt_system.get_child("CRATES")) {
      aStub.crates.emplace_back(
        swatch::system::treeToCrateStub(v.second)
      );
    }
    
    BOOST_FOREACH( const ptree::value_type &v, pt_system.get_child("PROCESSORS")) {
      swatch::processor::ProcessorStub lProcStub(swatch::processor::treeToProcessorStub(v.second));
      lProcStub.loggerName = aStub.id + "." + lProcStub.id;
      aStub.processors.emplace_back(lProcStub);
    }

    BOOST_FOREACH( const ptree::value_type &v, pt_system.get_child("DAQTTCS")) {
        swatch::dtm::DaqTTCStub lDaqTTCStub(swatch::dtm::treeToDaqTTCStub(v.second));
        lDaqTTCStub.loggerName = aStub.id + "." + lDaqTTCStub.id;
        aStub.daqttcs.emplace_back(lDaqTTCStub);
    }

    
    BOOST_FOREACH( const ptree::value_type& v, pt_system.get_child("LINKS")) {
        swatch::system::treeToLinkStub(v.second, aStub.links);
    }
    
    /* Enable if need services
    xdata::Vector<XParameterSet> serviceSets;
    BOOST_FOREACH( const ptree::value_type &v, pt_system.get_child("SERVICES")) {
        core::XParameterSet serviceSet = swatch::system::treeToServicesPars(v.second);
        serviceSets.push_back(serviceSet);
    }
    sysPars.add("services",serviceSets);
    */
   
    return aStub;
}

    
void treeToLinkStub(const boost::property_tree::ptree& aPTree, std::vector<LinkStub>& aLinkStubs) 
{
    const std::string name = aPTree.get<std::string>("NAME");
    const std::string src = aPTree.get<std::string>("FROM");
    const std::string dst = aPTree.get<std::string>("TO");
  
//    expandLinkSliceSyntax(name, src, dst, aLinkStubs);
    pushBackLinkStubs(aLinkStubs, name, src, dst);
    
}  


void pushBackLinkStubs(std::vector<LinkStub>& aLinkStubs, const std::string& aName, const std::string& aSrc, const std::string& aDst)
{
  std::vector<std::string> names = core::toolbox::IdSliceParser::parse(aName);
  std::vector<std::string> src = core::toolbox::IdSliceParser::parse(aSrc);
  std::vector<std::string> dst = core::toolbox::IdSliceParser::parse(aDst);

  if (names.size() != src.size())
    throw std::runtime_error(boost::lexical_cast<std::string>(names.size()) + " link names created from name \"" + aName + "\" using slice syntax, but " + boost::lexical_cast<std::string>(src.size()) + " source IDs created from \"" + aSrc + "\"");
  else if (names.size() != dst.size())
    throw std::runtime_error(boost::lexical_cast<std::string>(names.size()) + " link names created from name \"" + aName + "\" using slice syntax, but " + boost::lexical_cast<std::string>(dst.size()) + " destination IDs created from \"" + aDst + "\"");

  for (size_t i = 0; i < names.size(); i++) {
    LinkStub b(names.at(i));
    b.src = src.at(i);
    b.dst = dst.at(i);
    aLinkStubs.push_back(b);
  }
}


} // namespace system
} // namespace swatch
