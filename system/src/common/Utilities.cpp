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
treeToDaqTTCStub(const boost::property_tree::ptree& t) {
    DaqTTCStub aStub(t.get<std::string>("NAME"));

    aStub.creator        = t.get<std::string>("CREATOR");
    aStub.role           = t.get<std::string>("ROLE");
    aStub.uriT1          = t.get<std::string>("URI T1");
    aStub.addressTableT1 = t.get<std::string>("ADDRESS TABLE T1"); // FIXME
    aStub.uriT2          = t.get<std::string>("URI T2");
    aStub.addressTableT2 = t.get<std::string>("ADDRESS TABLE T2"); // FIXME
    aStub.crate          = t.get<std::string>("CRATE NAME");
    aStub.slot           = t.get<uint32_t>("CRATE SLOT");
    aStub.fedId          = t.get<uint16_t>("FED ID");
    BOOST_FOREACH( const auto &v, t.get_child("AMC SLOTS")) {
      aStub.amcSlots.emplace_back(
        v.second.get_value<uint32_t>()
      );
    }
    return aStub;
}

} // namespace system
} // namespace swatch


namespace swatch {
namespace system {


//---
swatch::system::CrateStub
treeToCrateStub(const boost::property_tree::ptree& t) {

    CrateStub astub(t.get<std::string>("NAME"));
    
    astub.location = t.get<std::string>("LOCATION");
    astub.description = t.get<std::string>("DESCRIPTION");
    
    return astub;
}
  

//---
swatch::system::SystemStub 
treeToSystemPars( const boost::property_tree::ptree& t ) {

    using boost::property_tree::ptree;
    using boost::property_tree::json_parser::read_json;


//    XParameterSet sysPars;

    const ptree &pt_system = t.get_child("SYSTEM");
    SystemStub aStub(pt_system.get<std::string>("NAME"));
    aStub.loggerName = aStub.id;
//    sysPars.add("name", xdata::String(pt_system.get<std::string>("NAME")));
    aStub.creator = pt_system.get<std::string>("CREATOR");

//    xdata::Vector<XParameterSet> crateSets;
//    std::vector<CrateStub> crateStubs; 
    BOOST_FOREACH( const ptree::value_type &v, pt_system.get_child("CRATES")) {
//      CrateStub cStubs = swatch::system::treeToCratePars(v.second);
      aStub.crates.emplace_back(
        swatch::system::treeToCrateStub(v.second)
      );
    }
//    sysPars.add("crates", crateSets);
    
//    xdata::Vector<XParameterSet> processorSets;
    BOOST_FOREACH( const ptree::value_type &v, pt_system.get_child("PROCESSORS")) {
      swatch::processor::ProcessorStub lProcStub(swatch::processor::treeToProcessorStub(v.second));
      lProcStub.loggerName = aStub.id + "." + lProcStub.id;
      aStub.processors.emplace_back(lProcStub);
    }
//    sysPars.add("processors",processorSets);

//    xdata::Vector<XParameterSet> daqTTCSets;
    BOOST_FOREACH( const ptree::value_type &v, pt_system.get_child("DAQTTCS")) {
//        core::XParameterSet amc13Set = swatch::system::treeToDaqTTCPars(v.second);
//        daqTTCSets.push_back(amc13Set);
        swatch::dtm::DaqTTCStub lDaqTTCStub(swatch::dtm::treeToDaqTTCStub(v.second));
        lDaqTTCStub.loggerName = aStub.id + "." + lDaqTTCStub.id;
        aStub.daqttcs.emplace_back(lDaqTTCStub);
    }
//    sysPars.add("daqttcs",daqTTCSets);

    
//    xdata::Vector<processor::LinkBag> linkBags;
    BOOST_FOREACH( const ptree::value_type& v, pt_system.get_child("LINKS")) {
        swatch::processor::treeToLinkStub(v.second, aStub.links);
    }
//    sysPars.add("links", linkBags);
    
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
