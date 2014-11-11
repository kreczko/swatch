

// Swatch Headers
#include "swatch/test/Descriptors.hpp"
#include "swatch/core/ParameterSet.hpp"
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
    ptree pt;

    read_json(shellExpandPath("${SWATCH_TESTS}/etc/testdb.json"), pt);

    ptree &pt_system = pt.get_child("SYSTEM");

    std::vector<ParameterSet> vpset;
    BOOST_FOREACH(ptree::value_type &v, pt_system.get_child("PROCESSORS")) {

        swatch::test::ProcessorDescriptor pd;

        pd.name         = v.second.get<std::string>("PROCESSOR NAME");
        pd.creator      = v.second.get<std::string>("PROCESSOR CREATOR");
        pd.uri          = v.second.get<std::string>("URI");
        pd.addressTable = v.second.get<std::string>("URI"); // FIXME
        pd.crateId      = v.second.get<std::string>("CRATE NAME");
        pd.slot         = v.second.get<uint32_t>("CRATE SLOT");

        std::cout << pd << std::endl;


    }

    /* code */
    return 0;
}


//---
std::vector<std::string>
shellExpandPaths ( const std::string& aPath ) {
    
    std::vector<std::string> lPaths;
    wordexp_t lSubstitutedPath;
    int code = wordexp(aPath.c_str(), &lSubstitutedPath, WRDE_NOCMD );
    if ( code ) throw std::runtime_error("Failed expanding path: "+aPath);

    for ( std::size_t i = 0 ; i != lSubstitutedPath.we_wordc ; i++ )
        lPaths.push_back(lSubstitutedPath.we_wordv[i]);

    wordfree(&lSubstitutedPath);
    
    return lPaths;
}


//---
std::string
shellExpandPath(const std::string& aPath) {
    std::vector<std::string> lPaths = shellExpandPaths( aPath );
    
    if ( lPaths.size() > 1 ) throw std::runtime_error("Failed to expand: multiple matches found");
    
    return lPaths[0];
}
