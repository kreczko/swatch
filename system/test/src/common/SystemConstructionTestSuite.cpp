#include <boost/test/unit_test.hpp>

// Swatch Headers
#include "swatch/core/XParameterSet.hpp"
#include "swatch/core/Object.hpp"
#include "swatch/core/Utilities.hpp"
#include "swatch/processor/Link.hpp"
#include "swatch/processor/Port.hpp"

#include "swatch/processor/Processor.hpp"
#include "swatch/processor/ProcessorStub.hpp"

#include "swatch/system/System.hpp"
#include "swatch/system/Crate.hpp"
#include "swatch/system/DaqTTCStub.hpp"
#include "swatch/processor/test/DummyProcessor.hpp"
#include "swatch/system/test/DummyAMC13Manager.hpp"
#include "swatch/system/test/utilities.hpp"
#include "swatch/logger/Log.hpp"

// XDAQ Headers
// #include "xdata/String.h"
// #include "xdata/Integer.h"

// Boost Headers
#include <boost/assign.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/unordered/unordered_map.hpp>
#include <boost/foreach.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/property_tree/json_parser.hpp>

using namespace swatch::logger;
using swatch::processor::test::DummyProcessor;

BOOST_AUTO_TEST_SUITE( SystemConstructionTestSuite )

BOOST_AUTO_TEST_CASE(BuildMiniSystem) {// this is not a test
    LOG(kInfo) << "Running SystemConstructionTestSuite/BuildMiniSystem";
    // TODO: add actual tests
    using namespace boost::assign;
    using namespace swatch::core;
    using namespace swatch::system;
    using namespace swatch::processor;
    using namespace swatch::system::test;

    // XParameterSet a;
    // a.insert("requires", xdata::String("ttc;daq"))("provides", xdata::String("trg"));
    // XParameterSet a1 = a, a2 = a, a3 = a;
    // 
    // Prepare Processor definition
    ProcessorStub sPro1("calol2-10"), sPro2("calol2-13"), sPro3("demux");

    sPro1.crate = "crateA";
    sPro1.slot = 1;
    addRxTxPortStubs(sPro1);
    
    sPro2.crate = "crateA";
    sPro2.slot = 2;
    addRxTxPortStubs(sPro2);

    sPro3.crate = "crateB";
    sPro3.slot = 1;
    addRxTxPortStubs(sPro3);


    BOOST_TEST_MESSAGE("Constructor");
    // TODO: improve the construction syntax?
    System* s = new System(SystemStub("calol2"));

    Crate* crateA = new Crate(CrateStub("crateA"));
    Crate* crateB = new Crate(CrateStub("crateB"));
    s->add(crateA);
    s->add(crateB);

    DummyProcessor* p1 = new DummyProcessor(sPro1);
    DummyProcessor* p2 = new DummyProcessor(sPro2);
    DummyProcessor* p3 = new DummyProcessor(sPro3);

    BOOST_TEST_MESSAGE("Adoption");
    s->add(p1);
    s->add(p2);
    s->add(p3);
    
    // ProcessorBag b1, b2, b3;
    // b1.bag.crate = xdata::String("crateA");
    // b1.bag.slot = xdata::UnsignedInteger(1);
    // addRxTxPortStubs(b1.bag);
    // a1.add("stub", b1);

    // b2.bag.crate = xdata::String("crateA");
    // b2.bag.slot = xdata::UnsignedInteger(2);
    // addRxTxPortStubs(b2.bag);
    // a2.add("stub", b2);

    // b3.bag.crate = xdata::String("crateB");
    // b3.bag.slot = xdata::UnsignedInteger(1);
    // addRxTxPortStubs(b3.bag);
    // a3.add("stub", b3);


    // BOOST_TEST_MESSAGE("Constructor");
    // System* s = new System("calol2");

    // Crate* crateA = new Crate("crateA");
    // Crate* crateB = new Crate("crateB");
    // s->add(crateA);
    // s->add(crateB);

    // DummyProcessor* p1 = new DummyProcessor("calol2-10", a1);
    // DummyProcessor* p2 = new DummyProcessor("calol2-13", a2);
    // DummyProcessor* p3 = new DummyProcessor("demux", a3);

    // BOOST_TEST_MESSAGE("Adoption");
    // s->add(p1);
    // s->add(p2);
    // s->add(p3);

    std::vector< std::pair<std::string, std::string> > links;
    push_back(links)
            ("calol2-10.ports.tx00", "demux.ports.rx01")
            ("calol2-13.ports.tx01", "demux.ports.rx00");
    
    unsigned int lid;
    std::vector< std::pair<std::string, std::string> >::iterator it;
    for (it = links.begin(), lid = 0; it != links.end(); ++it, ++lid) {
        OutputPort* src = s->getObj<OutputPort>(it->first);
        InputPort* dst = s->getObj<InputPort>(it->second);

        std::stringstream lname;
        lname << "link" << std::setw(3) << std::setfill('0') << lid;
        Link* lLink = new Link(lname.str(), src, dst);
        s->add(lLink);
    }
    
    // Add more tests here
    // Get a link from system
    // Get dst path
    // Get src path

    BOOST_TEST_MESSAGE("Destructor");
    delete s;
}

BOOST_AUTO_TEST_CASE(BuildFullCrate) {
  LOG(kInfo) << "Running SystemConstructionTestSuite/BuildFullCrate";
    using namespace boost::assign;
    using namespace swatch::core;
    using namespace swatch::system;
    using namespace swatch::processor;
    using namespace swatch::system::test;
    using std::cout;
    using std::endl;

    // Create a System
    BOOST_TEST_MESSAGE("Constructor");

    // TODO: Improve constructor syntax
    System* lSystem = new System(SystemStub("calol2"));
    
    // Crate comes first
    Crate* lCrate = new Crate(CrateStub("s2x3g18"));
    
    lSystem->add(lCrate);
    
    // AMC13
    DaqTTCStub sAmc13("AMC13-1");
    sAmc13.crate = "s2x3g18";
    sAmc13.slot = 13;

    DaqTTCManager* dtm = new DummyAMC13Manager(sAmc13);

    // AMC13
    // XParameterSet params13;

    // DaqTTCBag bag13;
    // bag13.bag.crate = xdata::String("s2x3g18");
    // bag13.bag.slot = xdata::UnsignedInteger(13);

    // params13.insert("requires", xdata::String(""))
    //     ("provides", xdata::String("ttc;daq"))
    //     ("stub", bag13);

    // DaqTTCManager* amc13 = new DummyAMC13Manager("amc13xg", params13);
    
    BOOST_TEST_MESSAGE("amc13 crate = "+dtm->getCrateId());
    BOOST_CHECK(dtm->getCrateId() == "s2x3g18" );

    ProcessorStub sDummy("dummy");
    sDummy.crate = "s2x3g18";

    // And a set of boards
    // XParameterSet paramsP;
    // ProcessorBag bagP;
    // bagP.bag.crate = xdata::String("s2x3g18");
    // paramsP.insert("requires", xdata::String("ttc;daq"))
    //     ("provides", xdata::String("trg"))
    //     ("stub", bagP);

    std::vector<DummyProcessor*> dummies;
    for (int s(1); s <= 12; ++s) {
        // XParameterSet a = paramsP; // copy the common attributes
        ProcessorStub sPro = sDummy;
        const std::string slot = boost::lexical_cast<std::string>(s);
        sPro.id = "pro-" + slot;
        sPro.slot = s;
        DummyProcessor* p = new DummyProcessor(sPro);
        dummies.push_back(p);
        BOOST_CHECK(p->getCrateId() == "s2x3g18" );
    }
    
    // Don't forget the services
    BOOST_TEST_MESSAGE("Adoption");
    lSystem->add( dtm );
    
    std::vector<DummyProcessor*>::iterator dIt;
    for (dIt = dummies.begin(); dIt != dummies.end(); ++dIt) {
//        BOOST_TEST_MESSAGE(">> Slot" << (*dIt)->getSlot());
        lSystem->add(*dIt);
    }

    Crate* crate = lSystem->getObj<Crate>("s2x3g18");
    
    // Was correct crate was created?
    BOOST_CHECK(crate->getId() == "s2x3g18" );
    
    // Was one and only crate was created
    // TODO
    
    // Is the correct number of slots populated?
    BOOST_CHECK(crate->getPopulatedSlots().size() == 13);
    
    // Were the objs assigned to the correct slots?
    BOOST_CHECK(crate->amc(1) == dummies[0]);
    BOOST_CHECK(crate->amc(7) == dummies[6]);
    
    // And the amc13?
    BOOST_CHECK(crate->amc13() == dtm);
    
    // Are the objs accessible with the crate aliases?
    BOOST_CHECK( (& crate->getObj("amc01")) == dummies[0]);
    BOOST_CHECK( (& crate->getObj("amc07")) == dummies[6]);
    BOOST_CHECK( (& crate->getObj("amc13")) == dtm);
    
    
    BOOST_TEST_MESSAGE("Destructor");
    delete lSystem;
}

BOOST_AUTO_TEST_SUITE_END()
