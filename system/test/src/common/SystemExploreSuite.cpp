#include <boost/test/unit_test.hpp>

// Swatch Headers
#include "swatch/core/ParameterSet.hpp"
#include "swatch/core/Object.hpp"
#include "swatch/core/Utilities.hpp"
#include "swatch/core/Link.hpp"
#include "swatch/core/Port.hpp"

#include "swatch/processor/Processor.hpp"
#include "swatch/processor/ProcessorFactory.hpp"

#include "swatch/system/System.hpp"
#include "swatch/system/SystemFactory.hpp"
#include "swatch/system/Crate.hpp"
#include "swatch/system/test/DummyProcessor.hpp"
#include "swatch/system/test/DummyAMC13Service.hpp"

// Boost Headers
#include <boost/assign.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/unordered/unordered_map.hpp>
#include <boost/foreach.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/property_tree/json_parser.hpp>

struct SystemSetupA {
    SystemSetupA() :
        system(0x0),
        crateC(0x0),
        crateD(0x0) {
        using namespace boost::assign;
        using namespace swatch::core;
        using namespace swatch::system;
        using namespace swatch::processor;
        using namespace swatch::system::test;
        using namespace std;

        ParameterSet a;
        a.insert("requires", "ttc;daq")("provides", "trigger")("class","DummyProcessor");
        ParameterSet a1 = a, a2 = a, a3 = a;

        system = new System("calol2");
        crateC = new Crate("crateC");
        crateD = new Crate("crateD");
        system->add(crateC);
        system->add(crateD);
        
        
        // Use 3 different methods to build the dummy processors
        // 1. explicit  constructor call
        a1.insert("crate", "crateC")("slot", 1);
        Processor* p1 = new DummyProcessor("mp7-10", a1);
        system->add(p1);
        
        // 2. Using ProcessorFactory, low level creator
        a2.insert("crate", "crateD") ("slot", 2);
        Processor* p2 = ProcessorFactory::get()->make("DummyProcessor","mp7-13", a2);
        system->add(p2);
        
        // 3. Using ProcessorFactory, PSet based compact creator
        a3.insert("name","mp_4")("crate", "crateD") ("slot", 10);
        Processor* p3 = ProcessorFactory::get()->make(a3);
        system->add(p3);
        
        vector< pair<string, string> > links;
        push_back(links)
                ("mp7-10.tx00", "mp7-13.rx01")
                ("mp7-10.tx01", "mp7-13.rx00");
        unsigned int lid;

        vector< pair<string, string> >::iterator lIt;
        for (lIt = links.begin(), lid = 0; lIt != links.end(); ++lIt, ++lid) {
            OutputPort* src = system->getObj<OutputPort>(lIt->first);
            InputPort* dst = system->getObj<InputPort>(lIt->second);

            stringstream lname;
            lname << "link" << std::setw(3) << std::setfill('0') << lid;
            Link* lLink = new Link(lname.str(), src, dst);
            system->add(lLink);
        }
    }

    ~SystemSetupA() {
        delete system;
    }

    swatch::system::System* system;
    swatch::system::Crate* crateC;
    swatch::system::Crate* crateD;
    swatch::processor::Processor* p1;
    swatch::processor::Processor* p2;
    swatch::processor::Processor* p3;

};

BOOST_FIXTURE_TEST_SUITE(SystemExploreTestSuite, SystemSetupA)

// TODO
BOOST_AUTO_TEST_CASE(ExploreSystem) {
    using namespace boost::assign;
    using namespace swatch::core;
    using namespace swatch::system;
    using namespace swatch::processor;
    using namespace swatch::system::test;
    using namespace std;

    cout << "Here we test iterators" << endl;
    cout << "======================" << endl;
    Object::iterator itObj;
    for (itObj = system->begin(); itObj != system->end(); ++itObj) {
        cout << " * " << itObj->path() << endl;
    }
    cout << endl;
    
    cout << "These are subsystem children" << endl;
    cout << "============================" << endl;
    
    BOOST_FOREACH( const std::string& name, system->getChildren()) {
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
        Object* o = system->getObj(name);
        cout << "Found " << name << " : " << o->path() << " of type " << o->typeName() << endl;
    }
    cout << "Multi-hop getter" << endl;
    cout << "================" << endl;
    Object* o = system->getObj("crateC")->getObj("amc01")->getObj("tx00");
    cout << "Testing  crate1 + mp7-13 + tx00: " << o->path() << " of type " << o->typeName() << endl;
    BOOST_CHECK_EQUAL(o->path(),"calol2.mp7-10.tx00");
    BOOST_CHECK_EQUAL(o->typeName(),"swatch::core::OutputPort");


    cout << endl;
    cout << "Processors in the system" << endl;
    cout << "========================" << endl;
    
    BOOST_FOREACH( Processor* p, system->getProcessors() ) {
        cout << p->id() << " of type " << p->typeName() << ": crate=" << p->getCrateId() << endl;
    }
    cout << endl;
    
    cout << "Printing crate views" << endl;
    cout << "====================" << endl;

    boost::unordered_map<std::string, Crate*>::const_iterator cit;
    for (cit = system->getCrates().begin(); cit != system->getCrates().end(); ++cit) {
        cout << "crate '" << cit->first << "' : " << *(cit->second) << endl;
    }
    cout << endl;
    
    std::vector<std::string> objects;

    std::vector<std::string>::iterator it;

    cout << "And these their Paths" << endl;
    cout << "=====================" << endl;
    objects = system->getPaths();
    for (it = objects.begin(); it != objects.end(); ++it) {
        cout << *it << endl;
    }
    cout << endl;
    
} 

BOOST_AUTO_TEST_SUITE_END()