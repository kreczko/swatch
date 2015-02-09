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

// Boost Headers
#include <boost/foreach.hpp>

namespace swatch {
namespace system {

swatch::core::ParameterSet
treeToCratePars(const boost::property_tree::ptree& t) {
    swatch::core::ParameterSet cratePars;
    return cratePars;
}
  
swatch::core::ParameterSet
treeToAMC13Pars(const boost::property_tree::ptree& t) {
    swatch::core::ParameterSet amc13Set;
    swatch::system::AMC13ServiceStub sd;

    sd.name           = t.get<std::string>("SERVICE NAME");
    sd.creator        = t.get<std::string>("SERVICE CREATOR");
    sd.uriT1          = t.get<std::string>("URI T1");
    sd.addressTableT1 = t.get<std::string>("ADDRESS TABLE T1"); // FIXME
    sd.uriT2          = t.get<std::string>("URI T2");
    sd.addressTableT2 = t.get<std::string>("ADDRESS TABLE T2"); // FIXME
    sd.crate          = t.get<std::string>("CRATE NAME");
    sd.slot           = t.get<uint32_t>("CRATE SLOT");

    // std::cout << sd << std::endl;
    amc13Set.set("name", sd.name);
    amc13Set.set("class", sd.creator);
    amc13Set.set("descriptor", sd);
    return amc13Set;
}

swatch::core::ParameterSet 
treeToSystemPars( const boost::property_tree::ptree& t ) {

    using boost::property_tree::ptree;
    using boost::property_tree::json_parser::read_json;
    using swatch::core::ParameterSet;
    using swatch::core::shellExpandPath;

    ParameterSet sysPars;
    // ptree t;

    // read_json(shellExpandPath(path), t);

    const ptree &pt_system = t.get_child("SYSTEM");

    std::deque<ParameterSet> processorSets;
    BOOST_FOREACH( const ptree::value_type &v, pt_system.get_child("PROCESSORS")) {
        core::ParameterSet procSet = swatch::processor::treeToProcessorPars(v.second);
        processorSets.push_back(procSet);
    }
    sysPars.set("processors",processorSets);

    std::deque<ParameterSet> amc13Sets;
    BOOST_FOREACH( const ptree::value_type &v, pt_system.get_child("SERVICES")) {
        if ( v.second.get<std::string>("SERVICE TYPE","") != "AMC13" ) continue;
        core::ParameterSet amc13Set = swatch::system::treeToAMC13Pars(v.second);
        amc13Sets.push_back(amc13Set);
    }
    sysPars.set("services",amc13Sets);

    return sysPars;
}

      

} // namespace system
} // namespace swatch
