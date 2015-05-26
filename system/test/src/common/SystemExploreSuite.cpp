#include <boost/test/unit_test.hpp>

// Swatch Headers
#include "swatch/logger/Log.hpp"

#include "swatch/core/XParameterSet.hpp"
#include "swatch/core/Object.hpp"
#include "swatch/core/Utilities.hpp"
#include "swatch/processor/Link.hpp"
#include "swatch/processor/Port.hpp"

#include "swatch/processor/Processor.hpp"

#include "swatch/system/System.hpp"
#include "swatch/core/Factory.hpp"
#include "swatch/system/Crate.hpp"
#include "swatch/processor/test/DummyProcessor.hpp"
#include "swatch/system/test/DummyAMC13Manager.hpp"
#include "swatch/system/test/utilities.hpp"

// XDAQ Headers
#include "xdata/UnsignedInteger.h"
#include "xdata/String.h"
#include "swatch/processor/ProcessorStub.hpp"

// Boost Headers
#include <boost/assign.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/unordered/unordered_map.hpp>
#include <boost/foreach.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/property_tree/json_parser.hpp>

// Namespace Resolution
using namespace swatch::logger;
using namespace swatch::core;
using namespace swatch::system;
using namespace swatch::processor;
using namespace swatch::system::test;

using swatch::processor::test::DummyProcessor;

struct SystemSetupA {
    SystemSetupA() :
        system(0x0),
        crateC(0x0),
        crateD(0x0) {
        using namespace boost::assign;
        using namespace std;

        XParameterSet a;
        a.insert("requires", xdata::String("ttc;daq"))
            ("provides", xdata::String("trigger"))
            ("class",xdata::String("swatch::processor::test::DummyProcessor"));
        XParameterSet a1 = a, a2 = a, a3 = a;

        system = new System("calol2");
        crateC = new Crate("crateC");
        crateD = new Crate("crateD");
        system->add(crateC);
        system->add(crateD);
        
        
        // Use 3 different methods to build the dummy processors
        // 1. explicit  constructor call
        ProcessorBag b1;
        b1.bag.crate = xdata::String("crateC");
        b1.bag.slot = xdata::UnsignedInteger(1);
        addRxTxPortStubs(b1.bag);
        a1.add("stub", b1);

        Processor* p1 = new DummyProcessor("mp7-10", a1);
        system->add(p1);
        
        // 2. Using ProcessorFactory, low level creator
        ProcessorBag b2;
        b2.bag.crate = xdata::String("crateD");
        b2.bag.slot = xdata::UnsignedInteger(2);
        addRxTxPortStubs(b2.bag);
        a2.add("stub", b2);
        
        // a2.insert("crate", xdata::String("crateD"))
          // ("slot", xdata::Integer(2));
//        Processor* p2 = ProcessorFactory::get()->make("swatch::processor::test::DummyProcessor","mp7-13", a2);
        Processor* p2 = Factory::get()->bake<Processor>("swatch::processor::test::DummyProcessor","mp7-13", a2);
        system->add(p2);
        
        // 3. Using ProcessorFactory, PSet based compact creator
        a3.insert("name",xdata::String("mp_4"));
        ProcessorBag b3;
        b3.bag.crate = xdata::String("crateD");
        b3.bag.slot = xdata::UnsignedInteger(3);
        addRxTxPortStubs(b3.bag);
        a3.add("stub", b3);
        
        Processor* p3 = Factory::get()->bake<Processor>(a3);
        system->add(p3);
        
        vector< pair<string, string> > links;
        push_back(links)
                ("mp7-10.links.tx00", "mp7-13.links.rx01")
                ("mp7-10.links.tx01", "mp7-13.links.rx00");
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


//         vector< pair<string, string> > lSource , lDestinations;
//         push_back(lSource)
//                 ("mp7-10","tx00")
//                 ("mp7-10","tx01");
//         push_back(lDestinations)
//                 ("mp7-13","rx01")
//                 ("mp7-13","rx00");
//         unsigned int lid;
// 
//         vector< pair<string, string> >::iterator lIt1( lSource.begin() ) , lIt2( lDestinations.begin() );
// 
//         for ( lid = 0; lIt1 != lSource.end(); ++lIt1, ++lIt2, ++lid) {
//             OutputPort* src = system->getObj<Processor>(lIt1->first)->linkInterface()->getOutput(lIt1->second);
//             InputPort* dst = system->getObj<Processor>(lIt2->first)->linkInterface()->getInput(lIt2->second);
// 
//             stringstream lname;
//             lname << "link" << std::setw(3) << std::setfill('0') << lid;
//             Link* lLink = new Link(lname.str(), src, dst);
//             system->add(lLink);
//         }


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
  LOG(kInfo) << "Running SystemExploreTestSuite/ExploreSystem";
    using namespace boost::assign;

    using namespace std;

    LOG(kDebug) << "Here we test iterators";
    LOG(kDebug) << "======================";
    Object::iterator itObj;
    for (itObj = system->begin(); itObj != system->end(); ++itObj) {
        LOG(kDebug) << " * " << itObj->path();
    }
    LOG(kDebug) << "";
    
    LOG(kDebug) << "These are subsystem children";
    LOG(kDebug) << "============================";
    
    BOOST_FOREACH( const std::string& name, system->getDescendants()) {
        LOG(kDebug) << name; 
    }
    LOG(kDebug) << "";
    
    
    LOG(kDebug) << "Testing getters and aliases";
    LOG(kDebug) << "===========================";
    vector<string> names;
    names += "mp7-10.links.tx00", "link000.src", "crateC.amc01.links.tx00";
    vector< string >::const_iterator itN;
    BOOST_FOREACH( string name, names ) {
        Object* o = system->getObj(name);
        LOG(kDebug) << "Found " << name << " : " << o->path() << " of type " << o->typeName();
    }
    LOG(kDebug) << "Multi-hop getter";
    LOG(kDebug) << "================";
    Object* o = system->getObj("crateC")->getObj("amc01")->getObj("links")->getObj("tx00");
    LOG(kDebug) << "Testing  crate1 + mp7-13 + tx00: " << o->path() << " of type " << o->typeName();
    BOOST_CHECK_EQUAL(o->path(),"calol2.mp7-10.links.tx00");
    BOOST_CHECK_EQUAL(o->typeName(),"swatch::processor::test::DummyTxPort");


    LOG(kDebug) << "";
    LOG(kDebug) << "Processors in the system";
    LOG(kDebug) << "========================";
    
    BOOST_FOREACH( Processor* p, system->getProcessors() ) {
        LOG(kDebug) << p->id() << " of type " << p->typeName() << ": crate=" << p->getCrateId();
    }
    LOG(kDebug) << "";
    
    LOG(kDebug) << "Printing crate views";
    LOG(kDebug) << "====================";

    System::CratesMap::const_iterator cit;
    for (cit = system->getCrates().begin(); cit != system->getCrates().end(); ++cit) {
        LOG(kDebug) << "crate '" << cit->first << "' : " << *(cit->second);
    }
    LOG(kDebug) << "";
    
    std::vector<std::string> objects;

    std::vector<std::string>::iterator it;

    LOG(kDebug) << "And these their Paths";
    LOG(kDebug) << "=====================";
    objects = system->getPaths();
    for (it = objects.begin(); it != objects.end(); ++it) {
        LOG(kDebug) << *it;
    }
    LOG(kDebug) << "";
    
} 

BOOST_AUTO_TEST_SUITE_END()
