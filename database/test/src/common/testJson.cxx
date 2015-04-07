#include <cstdlib>
#include <map>
#include <boost/property_tree/ptree.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

#include "swatch/database/UUID.hpp"
#include "swatch/core/ParameterSet.hpp"
#include "swatch/core/Object.hpp"



// Boost Headers


using namespace std;
using namespace swatch::core;

/*
 * 
 */
int main(int argc, char** argv) {

    using boost::property_tree::ptree;
    using boost::property_tree::json_parser::read_json;
    using namespace boost::assign;

    using swatch::core::ParameterSet;
    ptree pt;

    boost::property_tree::json_parser::read_json("etc/SystemInit.json", pt);

    typedef std::map<swatch::database::UUID, ParameterSet> UPsMap;
    typedef std::map<swatch::database::UUID, UPsMap> UUPsMap;
    typedef std::map<swatch::database::UUID, UUPsMap> UUUPsMap;

    UPsMap mSystem_;
    UUPsMap mProcessors_;
    UUUPsMap mPort_;
    UUPsMap mServices_;
    UPsMap mLink_;
    //cout << pt.get_child("SYSTEM.NAME")<< endl;

    std::vector<std::string> procStrArgs = list_of("PROCESSOR NAME")("PROCESSOR TYPE")("ADDRESS TABLE")("CRATE ID")("CRATE LOCATION")("CRATE DESCRIPTION");
    std::vector<std::string> procIntArgs = list_of("CRATE SLOT");
    std::vector<std::string> amc13StrArgs = list_of("SERVICE NAME")("SERVICE TYPE")("ADDRESS TABLE")("CRATE ID")("CRATE LOCATION")("CRATE DESCRIPTION")("IP ADDRESS #1")("IP ADDRESS #2");
    std::vector<std::string> amc13IntArgs = list_of("CRATE SLOT");


    //"PORTS NAMES" : ["Port#1", "Port#2", "Port#3", "Port#144"]}}, 

    BOOST_FOREACH(ptree::value_type &gsystem, pt.get_child("GSYSTEM")) {
        // cout << "gsystem " << gsystem.first << " = " << gsystem.second.get<std::string>() << endl;

        BOOST_FOREACH(ptree::value_type &system, gsystem.second) {
            swatch::database::UUID SysId(system.second.get<std::string>("NAME"));
            ParameterSet Ps;
            mSystem_.insert(std::pair<swatch::database::UUID, ParameterSet>(SysId, Ps));
            //  cout << "system " << system.first << " = " << SysId.LogicID() <<  endl;

            BOOST_FOREACH(ptree::value_type &processors, system.second.get_child("PROCESSORS")) {

                BOOST_FOREACH(ptree::value_type &processors, processors.second) {
                    std::cout << "processors " << processors.first << " = " << processors.second.data() << std::endl;
                    ParameterSet procSet;

                    BOOST_FOREACH(std::string& s, procStrArgs) {
                        cout << s << " : " << processors.second.get<std::string>(s) << endl;
                        procSet.set(s, processors.second.get<std::string>(s));
                    }

                    BOOST_FOREACH(std::string& s, procIntArgs) {
                        cout << s << " : " << processors.second.get<int>(s) << endl;
                        procSet.set(s, processors.second.get<int>(s));
                    }
                    std::string pname = processors.second.get<std::string>("PROCESSOR NAME");
                    uint32_t ftokenpos = pname.find("#");
                    uint32_t stokenpos = pname.find("_");
                    std::string pnamebft = pname.substr(0, ftokenpos);
                    std::string pnameaft = pname.substr(ftokenpos + 1, stokenpos - ftokenpos - 1);
                    std::string pnameast = pname.substr(stokenpos + 1);
                    std::cout << "NAME " << pname << "  " << pnamebft << "  " << pnameaft << "  " << pnameast << std::endl;
                    // swatch::database::UUID ProcId(processors.second.get<std::string>("PROCESSOR NAME"));
                    //                        mProcessors_.insert(std::pair<UUID, UPsMap>(SysId,std::pair<UUID, ParameterSet>(ProcId, procSet)));
                    //UUID processorId()

                    BOOST_FOREACH(ptree::value_type &ports, processors.second.get_child("PORTS NAMES")) {
                        std::cout << ports.second.data() << std::endl;
                    }

                }

            }

            BOOST_FOREACH(ptree::value_type &processors, system.second.get_child("SERVICES")) {

                BOOST_FOREACH(ptree::value_type &services, processors.second) {
                    // if ( services.second.get<std::string>("NAME","") != "AMC13" ) continue;
                    cout << "services " << services.first << " = " << services.second.data() << endl;
                    ParameterSet amc13Set;

                    BOOST_FOREACH(std::string& s, amc13StrArgs) {
                        cout << s << " : " << services.second.get<std::string>(s) << endl;
                        amc13Set.set(s, services.second.get<std::string>(s));
                    }

                    BOOST_FOREACH(std::string& s, amc13IntArgs) {
                        cout << s << " : " << services.second.get<int>(s) << endl;
                        amc13Set.set(s, services.second.get<int>(s));
                    }
                }
            }
        }
    }




    return 0;
}



