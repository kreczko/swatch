/**
 * @file    JsonReader.cpp
 * @author  Alessandro Thea
 * @brief   Brief description
 * @date    11/11/14
 */

#include "swatch/hardware/test/JsonReader.hpp"

// Swatch Headers
#include "swatch/core/Utilities.hpp"
#include "swatch/core/ParameterSet.hpp"
#include "swatch/processor/ProcessorDescriptor.hpp"
#include "swatch/system/AMC13ServiceDescriptor.hpp"
#include "swatch/processor/Utilities.hpp"

// Boost Headers
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>


namespace swatch {
namespace hardware {
namespace test {

swatch::core::ParameterSet readJson( const std::string path ) {

    using boost::property_tree::ptree;
    using boost::property_tree::json_parser::read_json;
    using swatch::core::ParameterSet;
    using swatch::core::shellExpandPath;

    ParameterSet pset;
    ptree pt;

    read_json(shellExpandPath(path), pt);

    ptree &pt_system = pt.get_child("SYSTEM");

    std::deque<ParameterSet> processorSets;
    BOOST_FOREACH(ptree::value_type &v, pt_system.get_child("PROCESSORS")) {
        ParameterSet procSet = swatch::processor::treeToProcessorPSet(v.second);
        processorSets.push_back(procSet);
    }
    pset.set("processors",processorSets);

    std::deque<ParameterSet> amc13Sets;
    BOOST_FOREACH(ptree::value_type &v, pt_system.get_child("SERVICES")) {
        if ( v.second.get<std::string>("SERVICE TYPE","") != "AMC13" ) continue;
        ParameterSet amc13Set;
        swatch::system::AMC13ServiceDescriptor sd;

        sd.name           = v.second.get<std::string>("SERVICE NAME");
        sd.creator        = v.second.get<std::string>("SERVICE CREATOR");
        sd.t1Uri          = v.second.get<std::string>("URI T1");
        sd.t1AddressTable = v.second.get<std::string>("ADDRESS TABLE T1"); // FIXME
        sd.t2Uri          = v.second.get<std::string>("URI T2");
        sd.t2AddressTable = v.second.get<std::string>("ADDRESS TABLE T2"); // FIXME
        sd.crateId        = v.second.get<std::string>("CRATE NAME");
        sd.slot           = v.second.get<uint32_t>("CRATE SLOT");


        std::cout << sd << std::endl;
        amc13Set.set("name", sd.name);
        amc13Set.set("class", sd.creator);
        amc13Set.set("descriptor", sd);
        amc13Sets.push_back(amc13Set);
    }
    pset.set("services",amc13Sets);

    return pset;
}


}
}
}