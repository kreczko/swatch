// Swatch Headers


// Boost Headers
#include <boost/assign.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/unordered/unordered_map.hpp>
#include <boost/foreach.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/property_tree/json_parser.hpp>

// C++ Headers
#include <iomanip>
#include <vector>
#include <map>

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
using namespace boost::assign;
using namespace swatch::core;
using namespace swatch::system;
using namespace swatch::processor;
using namespace swatch::system::test;

struct Params {
		Params() : ps_system(), ps_processors(), ps_services() {
			// based on swatch/test/etc/testdb.json
			ps_system.insert("name", "calol2");

			ParameterSet p1, p2, p3;
			p1.insert("PROCESSOR NAME", "MP-1")("PROCESSOR TYPE", "MP7");
			p1.insert("PROCESSOR CREATOR", "DummyProcessor")("ADDRESS TABLE", "/address/table/location");
			p1.insert("IP ADDRESS", "192.168.0.1")("crate", "crateA");
			p1.insert("slot", 1)("CRATE LOCATION", "MyLocation");
			p1.insert("CRATE DESCRIPTION", "MyCrateDescription")("PROCESSOR TYPE", "MP7");
			// unclear:
			// p1.insert("PORTS NAMES", ""Port#1", "Port#2", "Port#3", "Port#144"");
			p2 = p1;
			p2.set("PROCESSOR NAME", "MP-2");
			p2.set("IP ADDRESS", "192.168.0.2");
			p2.set("slot", 2);

			p3 = p1;
			p3.set("PROCESSOR NAME", "MP-3");
			p3.set("IP ADDRESS", "192.168.0.3");
			p3.set("crate", "crateB");

			ps_processors.push_back(p1);
			ps_processors.push_back(p2);
			ps_processors.push_back(p3);

			ParameterSet s1, s2;
			s1.insert("SERVICE NAME", "AMC13-1")("SERVICE TYPE", "AMC13");
			s1.insert("SERVICE CREATOR", "DummyAMC13Service")("ADDRESS TABLE", "/address/table/location");
			s1.insert("crate", "S2G17-01")("slot", 13);
			s1.insert("CRATE LOCATION", "MyCrateLocation")("CRATE DESCRIPTION", "MyCrateDescription");
			s1.insert("IP ADDRESS #1", "192.168.0.13")("IP ADDRESS #2", "192.168.0.14");

			s2.insert("SERVICE NAME", "MCH")("SERVICE TYPE", "MCH");
			s2.insert("SERVICE CREATOR", "DummyAMC13Service")("ADDRESS TABLE",
					"/address/table/location");
			s2.insert("crate", "S2G17-01")("slot", 14);
			s2.insert("CRATE LOCATION", "MyCrateLocation")("CRATE DESCRIPTION",
					"MyCrateDescription");
			s2.insert("IP ADDRESS", "192.168.0.15");
			ps_services.push_back(s1);
			ps_services.push_back(s2);

			ps_system.insert("processors", ps_processors);
			ps_system.insert("services", ps_services);
		}
		~Params(){

		}

		ParameterSet ps_system;
		std::vector<ParameterSet> ps_processors;
		std::vector<ParameterSet> ps_services;
};

BOOST_AUTO_TEST_SUITE( SystemTestSuite )

BOOST_FIXTURE_TEST_CASE(BuildSystemWithDefaultCreator, Params){
	System * system = SystemFactory::get()->make("SystemLoggingCreator", ps_system.get<std::string>("name"), ps_system);
	BOOST_CHECK_EQUAL(system->id(), "calol2");
	BOOST_CHECK_EQUAL(system->getProcessors().size(), size_t(3));
	BOOST_CHECK_EQUAL(system->getServices().size(), size_t(2));
	// detailed tests for the content of processors and services
	// should be done in the respective Creator tests.
}

BOOST_AUTO_TEST_CASE(AddCrate) {
	Crate * crate = new Crate("myCrate");
	System * system = new System("mySystem");
	system->add(crate);
	Crate * stored_crate = system->getObj<Crate>("myCrate");
	BOOST_CHECK_EQUAL(crate->id(), stored_crate->id() );
}

BOOST_AUTO_TEST_CASE(AddCrateToMap) {
	Crate * crate = new Crate("myCrate");
	System * system = new System("mySystem");
	system->add(crate);
	boost::unordered_map<std::string, Crate*> crates = system->getCrates();
	// check if map contains the object ID
	bool is_crate_in_map = crates.find(crate->id()) != crates.end();
	BOOST_CHECK_EQUAL(is_crate_in_map, true );
}

BOOST_AUTO_TEST_CASE(AddCrateShouldNotOverwrite) {
	Crate * crateA = new Crate("myCrateA");
	Crate * crateAprime = new Crate("myCrateA");

	ParameterSet params;
	params.insert("requires", "ttc;daq")("provides", "trg");
	params.insert("crate", "crateA") ("slot", 1);
	DummyProcessor* p = new DummyProcessor("calol2-10", params);
	crateA->add(p);
	BOOST_CHECK_EQUAL(crateA->getPopulatedSlots().size(), size_t(1));

	System * system = new System("mySystem");
	system->add(crateA);
	BOOST_CHECK_THROW(system->add(crateAprime), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(AddCrateNULLPointerShouldThrowException) {
	System * system = new System("mySystem");
	BOOST_CHECK_THROW(system->add((Crate*) NULL), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(AddProcessorNULLPointerShouldThrowException) {
	System * system = new System("mySystem");
	BOOST_CHECK_THROW(system->add((Processor*) NULL), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(AddServiceNULLPointerShouldThrowException) {
	System * system = new System("mySystem");
	BOOST_CHECK_THROW(system->add((Service*) NULL), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(AddAMC13NULLPointerShouldThrowException) {
	System * system = new System("mySystem");
	BOOST_CHECK_THROW(system->add((AMC13Service*) NULL), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(AddLinkNULLPointerShouldThrowException) {
	System * system = new System("mySystem");
	BOOST_CHECK_THROW(system->add((Link*) NULL), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(HasCrate) {
	Crate * crate = new Crate("myCrate");
	System * system = new System("mySystem");
	system->add(crate);
	BOOST_CHECK_EQUAL(system->hasCrate("myCrate"), true );
	BOOST_CHECK_EQUAL(system->hasCrate("MyImaginaryCrate"), false );
}

BOOST_AUTO_TEST_CASE(BuildSystem) {// this is not a test
    ParameterSet a;
    a.insert("requires", "ttc;daq")("provides", "trg");
    ParameterSet a1 = a, a2 = a, a3 = a;
    a1.insert("crate", "crateA") ("slot", 1);
    a2.insert("crate", "crateA") ("slot", 2);
    a3.insert("crate", "crateB") ("slot", 1);

    BOOST_TEST_MESSAGE("Constructor");
    System* s = new System("calol2");

    Crate* crateA = new Crate("crateA");
    Crate* crateB = new Crate("crateB");
    s->add(crateA);
    s->add(crateB);

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
    using std::cout;
    using std::endl;

    // Create a System
    BOOST_TEST_MESSAGE("Constructor");
    System* lSystem = new System("calol2");
    // crate comes first
    Crate* the_crate = new Crate("s2x3g18");
    lSystem->add(the_crate);
    // AMC13
    ParameterSet params13;
    params13.insert("requires", "")("provides", "ttc;daq")("crate", "s2x3g18")("slot",13);
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

BOOST_AUTO_TEST_CASE(ExploreSystem) {
    using namespace boost::assign;
    using namespace std;

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
} 

BOOST_AUTO_TEST_SUITE_END()
