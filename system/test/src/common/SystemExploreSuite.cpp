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

/*
struct SystemSetupA {
    SystemSetupA() :
        system(0x0),
        crateC(0x0),
        crateD(0x0) {
        using namespace boost::assign;
        using namespace std;

        system = new System(SystemStub("mysys"));
        crateC = new Crate(CrateStub("crateC"));
        crateD = new Crate(CrateStub("crateD"));
        system->add(crateC);
        system->add(crateD);
        
        
        // Use 2 different methods to build the dummy processors
        // 1. explicit  constructor call
        ProcessorStub s1("dummy-10");
        s1.crate = "crateC";
        s1.slot = 1;
        addRxTxPortStubs(s1); 
        Processor* p1 = new DummyProcessor(s1);
        system->add(p1);

        ProcessorStub s2("dummy-13");
        s2.crate = "crateD";
        s2.slot = 2;
        addRxTxPortStubs(s2); 
        Processor* p2 = Factory::get()->make<Processor>("swatch::processor::test::DummyProcessor",s2);
        system->add(p2);
        
        vector< pair<string, string> > links;
        push_back(links)
                ("dummy-10.ports.tx00", "dummy-13.ports.rx01")
                ("dummy-10.ports.tx01", "dummy-13.ports.rx00");
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
  LOG(kInfo) << "Running SystemExploreTestSuite/ExploreSystem";
    using namespace boost::assign;

    using namespace std;

    LOG(kDebug) << "Here we test iterators";
    LOG(kDebug) << "======================";
    Object::iterator itObj;
    for (itObj = system->begin(); itObj != system->end(); ++itObj) {
        LOG(kDebug) << " * " << itObj->getPath();
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
    names += "dummy-10.ports.tx00", "link000.src", "crateC.amc01.ports.tx00";
    vector< string >::const_iterator itN;
    BOOST_FOREACH( string name, names ) {
        Object& o = system->getObj(name);
        LOG(kDebug) << "Found " << name << " : " << o.getPath() << " of type " << o.getTypeName();
    }
    LOG(kDebug) << "Multi-hop getter";
    LOG(kDebug) << "================";
    Object& o = system->getObj("crateC").getObj("amc01").getObj("ports").getObj("tx00");
    LOG(kDebug) << "Testing  crate1 + dummy-13 + tx00: " << o.getPath() << " of type " << o.getTypeName();
    BOOST_CHECK_EQUAL(o.getPath(),"mysys.dummy-10.ports.tx00");
    BOOST_CHECK_EQUAL(o.getTypeName(),"swatch::processor::test::DummyTxPort");


    LOG(kDebug) << "";
    LOG(kDebug) << "Processors in the system";
    LOG(kDebug) << "========================";
    
    BOOST_FOREACH( Processor* p, system->getProcessors() ) {
        LOG(kDebug) << p->getId() << " of type " << p->getTypeName() << ": crate=" << p->getCrateId();
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
*/