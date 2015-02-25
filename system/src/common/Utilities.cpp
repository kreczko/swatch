/**
 * @file    Utilities.cpp
 * @author  Alessandro Thea
 * @brief   Processor service functions
 * @date    11/11/14
 */


#include "swatch/system/Utilities.hpp"

// Swatch Headers
#include "swatch/processor/Utilities.hpp"
#include "swatch/system/AMC13ServiceStub.hpp"
#include "swatch/system/CrateStub.hpp"

// Boost Headers
#include <boost/foreach.hpp>

namespace swatch {
namespace system {

swatch::core::XParameterSet
treeToCratePars(const boost::property_tree::ptree& t) {

    swatch::core::XParameterSet cratePars;
    CrateStub astub;
    
    astub.name = t.get<std::string>("CRATE NAME");
    astub.location = t.get<std::string>("CRATE LOCATION");
    astub.description = t.get<std::string>("CRATE DESCRIPTION");
    
    CrateBag abag;
    abag.bag = astub;
    cratePars.add("name", abag.bag.name);
    cratePars.add("stub", abag );
    return cratePars;
}
  
swatch::core::XParameterSet
treeToAMC13Pars(const boost::property_tree::ptree& t) {
    swatch::core::XParameterSet amc13Set;
    AMC13ServiceStub astub;

    astub.name           = t.get<std::string>("SERVICE NAME");
    astub.creator        = t.get<std::string>("SERVICE CREATOR");
    astub.uriT1          = t.get<std::string>("URI T1");
    astub.addressTableT1 = t.get<std::string>("ADDRESS TABLE T1"); // FIXME
    astub.uriT2          = t.get<std::string>("URI T2");
    astub.addressTableT2 = t.get<std::string>("ADDRESS TABLE T2"); // FIXME
    astub.crate          = t.get<std::string>("CRATE NAME");
    astub.slot           = t.get<uint32_t>("CRATE SLOT");

    AMC13ServiceBag abag;
    abag.bag = astub;

    amc13Set.add("name", abag.bag.name);
    amc13Set.add("class", abag.bag.creator);
    amc13Set.add("stub", abag);
    return amc13Set;
}

swatch::core::XParameterSet 
treeToSystemPars( const boost::property_tree::ptree& t ) {

    using boost::property_tree::ptree;
    using boost::property_tree::json_parser::read_json;
    using swatch::core::XParameterSet;
    using swatch::core::shellExpandPath;

    XParameterSet sysPars;

    const ptree &pt_system = t.get_child("SYSTEM");
    sysPars.add("name", xdata::String(pt_system.get<std::string>("NAME")));

    xdata::Vector<XParameterSet> crateSets;
    BOOST_FOREACH( const ptree::value_type &v, pt_system.get_child("CRATES")) {
      core::XParameterSet crateSet = swatch::system::treeToCratePars(v.second);
      crateSets.push_back(crateSet);
    }
    sysPars.add("crates", crateSets);
    
    xdata::Vector<XParameterSet> processorSets;
    BOOST_FOREACH( const ptree::value_type &v, pt_system.get_child("PROCESSORS")) {
        core::XParameterSet procSet = swatch::processor::treeToProcessorPars(v.second);
        processorSets.push_back(procSet);
    }
    sysPars.add("processors",processorSets);

    xdata::Vector<XParameterSet> amc13Sets;
    BOOST_FOREACH( const ptree::value_type &v, pt_system.get_child("SERVICES")) {
        if ( v.second.get<std::string>("SERVICE TYPE","") != "AMC13" ) continue;
        core::XParameterSet amc13Set = swatch::system::treeToAMC13Pars(v.second);
        amc13Sets.push_back(amc13Set);
    }
    sysPars.add("services",amc13Sets);

    return sysPars;
}

      

} // namespace system
} // namespace swatch
