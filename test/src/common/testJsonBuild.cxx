// Boost Headers
#include <boost/assign.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/unordered/unordered_map.hpp>
#include <boost/foreach.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/property_tree/json_parser.hpp>

// C++ Headers
#include <iomanip>

// Swatch includes
#include "swatch/core/ParameterSet.hpp"
#include "swatch/core/Object.hpp"
#include "swatch/core/Utilities.hpp"
#include "swatch/core/Link.hpp"
#include "swatch/core/Port.hpp"
#include "swatch/processor/Processor.hpp"
#include "swatch/processor/ProcessorFactory.hpp"
#include "swatch/system/System.hpp"
#include "swatch/system/Crate.hpp"
#include "swatch/system/SystemFactory.hpp"
#include "swatch/system/test/DummyProcessor.hpp"
#include "swatch/system/test/DummyAMC13Service.hpp"


// Namespace resolution
using namespace std;
using namespace swatch::core;
using namespace swatch::processor;
using namespace swatch::system;

using swatch::system::test::DummyProcessor;
using swatch::system::test::DummyAMC13Service;


int main(int argc, char const *argv[])
{
using namespace boost::assign;
    
    using boost::property_tree::ptree;
    using boost::property_tree::json_parser::read_json;
    using swatch::core::ParameterSet;
    ptree pt;

    read_json("test/etc/testdb.json", pt);

    ParameterSet sysset;
        
  
    ptree &system = pt.get_child("SYSTEM");
    sysset.insert("name",system.get<std::string>("NAME"));
    
    std::vector<ParameterSet> vpset;

    std::vector<std::string> procStrArgs = list_of("PROCESSOR NAME")("PROCESSOR CREATOR")("PROCESSOR TYPE")("IP ADDRESS")("CRATE NAME");
    std::vector<std::string> procIntArgs = list_of("CRATE SLOT");
    
    BOOST_FOREACH(ptree::value_type &v, system.get_child("PROCESSORS")) {
        ParameterSet procSet;
        BOOST_FOREACH( std::string& s, procStrArgs) {
            cout << s << " : " << v.second.get<std::string>(s) << endl;
            procSet.set(s,v.second.get<std::string>(s));
        }
        BOOST_FOREACH( std::string& s, procIntArgs) {
            cout << s << " : " << v.second.get<int>(s) << endl;
            procSet.set(s,v.second.get<int>(s));
        }
        vpset.push_back(procSet);
    }
   
    sysset.set("processors",vpset);
    vpset.clear();
   
    std::vector<std::string> amc13StrArgs = list_of("SERVICE NAME")("SERVICE TYPE")("SERVICE CREATOR")("CRATE NAME")("CRATE LOCATION")("CRATE DESCRIPTION")("IP ADDRESS #1")("IP ADDRESS #2");
    std::vector<std::string> amc13IntArgs = list_of("CRATE SLOT");
    
    BOOST_FOREACH(ptree::value_type &v, system.get_child("SERVICES")) {
        if ( v.second.get<std::string>("SERVICE TYPE","") != "AMC13" ) continue;
        std::string s("SERVICE NAME");
        cout << s << " : " << v.second.get<std::string>(s) << " + " << v.second.get<std::string>("PIPPO","no pippo")  << endl;
    
        ParameterSet amc13Set;
        BOOST_FOREACH( std::string& s, amc13StrArgs) {
            cout << s << " : " << v.second.get<std::string>(s) << endl;
            amc13Set.set(s,v.second.get<std::string>(s));
        }
        BOOST_FOREACH( std::string& s, amc13IntArgs) {
            cout << s << " : " << v.second.get<int>(s) << endl;
            amc13Set.set(s,v.second.get<int>(s));
        }
        vpset.push_back(amc13Set);
    }
    
    sysset.set("services",vpset);
    
    swatch::system::System* mysys = swatch::system::SystemFactory::get()->make("SystemCreator", sysset.get<std::string>("name"), sysset);
    cout << "Here's a brand new system: " << mysys << endl; 
    
    return 0;
}