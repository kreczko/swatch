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
#include "swatch/processor/Link.hpp"
#include "swatch/processor/Port.hpp"
#include "swatch/core/Factory.hpp"

#include "swatch/processor/ProcessorStub.hpp"
#include "swatch/processor/Processor.hpp"

#include "swatch/system/System.hpp"
#include "swatch/system/Crate.hpp"
#include "swatch/system/CrateStub.hpp"
#include "swatch/dtm/DaqTTCStub.hpp"
#include "swatch/processor/test/DummyProcessor.hpp"
#include "swatch/system/test/DummyAMC13Manager.hpp"
#include "swatch/logger/Log.hpp"

using namespace boost::assign;
using namespace swatch::logger;
using namespace swatch::system::test;

namespace swco  = swatch::core;
namespace swpro = swatch::processor;
namespace swsys = swatch::system;
namespace swdtm = swatch::dtm;

using swatch::processor::test::DummyProcessor;

struct Params {
    Params() :
    sysStub("mysystem"),
    emptyStub("empty"),

     cA("crateA"),
     cB("crateB"),
     p1("pro1"),
     p2("pro2"),
     p3("pro3"),
     dts("AMC13-1")

    {

      
        // based on swatch/test/etc/testdb.json
//        ps_system.insert("name", xdata::String("calol2"));
        sysStub.creator = "swatch::system::System";
        
        cA.location = "MyCrateLocation";
        cA.description = "MyCrateDescription";
        
        cB.location = "AnotherLocation";
        cB.description = "AnotherDescription";

        sysStub.crates += cA, cB;
        
        // Define a processor template
        swpro::ProcessorStub pTmpl("template");
        
        pTmpl.creator = "swatch::processor::test::DummyProcessor";
        pTmpl.addressTable = "/address/table/location";
        pTmpl.uri = "";
        pTmpl.crate = "crateA";
        pTmpl.slot = 0;

        // Create 3 processor stubs
//        swpro::ProcessorStub st1(pTmpl), st2(pTmpl), st3(pTmpl);
        
        p1 = p2 = p3 = pTmpl;
        
        
        p1.id = "MP-1";
        p1.slot = 1;
        p1.uri = "ipbusudp-2.0://127.0.0.1:50001";

        p2.id = "MP-2";
        p2.slot = 2;
        p2.uri = "ipbusudp-2.0://127.0.0.1:50002";

        p3.id = "MP-3";
        p3.slot = 3;
        p3.uri = "ipbusudp-2.0://127.0.0.1:50003";
        
        sysStub.processors += p1,p2,p3;

        // swsys::DaqTTCStub dtm("AMC13-1");

        dts.creator = "swatch::system::test::DummyAMC13Manager";
        dts.uriT1   = "ipbusudp-2.0://127.0.0.1:50014";
        dts.addressTableT1 = "/address/table/location";
        dts.uriT2   = "ipbusudp-2.0://127.0.0.1:50014"; 
        dts.addressTableT2 = "/address/table/location";
        dts.crate   = "crateA";
        dts.slot    = 13;       
        

        sysStub.daqttcs += dts;
    }
    
    
    ~Params(){

    }


    swsys::SystemStub sysStub, emptyStub;
    swsys::CrateStub cA, cB;
    swpro::ProcessorStub p1,p2,p3;
    swdtm::DaqTTCStub dts;

};

BOOST_AUTO_TEST_SUITE( SystemTestSuite )

BOOST_FIXTURE_TEST_CASE(BuildSystemWithDefaultCreator, Params){
  LOG(kInfo) << "Running SystemTestSuite/BuildSystemWithDefaultCreator";
//    swsys::System * system = swsys::SystemFactory::get()->make("swatch::system::SystemLoggingCreator", ps_system.get<xdata::String>("name"), ps_system);
    // swsys::System * system = swco::Factory::get()->make<swsys::System>("swatch::system::SystemLoggingCreator", ps_system.get<xdata::String>("name"), ps_system);
    swsys::System system(sysStub);
    BOOST_CHECK_EQUAL(system.getId(), "mysystem");
    BOOST_CHECK_EQUAL(system.getProcessors().size(), size_t(3));
    BOOST_CHECK_EQUAL(system.getDaqTTC().size(), size_t(1));
    // detailed tests for the content of processors and services
    // should be done in the respective Creator tests.
}

BOOST_FIXTURE_TEST_CASE(AddCrate, Params) {
  LOG(kInfo) << "Running SystemTestSuite/AddCrate";
    emptyStub.crates.push_back(cA);
    swsys::System * system = new swsys::System(emptyStub);
    swsys::Crate * stored_crate = system->getObj<swsys::Crate>("crateA");
    BOOST_CHECK_EQUAL(cA.id, stored_crate->getId() );
}

BOOST_FIXTURE_TEST_CASE(AddCrateToMap, Params) {
  LOG(kInfo) << "Running SystemTestSuite/AddCrateToMap";
    swsys::Crate * crate = new swsys::Crate(cA);
    emptyStub.crates.push_back(cA);
    swsys::System * system = new swsys::System(emptyStub);
    swsys::System::CratesMap crates = system->getCrates();
    // check if map contains the object ID
    bool is_crate_in_map = crates.find(crate->getId()) != crates.end();
    BOOST_CHECK_EQUAL(is_crate_in_map, true );
}

BOOST_FIXTURE_TEST_CASE(AddCrateShouldNotOverwrite, Params) {
  LOG(kInfo) << "Running SystemTestSuite/AddCrateShouldNotOverwrite";

  swpro::ProcessorStub sPro("MP-10");
  sPro.creator = "swatch::processor::test::DummyProcessor";
  sPro.crate = "crateA";
  sPro.slot  = 1;

  emptyStub.crates.push_back(cA);
  emptyStub.processors.push_back(sPro);
  swsys::System sys(emptyStub);

  const swsys::Crate* crateA = sys.getCrates().find(cA.id)->second;
  BOOST_CHECK_EQUAL(crateA->getPopulatedSlots().size(), size_t(1));

  emptyStub.crates.push_back(cA);
  BOOST_CHECK_THROW(swsys::System lSystem(emptyStub), std::runtime_error);
}

/*
BOOST_FIXTURE_TEST_CASE(AddCrateNULLPointerShouldThrowException, Params) {
    swsys::System * system = new swsys::System(emptyStub);
    BOOST_CHECK_THROW(system->add((swsys::Crate*) NULL), std::invalid_argument);
}

BOOST_FIXTURE_TEST_CASE(AddProcessorNULLPointerShouldThrowException, Params) {
    swsys::System * system = new swsys::System(emptyStub);
    BOOST_CHECK_THROW(system->add((swpro::Processor*) NULL), std::invalid_argument);
}

BOOST_FIXTURE_TEST_CASE(AddServiceNULLPointerShouldThrowException, Params) {
    swsys::System * system = new swsys::System(empty);
    BOOST_CHECK_THROW(system->add((swsys::Service*) NULL), std::invalid_argument);
}

BOOST_FIXTURE_TEST_CASE(AddAMC13NULLPointerShouldThrowException, Params) {
    swsys::System * system = new swsys::System(empty);
    BOOST_CHECK_THROW(system->add((swsys::DaqTTCManager*) NULL), std::invalid_argument);
}

BOOST_FIXTURE_TEST_CASE(AddLinkNULLPointerShouldThrowException, Params) {
    swsys::System * system = new swsys::System(empty);
    BOOST_CHECK_THROW(system->add((swpro::Link*) NULL), std::invalid_argument);
}
 */


BOOST_FIXTURE_TEST_CASE(HasCrate, Params) {
    emptyStub.crates.push_back(cA);
    swsys::System * system = new swsys::System(emptyStub);
    BOOST_CHECK_EQUAL(system->hasCrate("crateA"), true );
    BOOST_CHECK_EQUAL(system->hasCrate("MyImaginaryCrate"), false );
}

BOOST_FIXTURE_TEST_CASE(AddAMC13Service, Params) {
  LOG(kInfo) << "Running SystemTestSuite/AddAMC13Service";

  emptyStub.crates.push_back(cA);
  emptyStub.daqttcs.push_back(dts);
  swsys::System system(emptyStub);

  BOOST_CHECK_EQUAL(system.getServices().size(), size_t(0));
  BOOST_CHECK_EQUAL(system.getDaqTTC().size(), size_t(1));

  swdtm::DaqTTCManager * stored_service = system.getObj<swdtm::DaqTTCManager>(dts.id);
  BOOST_REQUIRE( stored_service != NULL );
  BOOST_CHECK_EQUAL(dts.id, stored_service->getId() );
}

BOOST_AUTO_TEST_SUITE_END()
