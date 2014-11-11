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

BOOST_AUTO_TEST_SUITE_END()
