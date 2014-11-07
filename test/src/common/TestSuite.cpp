#include "swatch/test/TestSuite.hpp"


// uHAL Headers
#include "uhal/log/log.hpp"

// Swatch Headers


// Boost Headers
#include <boost/assign.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/unordered/unordered_map.hpp>
#include <boost/foreach.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/property_tree/json_parser.hpp>

// Std Headers
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

namespace swatch {
namespace test {

SystemExploreTest::SystemExploreTest() :
TestUnit("sys_explore") {
}

bool
SystemExploreTest::run() {

    using namespace boost::assign;

    ParameterSet a;
    a.insert("requires", "ttc;daq")("provides", "trigger")("class","DummyProcessor");
    ParameterSet a1 = a, a2 = a, a3 = a;

    System* lSystem = new System("calol2");
	Crate * crateC = new Crate("crateC");
    Crate * crateD = new Crate("crateD");
	lSystem->add(crateC);
	lSystem->add(crateD);
    
    
    // Use 3 different methods to build the dummy processors
    // 1. explicit  constructor call
    a1.insert("crate", "crateC")("slot", 1);
    Processor* p1 = new DummyProcessor("mp7-10", a1);
    lSystem->add(p1);
    
    // 2. Using ProcessorFactory, low level creator
    a2.insert("crate", "crateD") ("slot", 2);
    Processor* p2 = ProcessorFactory::get()->make("DummyProcessor","mp7-13", a2);
    lSystem->add(p2);
    
    // 3. Using ProcessorFactory, PSet based compact creator
    a3.insert("name","mp_4")("crate", "crateD") ("slot", 10);
    Processor* p3 = ProcessorFactory::get()->make(a3);
    lSystem->add(p3);
    
    vector< pair<string, string> > links;
    push_back(links)
            ("mp7-10.tx00", "mp7-13.rx01")
            ("mp7-10.tx01", "mp7-13.rx00");
    unsigned int lid;
    vector< pair<string, string> >::iterator lIt;
    for (lIt = links.begin(), lid = 0; lIt != links.end(); ++lIt, ++lid) {
        OutputPort* src = lSystem->getObj<OutputPort>(lIt->first);
        InputPort* dst = lSystem->getObj<InputPort>(lIt->second);

        stringstream lname;
        lname << "link" << std::setw(3) << std::setfill('0') << lid;
        Link* lLink = new Link(lname.str(), src, dst);
        lSystem->add(lLink);
    }
    
    cout << "Here we test iterators" << endl;
    cout << "======================" << endl;
    Object::iterator itObj;
    for (itObj = lSystem->begin(); itObj != lSystem->end(); ++itObj) {
        cout << " * " << itObj->path() << endl;
    }
    cout << endl;
    
    cout << "These are subsystem children" << endl;
    cout << "============================" << endl;
    
    BOOST_FOREACH( const std::string& name, lSystem->getChildren()) {
        cout << name << endl; 
    }
    cout << endl;
    
    
    cout << endl;
    cout << "Testing getters and aliases" << endl;
    cout << "===========================" << endl;
    vector<string> names;
    names += "mp7-10.tx00", "link000.src", "crateC.amc01.tx00";
    vector< string >::const_iterator itN;
    BOOST_FOREACH( string name, names ) {
        Object* o = lSystem->getObj(name);
        cout << "Found " << name << " : " << o->path() << " of type " << o->typeName() << endl;
    }
    cout << "Multi-hop getter" << endl;
    cout << "================" << endl;
    Object* o = lSystem->getObj("crateC")->getObj("amc01")->getObj("tx00");
    cout << "Testing  crate1 + mp7-13 + tx00: " << o->path() << " of type " << o->typeName() << endl;
   
    cout << endl;
    cout << "Processors in the system" << endl;
    cout << "========================" << endl;
    
    BOOST_FOREACH( Processor* p, lSystem->getProcessors() ) {
        cout << p->id() << " of type " << p->typeName() << ": crate=" << p->getCrateId() << endl;
    }
    cout << endl;
    
    cout << "Printing crate views" << endl;
    cout << "====================" << endl;

    boost::unordered_map<std::string, Crate*>::const_iterator cit;
    for (cit = lSystem->getCrates().begin(); cit != lSystem->getCrates().end(); ++cit) {
        cout << "crate '" << cit->first << "' : " << *(cit->second) << endl;
    }
    cout << endl;
    
    std::vector<std::string> objects;

    std::vector<std::string>::iterator it;

    cout << "And these their Paths" << endl;
    cout << "=====================" << endl;
    objects = lSystem->getPaths();
    for (it = objects.begin(); it != objects.end(); ++it) {
        cout << *it << endl;
    }
    cout << endl;
    
    delete lSystem;
    return true;

}


class Dummy {
public:
    Dummy( const std::string& id ) : name(id) { ++counts; uhal::log(uhal::Debug()," >> Dummy ",name," lives! (",uhal::Integer(counts),")");}
    Dummy( const Dummy& o ) : name(o.name) { ++counts; uhal::log(uhal::Debug(),">> Dummy ", name, " copied (",uhal::Integer(counts),")");}
    virtual ~Dummy() { --counts; cout << " >> Dummy " << name << " dies! (" << counts << ")" << endl; }
    std::string name;
    static uint32_t counts;
};

uint32_t Dummy::counts = 0;

JsonBuildTest::JsonBuildTest() :
TestUnit("json_test"){
}

bool
JsonBuildTest::run() {
    using namespace boost::assign;
    
    using boost::property_tree::ptree;
    using boost::property_tree::json_parser::read_json;
    using swatch::core::ParameterSet;
    ptree pt;

    read_json("etc/testdb.json", pt);

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
    
    return true;
}

}
}
