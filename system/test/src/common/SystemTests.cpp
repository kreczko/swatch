#include <boost/test/unit_test.hpp>

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
#include "swatch/core/XParameterSet.hpp"
#include "swatch/core/Object.hpp"
#include "swatch/core/Utilities.hpp"
#include "swatch/core/Link.hpp"
#include "swatch/core/Port.hpp"

#include "swatch/processor/ProcessorStub.hpp"
#include "swatch/processor/Processor.hpp"
#include "swatch/processor/ProcessorFactory.hpp"

#include "swatch/system/System.hpp"
#include "swatch/system/SystemFactory.hpp"
#include "swatch/system/Crate.hpp"
#include "swatch/system/CrateStub.hpp"
#include "swatch/system/AMC13ServiceStub.hpp"
#include "swatch/system/test/DummyProcessor.hpp"
#include "swatch/system/test/DummyAMC13Service.hpp"

// XDAQ Headers
#include "xdata/String.h"
#include "xdata/Integer.h"

using namespace boost::assign;
//using namespace swatch::core;
//using namespace swatch::system;
// using namespace swatch::processor;
using namespace swatch::system::test;

namespace swco  = swatch::core;
namespace swpro = swatch::processor;
namespace swsys = swatch::system;

struct Params {
		Params() : ps_system(), ps_processors(), ps_services() {

      /*
			// based on swatch/test/etc/testdb.json
			ps_system.insert("name", "calol2");

            
            swsys::CrateStub crtAStub;            
            crtAStub.name = "crateA";
            crtAStub.location = "MyCrateLocation";
            crtAStub.description = "MyCrateDescription";
            
            swsys::CrateStub crtBStub;
            crtBStub.name = "crateB";
            crtBStub.location = "AnotherLocation";
            crtBStub.description = "AnotherDescription";

            swco::XParameterSet cA, cB;
            cA.set("name",crtAStub.name);
            cA.set("stub", crtAStub);

            cB.set("name",crtBStub.name);
            cB.set("stub", crtBStub);
            
            ps_crates += cA, cB;
            
			swpro::ProcessorStub proStubTmpl;
            
            proStubTmpl.creator = "DummyProcessor";
            proStubTmpl.addressTable = "/address/table/location";
			
		    proStubTmpl.name = "";
    		proStubTmpl.creator = "DummyProcessor";
    		proStubTmpl.addressTable = "/address/table/location";
    		proStubTmpl.uri = "";
    		proStubTmpl.crate = "crateA";
    		proStubTmpl.slot = 0;
    
            swco::XParameterSet p1, p2, p3;
		    swpro::ProcessorStub st1(proStubTmpl), st2(proStubTmpl), st3(proStubTmpl);

            st1.name = "MP-1";
            st1.slot = 1;
            st1.uri = "ipbusudp-2.0://127.0.0.1:50001";
            
            p1.set("name",st1.name);
            p1.set("class",st1.creator);
            p1.set("descriptor",st1);

            st2.name = "MP-2";
            st2.slot = 2;
            st2.uri = "ipbusudp-2.0://127.0.0.1:50002";

            p2.set("name",st2.name);
            p2.set("class",st2.creator);
            p2.set("descriptor",st2);

            st3.name = "MP-3";
            st3.slot = 3;
            st3.uri = "ipbusudp-2.0://127.0.0.1:50003";

            p3.set("name",st3.name);
            p3.set("descriptor",st3);
            p3.set("class",st3.creator);
            
            ps_processors += p1,p2,p3;

            swsys::AMC13ServiceStub amc13Stub, fakeMCHStub;

            amc13Stub.name    = "AMC13-1";
            amc13Stub.creator = "DummyAMC13Service";
			amc13Stub.uriT1   = "ipbusudp-2.0://127.0.0.1:50014";
			amc13Stub.addressTableT1 = "/address/table/location";
			amc13Stub.uriT2   = "ipbusudp-2.0://127.0.0.1:50014";  
			amc13Stub.addressTableT2 = "/address/table/location";
			amc13Stub.crate   = "crateA";     
			amc13Stub.slot    = 13;       
            
			swco::XParameterSet srv1, srv2;
            srv1.set("name", amc13Stub.name);
            srv1.set("class", amc13Stub.creator);
            srv1.set("descriptor", amc13Stub);
            
            fakeMCHStub = amc13Stub;
            fakeMCHStub.name = "MCH-1";
            fakeMCHStub.slot = 14;
            
            srv2.set("name", fakeMCHStub.name);
            srv2.set("class", fakeMCHStub.creator);
            srv2.set("descriptor", fakeMCHStub);

			ps_services.push_back(srv1);
			ps_services.push_back(srv2);

			ps_system.insert("crates", ps_crates);
			ps_system.insert("processors", ps_processors);
			ps_system.insert("services", ps_services);
      */
		}
		~Params(){

		}

		swco::XParameterSet ps_system;
    std::vector<swco::XParameterSet> ps_crates;
		std::vector<swco::XParameterSet> ps_processors;
		std::vector<swco::XParameterSet> ps_services;
};

BOOST_AUTO_TEST_SUITE( SystemTestSuite )

BOOST_FIXTURE_TEST_CASE(BuildSystemWithDefaultCreator, Params){
	swsys::System * system = swsys::SystemFactory::get()->make("SystemLoggingCreator", ps_system.get<xdata::String>("name"), ps_system);
	BOOST_CHECK_EQUAL(system->id(), "calol2");
	BOOST_CHECK_EQUAL(system->getProcessors().size(), size_t(3));
	BOOST_CHECK_EQUAL(system->getServices().size(), size_t(2));
	// detailed tests for the content of processors and services
	// should be done in the respective Creator tests.
}

BOOST_AUTO_TEST_CASE(AddCrate) {
	swsys::Crate * crate = new swsys::Crate("myCrate");
	swsys::System * system = new swsys::System("mySystem");
	system->add(crate);
	swsys::Crate * stored_crate = system->getObj<swsys::Crate>("myCrate");
	BOOST_CHECK_EQUAL(crate->id(), stored_crate->id() );
}

BOOST_AUTO_TEST_CASE(AddCrateToMap) {
	swsys::Crate * crate = new swsys::Crate("myCrate");
	swsys::System * system = new swsys::System("mySystem");
	system->add(crate);
	boost::unordered_map<std::string, swsys::Crate*> crates = system->getCrates();
	// check if map contains the object ID
	bool is_crate_in_map = crates.find(crate->id()) != crates.end();
	BOOST_CHECK_EQUAL(is_crate_in_map, true );
}

BOOST_AUTO_TEST_CASE(AddCrateShouldNotOverwrite) {
	swsys::Crate * crateA = new swsys::Crate("myCrateA");
	swsys::Crate * crateAprime = new swsys::Crate("myCrateA");

	swco::XParameterSet params;
	params.insert("requires", xdata::String("ttc;daq"))
        ("provides", xdata::String("trg"));
	params.insert("crate", xdata::String("crateA"))
        ("slot", xdata::Integer(1));
	DummyProcessor* p = new DummyProcessor("calol2-10", params);
	crateA->add(p);
	BOOST_CHECK_EQUAL(crateA->getPopulatedSlots().size(), size_t(1));

	swsys::System * system = new swsys::System("mySystem");
	system->add(crateA);
	BOOST_CHECK_THROW(system->add(crateAprime), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(AddCrateNULLPointerShouldThrowException) {
	swsys::System * system = new swsys::System("mySystem");
	BOOST_CHECK_THROW(system->add((swsys::Crate*) NULL), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(AddProcessorNULLPointerShouldThrowException) {
	swsys::System * system = new swsys::System("mySystem");
	BOOST_CHECK_THROW(system->add((swpro::Processor*) NULL), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(AddServiceNULLPointerShouldThrowException) {
	swsys::System * system = new swsys::System("mySystem");
	BOOST_CHECK_THROW(system->add((swsys::Service*) NULL), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(AddAMC13NULLPointerShouldThrowException) {
	swsys::System * system = new swsys::System("mySystem");
	BOOST_CHECK_THROW(system->add((swsys::DaqTTCService*) NULL), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(AddLinkNULLPointerShouldThrowException) {
	swsys::System * system = new swsys::System("mySystem");
	BOOST_CHECK_THROW(system->add((swco::Link*) NULL), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(HasCrate) {
	swsys::Crate * crate = new swsys::Crate("myCrate");
	swsys::System * system = new swsys::System("mySystem");
	system->add(crate);
	BOOST_CHECK_EQUAL(system->hasCrate("myCrate"), true );
	BOOST_CHECK_EQUAL(system->hasCrate("MyImaginaryCrate"), false );
}

BOOST_AUTO_TEST_SUITE_END()
