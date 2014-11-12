

// Swatch Headers
#include "swatch/core/Utilities.hpp"
// #include "swatch/test/Descriptors.hpp"
#include "swatch/core/ParameterSet.hpp"
#include "swatch/processor/ProcessorStub.hpp"
#include "swatch/system/AMC13ServiceStub.hpp"

// Boost Headers
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

// C++ Headers
#include <vector>
#include <wordexp.h>
#include <stdexcept>

std::vector<std::string> shellExpandPaths ( const std::string& aPath );
std::string shellExpandPath(const std::string& aPath);

//---
int main(int argc, char const *argv[]) {

    using boost::property_tree::ptree;
    using boost::property_tree::json_parser::read_json;
    using swatch::core::ParameterSet;
    using swatch::core::shellExpandPath;
    ptree pt;

    read_json(shellExpandPath("${SWATCH_ROOT}/hardware/testdb.json"), pt);

    ptree &pt_system = pt.get_child("SYSTEM");

    std::deque<ParameterSet> processorSets;
    BOOST_FOREACH(ptree::value_type &v, pt_system.get_child("PROCESSORS")) {
        ParameterSet procSet;
        swatch::processor::ProcessorStub pd;

        pd.name         = v.second.get<std::string>("PROCESSOR NAME");
        pd.creator      = v.second.get<std::string>("PROCESSOR CREATOR");
        pd.uri          = v.second.get<std::string>("URI");
        pd.addressTable = v.second.get<std::string>("ADDRESS TABLE"); // FIXME
        pd.crate        = v.second.get<std::string>("CRATE NAME");
        pd.slot         = v.second.get<uint32_t>("CRATE SLOT");

        std::cout << pd << std::endl;
        procSet.set("id", pd.name);
        procSet.set("descriptor", pd);
        processorSets.push_back(procSet);
    }

    std::deque<ParameterSet> amc13Sets;
    BOOST_FOREACH(ptree::value_type &v, pt_system.get_child("SERVICES")) {
        if ( v.second.get<std::string>("SERVICE TYPE","") != "AMC13" ) continue;
        ParameterSet amc13Set;
        swatch::system::AMC13ServiceStub sd;

        sd.name           = v.second.get<std::string>("SERVICE NAME");
        sd.creator        = v.second.get<std::string>("SERVICE CREATOR");
        sd.uriT1          = v.second.get<std::string>("URI T1");
        sd.addressTableT1 = v.second.get<std::string>("ADDRESS TABLE T1"); // FIXME
        sd.uriT2          = v.second.get<std::string>("URI T2");
        sd.addressTableT2 = v.second.get<std::string>("ADDRESS TABLE T2"); // FIXME
        sd.crate          = v.second.get<std::string>("CRATE NAME");
        sd.slot           = v.second.get<uint32_t>("CRATE SLOT");


        std::cout << sd << std::endl;
        amc13Set.set("id", sd.name);
        amc13Set.set("descriptor", sd);
        amc13Sets.push_back(amc13Set);
    }

    /* code */
    return 0;
}

