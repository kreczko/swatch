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

// boost headers
#include "boost/foreach.hpp"

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
    BOOST_FOREACH( const auto &v, aPTree.get_child("AMC SLOTS")) {
      aStub.amcSlots.emplace_back(
        v.second.get_value<uint32_t>()
      );
    }
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
        swatch::processor::treeToLinkStub(v.second, aStub.links);
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

      

} // namespace system
} // namespace swatch
