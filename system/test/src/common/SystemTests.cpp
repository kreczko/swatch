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


#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE( SystemTestSuite )

BOOST_AUTO_TEST_CASE(BuildSystem) {
    using namespace boost::assign;
    using namespace swatch::core;
    using namespace swatch::system;
    using namespace swatch::system::test;


    ParameterSet a;
    a.insert("requires", "ttc;daq")("provides", "trg");
    ParameterSet a1 = a, a2 = a, a3 = a;
    a1.insert("crate", "crateA") ("slot", 1);
    a2.insert("crate", "crateA") ("slot", 2);
    a3.insert("crate", "crateB") ("slot", 1);

    BOOST_TEST_MESSAGE("Constructor");
    System* s = new System("calol2");

    DummyProcessor* p1 = new DummyProcessor("calol2-10", a1);
    DummyProcessor* p2 = new DummyProcessor("calol2-13", a2);
    DummyProcessor* p3 = new DummyProcessor("demux", a3);

    BOOST_TEST_MESSAGE("Adoption");
    s->add(p1);
    s->add(p2);
    s->add(p3);

    std::vector< std::pair<std::string, std::string> > links;
    push_back(links)
            ("calol2-10.tx00", "demux.rx01")
            ("calol2-13.tx01", "demux.rx00");
    
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

BOOST_AUTO_TEST_CASE(BuildCrate) {
    // Local Namespace Resolution
    using namespace swatch::core;
    using namespace swatch::system;
    using namespace swatch::system::test;
    
    using std::cout;
    using std::endl;

    // Create a System
    BOOST_TEST_MESSAGE("Constructor");
    System* lSystem = new System("calol2");
    
    // AMC13 comes first
    ParameterSet params13;
    params13.insert("requires", "")("provides", "ttc;daq")("crate", "s2x3g18");
    AMC13Service* amc13 = new DummyAMC13Service("amc13xg", params13);
    BOOST_CHECK(amc13->getCrateId() == "s2x3g18" );

    // And a set of boards
    ParameterSet params;
    params.insert("requires", "ttc;daq")("provides", "trg")("crate", "s2x3g18");
    std::vector<DummyProcessor*> dummies;
    for (int s(1); s <= 12; ++s) {
        ParameterSet a = params; // copy the common attributes
        const std::string slot = boost::lexical_cast<std::string>(s);
        a.insert("slot", s);
        DummyProcessor* p = new DummyProcessor("mp7-" + slot, a);
        dummies.push_back(p);
        BOOST_CHECK(p->getCrateId() == "s2x3g18" );
    }
    
    // Don't forget the services
    BOOST_TEST_MESSAGE("Adoption");
    lSystem->add( amc13 );
    
    std::vector<DummyProcessor*>::iterator dIt;
    for (dIt = dummies.begin(); dIt != dummies.end(); ++dIt) {
//        BOOST_TEST_MESSAGE(">> Slot" << (*dIt)->getSlot());
        lSystem->add(*dIt);
    }

    Crate* crate = lSystem->getObj<Crate>("s2x3g18");
    
    // Was correct crate was created?
    BOOST_CHECK(crate->id() == "s2x3g18" );
    
    // Was one and only crate was created
    // TODO
    
    // Is the correct number of slots populated?
    BOOST_CHECK(crate->getPopulatedSlots().size() == 13);
    
    // Were the objs assigned to the correct slots?
    BOOST_CHECK(crate->amc(1) == dummies[0]);
    BOOST_CHECK(crate->amc(7) == dummies[6]);
    
    // And the amc13?
    BOOST_CHECK(crate->amc13() == amc13);
    
    // Are the objs accessible with the crate aliases?
    BOOST_CHECK(crate->getObj("amc01") == dummies[0]);
    BOOST_CHECK(crate->getObj("amc07") == dummies[6]);
    BOOST_CHECK(crate->getObj("amc13") == amc13);
    
    
    BOOST_TEST_MESSAGE("Destructor");
    delete lSystem;
}

BOOST_AUTO_TEST_SUITE_END()
