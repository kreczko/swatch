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

//SystemBuildTest::SystemBuildTest(unsigned int iterations) :
//TestUnit("sys_build"), mIterations(iterations) {
//}
//
//bool
//SystemBuildTest::run() {
//    using namespace boost::assign;
//
//    // for pretty printing, calculate how many digits to use
//    int digits(0), number(mIterations);
//    do {
//        number /= 10;
//        digits++;
//    } while (number != 0);
//
//    Arguments a;
//    a.insert("requires", "ttc;daq")("provides", "trg");
//    Arguments a1 = a, a2 = a, a3 = a;
//    a1.insert("crate", "crateA") ("slot", 1);
//    a2.insert("crate", "crateA") ("slot", 2);
//    a3.insert("crate", "crateB") ("slot", 1);
//
//    for (unsigned int i(0); i < mIterations; ++i) {
//
//        stringstream ss;
//        ss << "[" << std::setw(digits) << std::setfill('0') << i << "] --> ";
//
////        cout << ss.str() << "Constructor" << endl;
//        uhal::log( uhal::Info(), ss.str(), "Constructor");
//        System* s = new System("calol2");
//
//        DummyProcessor* p1 = new DummyProcessor("calol2-10", a1);
//        DummyProcessor* p2 = new DummyProcessor("calol2-13", a2);
//        DummyProcessor* p3 = new DummyProcessor("demux", a3);
//
//        uhal::log( uhal::Info(), ss.str(), "Adoption" );
//        s->add(p1);
//        s->add(p2);
//        s->add(p3);
//        
//        vector< pair<string,string> > links;
//        push_back(links) 
//                ("calol2-10.tx00", "demux.rx01")
//                ("calol2-13.tx01", "demux.rx00");
//        unsigned int lid;
//        vector< pair<string,string> >::iterator it;
//        for ( it = links.begin(), lid=0; it != links.end(); ++it, ++lid ) {
//            OutputPort* src = s->getObj<OutputPort>(it->first);
//            InputPort* dst = s->getObj<InputPort>(it->second);
//            
//            stringstream lname;
//            lname << "link" << std::setw(3) << std::setfill('0') << lid;
//            Link* lLink = new Link( lname.str(), src, dst );
//            s->add(lLink);
//        }
//
//        
//
////        cout << ss.str() << "Destructor" << endl;
//        uhal::log( uhal::Info(), ss.str(), "Destructor" );
//        
//        delete s;
//    }
//
//    return true;
//}

//SystemBuildFullCrate::SystemBuildFullCrate() :
//TestUnit("sys_build_crate") {
//}
//
//bool
//SystemBuildFullCrate::run() {
//    using namespace boost::assign;
//    Arguments args, args13;
//    args.insert("requires", "ttc;daq")("provides", "trg")("crate", "s2x3g18");
//    args13.insert("requires", "")("provides", "ttc;daq")("crate", "s2x3g18");
//    
//
//    stringstream ss;
//    ss << "[CrateTest] --> ";
//
//    // Create a System
//    cout << ss.str() << "Constructor" << endl;
//    System* lSystem = new System("calol2");
//    
//
//    // AMC13 comes first
//    AMC13Service* amc13 = new DummyAMC13Service("amc13xg", args13);
//    
//    // And a set of boards
//    std::vector<DummyProcessor*> dummies;
//    for (int s(0); s < 12; ++s) {
//        Arguments a = args; // copy the common attributes
//        const string slot = boost::lexical_cast<std::string>(s);
//        a.insert("slot", s);
//        DummyProcessor* p = new DummyProcessor("mp7-" + slot, a);
//        dummies.push_back(p);
//    }
//    cout << endl;
//    
//    // Don't forget the services
//    cout << ss.str() << "Adoption" << endl;
//    lSystem->add( amc13 );
//    
//    std::vector<DummyProcessor*>::iterator dIt;
//    for (dIt = dummies.begin(); dIt != dummies.end(); ++dIt) {
//        lSystem->add(*dIt);
//    }
//    cout << endl;
//
//    Crate* crate = lSystem->getObj<Crate>("s2x3g18");
//    cout << *(crate) << endl;
//    
//    cout << "Crate " << crate->id() << ", populated slots: "; 
//    BOOST_FOREACH( uint32_t i, crate->getPopulatedSlots() ) {
//        cout << i << " ";
//    }
//    cout << endl;
//    
//    cout << ss.str() << "Destructor" << endl;
//    delete lSystem;
//
//    return true;
//
//}

SystemExploreTest::SystemExploreTest() :
TestUnit("sys_explore") {
}

bool
SystemExploreTest::run() {

    using namespace boost::assign;

    Arguments a;
    a.insert("requires", "ttc;daq")("provides", "trigger")("class","DummyProcessor");
    Arguments a1 = a, a2 = a, a3 = a;

    System* lSystem = new System("calol2");

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
    cout << "====================" << endl;
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
