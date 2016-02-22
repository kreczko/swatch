/**
 * @file    Utilities.cpp
 * @author  Alessandro Thea
 * @brief   Processor service functions
 * @date    11/11/14
 */

#include "swatch/system/Utilities.hpp"


// SWATCH headers
#include "swatch/processor/Utilities.hpp"
#include "swatch/core/Utilities.hpp"
#include "swatch/processor/ProcessorStub.hpp"
#include "swatch/dtm/DaqTTCStub.hpp"
#include "swatch/system/CrateStub.hpp"
#include "swatch/core/toolbox/IdSliceParser.hpp"
#include "swatch/core/AbstractFactory.hpp"

// boost headers
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/range/algorithm/copy.hpp>

namespace swatch {
namespace dtm {

//---


swatch::dtm::DaqTTCStub
treeToDaqTTCStub(const boost::property_tree::ptree& aPTree)
{
  std::set<std::string> expected = {
    "NAME",
    "CREATOR",
    "ROLE",
    "URI T1",
    "ADDRESS TABLE T1",
    "URI T2",
    "ADDRESS TABLE T2",
    "CRATE NAME",
    "CRATE SLOT",
    "FED ID"
  };
  
  core::checkPtreeEntries(aPTree, expected);
  
  DaqTTCStub aStub(aPTree.get<std::string>("NAME"));

  aStub.creator = aPTree.get<std::string>("CREATOR");
  aStub.role = aPTree.get<std::string>("ROLE");
  aStub.uriT1 = aPTree.get<std::string>("URI T1");
  aStub.addressTableT1 = aPTree.get<std::string>("ADDRESS TABLE T1"); // FIXME
  aStub.uriT2 = aPTree.get<std::string>("URI T2");
  aStub.addressTableT2 = aPTree.get<std::string>("ADDRESS TABLE T2"); // FIXME
  aStub.crate = aPTree.get<std::string>("CRATE NAME");
  aStub.slot = aPTree.get<uint32_t>("CRATE SLOT");
  aStub.fedId = aPTree.get<uint16_t>("FED ID");

  return aStub;
}

} // namespace dtm
} // namespace swatch


namespace swatch {
namespace system {


//---


swatch::system::CrateStub
treeToCrateStub(const boost::property_tree::ptree& aPTree)
{
  std::set<std::string> expected = {
    "NAME",
    "LOCATION",
    "DESCRIPTION"
  };
  core::checkPtreeEntries(aPTree, expected);
  
  CrateStub astub(aPTree.get<std::string>("NAME"));

  astub.location = aPTree.get<std::string>("LOCATION");
  astub.description = aPTree.get<std::string>("DESCRIPTION");

  return astub;
}


//---


swatch::system::SystemStub
treeToSystemStub(const boost::property_tree::ptree& aPTree)
{
  using boost::property_tree::ptree;


  const ptree &lPTSystem = aPTree.get_child("SYSTEM");

  std::set<std::string> expected = {
    "NAME",
    "CREATOR",
    "CRATES",
    "EXCLUDED BOARDS",
    "PROCESSORS",
    "DAQTTCS",
    "LINKS",
    "CONNECTED FEDS"
  };
  core::checkPtreeEntries(lPTSystem, expected);

  SystemStub lStub(lPTSystem.get<std::string>("NAME"));
  lStub.loggerName = lStub.id;
  lStub.creator = lPTSystem.get<std::string>("CREATOR");


  BOOST_FOREACH(const ptree::value_type &v, lPTSystem.get_child("CRATES"))
  {
    lStub.crates.emplace_back(
        swatch::system::treeToCrateStub(v.second)
        );
  }

  BOOST_FOREACH(const ptree::value_type &v, lPTSystem.get_child("EXCLUDED BOARDS"))
  {
    lStub.excludedBoards.push_back(v.second.get_value<std::string>());
  }

  BOOST_FOREACH(const ptree::value_type &v, lPTSystem.get_child("PROCESSORS"))
  {
    swatch::processor::ProcessorStub lProcStub(swatch::processor::treeToProcessorStub(v.second));
    lProcStub.loggerName = lStub.id + "." + lProcStub.id;
    lStub.processors.emplace_back(lProcStub);
  }


  BOOST_FOREACH(const ptree::value_type &v, lPTSystem.get_child("DAQTTCS"))
  {
    swatch::dtm::DaqTTCStub lDaqTTCStub(swatch::dtm::treeToDaqTTCStub(v.second));
    lDaqTTCStub.loggerName = lStub.id + "." + lDaqTTCStub.id;
    lStub.daqttcs.emplace_back(lDaqTTCStub);
  }


  BOOST_FOREACH(const ptree::value_type& v, lPTSystem.get_child("LINKS"))
  {
    swatch::system::treeToLinkStub(v.second, lStub.links);
  }

  /* Enable if need services
  xdata::Vector<XParameterSet> serviceSets;
  BOOST_FOREACH( const ptree::value_type &v, pt_system.get_child("SERVICES")) {
      core::XParameterSet serviceSet = swatch::system::treeToServicesPars(v.second);
      serviceSets.push_back(serviceSet);
  }
  sysPars.add("services",serviceSets);
   */

  BOOST_FOREACH(const ptree::value_type& v, lPTSystem.get_child("CONNECTED FEDS"))
  {
    std::set<std::string> expected = {
      "FED ID",
      "RX PORTS",

    };
    
    // MAke sure the ptree entries maxt expectations
    core::checkPtreeEntries(v.second, expected);
    
    
    uint32_t fedID = v.second.get<uint32_t>("FED ID");
    
    // Check for duplicates
    if ( lStub.connectedFEDs.count(fedID) ) {
      std::ostringstream msg;
      msg << "FED " << fedID << " is referenced multiple times in 'CONNECTED FEDS'";
      throw core::FailedJSONParsing(msg.str());
    }
    
    
    std::set<std::string> lRxPorts;
    // Map to track duplicates
    std::map<std::string,uint32_t> lDuplicates;
    BOOST_FOREACH(const ptree::value_type& w, v.second.get_child("RX PORTS")) {
      
      std::vector<std::string> ids = core::toolbox::IdSliceParser::parse(w.second.get_value<std::string>());
      
      BOOST_FOREACH( std::string lPortPath, ids) {

        // Skip this port if it references an excluded board
        const std::string lProcId = lPortPath.substr(0, lPortPath.find('.'));
        if (std::count(lStub.excludedBoards.begin(), lStub.excludedBoards.end(), lProcId) > 0)
          continue;
        
        // Continue if insertion succeeds i.e. no duplicates
        if ( lRxPorts.insert(lPortPath).second ) continue;
        
        // Otherwise Increase duplicate count for id
        lDuplicates[lPortPath]++;
        
      }
      
    }

    if ( !lDuplicates.empty() ) {
      std::ostringstream msg;
      msg << "Duplicates found while parsing CONNECTED FEDS map, FED "<< fedID << ". ";
      BOOST_FOREACH( auto count, lDuplicates ) {
        msg << "'" << count.first << "': " << count.second << " ";
      }
      throw core::FailedJSONParsing(msg.str());
    }
    
    // Fill the map
    boost::copy(lRxPorts, std::back_inserter(lStub.connectedFEDs[fedID]));

  }
  
  std::set<std::string> lAllRxPorts, lDuplicates;

  // Check for duplicates across the whole FEDConnectionMap
  BOOST_FOREACH( auto fed, lStub.connectedFEDs ) {
      BOOST_FOREACH( std::string id, fed.second ) {
        // Continue if insertion succeeds i.e. no duplicates
        if ( lAllRxPorts.insert(id).second ) continue;
        
        // Otherwise 
        lDuplicates.insert(id);
      }
  }
  

  if (!lDuplicates.empty()) {
    // Map to track duplicates across different feds
    std::map<std::string, std::vector<uint32_t> > lDuplicateMap;
    BOOST_FOREACH( std::string id, lDuplicates ) {
      BOOST_FOREACH( auto fed, lStub.connectedFEDs ) {
 
        if ( std::find(fed.second.begin(), fed.second.end(), id) == fed.second.end() ) continue;
        
        lDuplicateMap[id].push_back(fed.first);
      } 
    }
    std::ostringstream msg;
    msg << "Duplicates found across multiple FEDs while parsing CONNECTED FEDS map. ";


    BOOST_FOREACH(auto count, lDuplicateMap){
      msg << "id: '" << count.first << "': ";
      boost::copy(count.second, std::ostream_iterator<uint32_t>(msg, " "));
    }
    throw core::FailedJSONParsing(msg.str());
  }
  
  return lStub;
}


void treeToLinkStub(const boost::property_tree::ptree& aPTree, std::vector<LinkStub>& aLinkStubs)
{
  
  std::set<std::string> expected = {"NAME", "RX", "RX PORT", "TX", "TX PORT"};
  core::checkPtreeEntries(aPTree, expected);
  
  const std::string name = aPTree.get<std::string>("NAME");
  const std::string src = aPTree.get<std::string>("RX");
  const std::string srcPort = aPTree.get<std::string>("RX PORT");
  const std::string dst = aPTree.get<std::string>("TX");
  const std::string dstPort = aPTree.get<std::string>("TX PORT");

  std::vector<std::string> names = core::toolbox::IdSliceParser::parse(name);

  std::vector<std::string> srcPorts = core::toolbox::IdSliceParser::parse(srcPort);
  std::vector<std::string> dstPorts = core::toolbox::IdSliceParser::parse(dstPort);

  if (srcPorts.size() != names.size()) {
    std::ostringstream msg;
    msg << "Link Stub creation error: Link ids '"
        << name << "' do not have the same multiplicity as source ids '"
        << src << "', '" << srcPort << "'. N(ids)=" << names.size() << " N(srcs)=" << srcPorts.size();
    throw std::runtime_error(msg.str());
  }

  if (dstPorts.size() != names.size()) {
    std::ostringstream msg;
    msg << "Link Stub creation error: Link ids '"
        << name << "' do not have the same multiplicity as destination ids '"
        << dst << "', '" << dstPort << "'. N(ids)=" << names.size() << " N(dsts)=" << dstPorts.size();
    throw std::runtime_error(msg.str());
  }

  for (size_t i = 0; i < names.size(); i++) {
    LinkStub b(names.at(i));
    b.srcProcessor = src;
    b.srcPort = srcPorts.at(i);
    b.dstProcessor = dst;
    b.dstPort = dstPorts.at(i);

    aLinkStubs.push_back(b);
  }
}


} // namespace system
} // namespace swatch
