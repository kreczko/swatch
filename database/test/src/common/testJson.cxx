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
                        BOOST_FOREACH( std::string& s, procStrArgs) {
                            cout << s << " : " << processors.second.get<std::string>(s) << endl;
                            procSet.set(s,processors.second.get<std::string>(s));
                        }
                        BOOST_FOREACH( std::string& s, procIntArgs) {
                            cout << s << " : " << processors.second.get<int>(s) << endl;
                            procSet.set(s,processors.second.get<int>(s));
                        }
                        std::string pname = processors.second.get<std::string>("PROCESSOR NAME");
                        uint32_t ftokenpos = pname.find("#");
                        uint32_t stokenpos = pname.find("_");
                        std::string pnamebft = pname.substr(0, ftokenpos);
                        std::string pnameaft = pname.substr(ftokenpos+1,  stokenpos-ftokenpos-1 );
                        std::string pnameast = pname.substr(stokenpos+1);
                        std::cout << "NAME " << pname << "  "  << pnamebft <<  "  "  << pnameaft <<  "  "  << pnameast <<std::endl;
                       // swatch::database::UUID ProcId(processors.second.get<std::string>("PROCESSOR NAME"));
//                        mProcessors_.insert(std::pair<UUID, UPsMap>(SysId,std::pair<UUID, ParameterSet>(ProcId, procSet)));
                       //UUID processorId()
                       BOOST_FOREACH(ptree::value_type &ports, processors.second.get_child("PORTS NAMES")){
                              std::cout <<  ports.second.data() << std::endl;    
                       }
                    
                   }
                   
                }
                BOOST_FOREACH(ptree::value_type &processors, system.second.get_child("SERVICES")) {
                     BOOST_FOREACH(ptree::value_type &services, processors.second) {
                        // if ( services.second.get<std::string>("NAME","") != "AMC13" ) continue;
                        cout << "services " << services.first << " = " << services.second.data() << endl;
                        ParameterSet amc13Set;
                        BOOST_FOREACH( std::string& s, amc13StrArgs) {
                            cout << s << " : " << services.second.get<std::string>(s) << endl;
                            amc13Set.set(s,services.second.get<std::string>(s));
                        }
                        BOOST_FOREACH( std::string& s, amc13IntArgs) {
                            cout << s << " : " << services.second.get<int>(s) << endl;
                            amc13Set.set(s,services.second.get<int>(s));
                        }
                     }
                }     
            }
        }
    
     
    
   
    return 0;
}

/*
    ptree &system = pt.get_child("SYSTEM");
    BOOST_FOREACH(ptree::value_type &v, system.get_child("PROCESSORS")) {
        
        vpset.push_back(procSet);
    }
    */

  /* 
   using boost::property_tree::ptree;
    ptree::const_iterator end = pt.end();
    for (ptree::const_iterator it = pt.begin(); it != end; ++it) {
        std::cout << it->first << ": " << it->second.get_value<std::string>() << std::endl;
        cout << typeid (it->second.data()).name() << endl;
    }
 
 */







/*
class SystemCreator : public swatch::system::SystemFactory::CreatorInterface {
public:

    virtual swatch::system::System* operator()(const std::string& aId, const swatch::core::ParameterSet& aPSet);
};

swatch::system::System*
SystemCreator::operator()(const std::string& aId, const swatch::core::ParameterSet& aPSet) {
    using namespace swatch::core;
    using namespace swatch::system;
    using namespace swatch::processor;   

    using namespace uhal;
    
    log(Info(),"Building System ",aId);
    System* sys = new System(aId, aPSet);
    
    std::vector<ParameterSet> vPSets;
    
    log(Info(), "Building Processors");
    vPSets = aPSet.get< std::vector<ParameterSet> >("processors");
    log(Debug(), " + Found ",uhal::Integer(vPSets.size())," processors");
    
    BOOST_FOREACH(ParameterSet& ps,vPSets) {
        log(Debug(),"name: ", ps.get<std::string>("PROCESSOR NAME")," ", ps.get<std::string>("PROCESSOR CREATOR"));
        
        Processor* p = ProcessorFactory::get()->make(ps.get<std::string>("PROCESSOR CREATOR"), ps.get<std::string>("PROCESSOR NAME"), ps);

        sys->add(p);  
    }

    log(Info(), "Building Amc13s");
    vPSets = aPSet.get< std::vector<ParameterSet> >("amc13s");
    log(Debug(), " + Found ",uhal::Integer(vPSets.size())," amc13s");

    BOOST_FOREACH(ParameterSet& ps,vPSets) {
        log(Debug(),"name: ", ps.get<std::string>("SERVICE NAME")," ", ps.get<std::string>("SERVICE CREATOR"));
        
        AMC13Service* a = static_cast<AMC13Service*>(ServiceFactory::get()->make(ps.get<std::string>("SERVICE CREATOR"), ps.get<std::string>("SERVICE NAME"), ps));
        
        sys->add(a);
    }
    return sys;
}

SWATCH_SYSTEM_REGISTER_CREATOR(SystemCreator);

using namespace std;


  
 */

    
    
    
/*
    ParameterSet sysset;
    
    uhal::setLogLevelTo(uhal::Debug());
    
  
    ptree &system = pt.get_child("SYSTEM");
    sysset.insert("name",system.get<std::string>("NAME"));
    
    std::vector<ParameterSet> vpset;

    std::vector<std::string> procStrArgs = list_of("PROCESSOR NAME")("PROCESSOR CREATOR")("PROCESSOR TYPE")("ADDRESS TABLE")("IP ADDRESS")("CRATE ID");
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
   
    std::vector<std::string> amc13StrArgs = list_of("SERVICE NAME")("SERVICE TYPE")("SERVICE CREATOR")("ADDRESS TABLE")("CRATE ID")("CRATE LOCATION")("CRATE DESCRIPTION")("IP ADDRESS #1")("IP ADDRESS #2");
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
    
    sysset.set("amc13s",vpset);
    
    swatch::system::System* mysys = swatch::system::SystemFactory::get()->make("SystemCreator", sysset.get<std::string>("name"), sysset);
    cout << "Here's a brand new system: " << mysys << endl; 
    
    return 0;
}

*/

