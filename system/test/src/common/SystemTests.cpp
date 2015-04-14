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
#include "swatch/core/Factory.hpp"

#include "swatch/processor/ProcessorStub.hpp"
#include "swatch/processor/Processor.hpp"

#include "swatch/system/System.hpp"
#include "swatch/system/Crate.hpp"
#include "swatch/system/CrateStub.hpp"
#include "swatch/system/DaqTTCStub.hpp"
#include "swatch/processor/test/DummyProcessor.hpp"
#include "swatch/system/test/DummyAMC13Manager.hpp"
#include "swatch/logger/Log.hpp"

// XDAQ Headers
#include "xdata/String.h"
#include "xdata/Integer.h"

using namespace boost::assign;
using namespace swatch::logger;
using namespace swatch::system::test;

namespace swco  = swatch::core;
namespace swpro = swatch::processor;
namespace swsys = swatch::system;

using swatch::processor::test::DummyProcessor;

struct Params {
    Params() :
      ps_system(),
      p1(),
      p2(),
      p3(),
      cA(),
      cB(),
      srv1(),
      srv2(),
      ps_processors(),
      ps_daqttc(),
      ps_services() {

      
        // based on swatch/test/etc/testdb.json
        ps_system.insert("name", xdata::String("calol2"));

        
        swsys::CrateBag crtABag;            
        crtABag.bag.name = xdata::String("crateA");
        crtABag.bag.location = xdata::String("MyCrateLocation");
        crtABag.bag.description = xdata::String("MyCrateDescription");
        
        swsys::CrateBag crtBBag;
        crtBBag.bag.name = xdata::String("crateB");
        crtBBag.bag.location = xdata::String("AnotherLocation");
        crtBBag.bag.description = xdata::String("AnotherDescription");

//        swco::XParameterSet cA, cB;
        cA.add("name",crtABag.bag.name);
        cA.add("stub", crtABag);

        cB.add("name",crtBBag.bag.name);
        cB.add("stub", crtBBag);
        
        ps_crates += cA, cB;
        
        swpro::ProcessorStub proStubTmpl;
        
        proStubTmpl.creator = xdata::String("swatch::processor::test::DummyProcessor");
        proStubTmpl.addressTable = xdata::String("/address/table/location");
        
        proStubTmpl.name = xdata::String("");
        proStubTmpl.creator = xdata::String("swatch::processor::test::DummyProcessor");
        proStubTmpl.addressTable = xdata::String("/address/table/location");
        proStubTmpl.uri = xdata::String("");
        proStubTmpl.crate = xdata::String("crateA");
        proStubTmpl.slot = xdata::UnsignedInteger(0);

//        swco::XParameterSet p1, p2, p3;

        swpro::ProcessorBag st1, st2, st3;
        st1.bag = proStubTmpl;
        st2.bag = proStubTmpl;
        st3.bag = proStubTmpl;

        st1.bag.name = xdata::String("MP-1");
        st1.bag.slot = xdata::UnsignedInteger(1);
        st1.bag.uri = xdata::String("ipbusudp-2.0://127.0.0.1:50001");
        
        p1.add("name",st1.bag.name);
        p1.add("class",st1.bag.creator);
        p1.add("stub",st1);

        st2.bag.name = xdata::String("MP-2");
        st2.bag.slot = xdata::UnsignedInteger(2);
        st2.bag.uri = xdata::String("ipbusudp-2.0://127.0.0.1:50002");

        p2.add("name",st2.bag.name);
        p2.add("class",st2.bag.creator);
        p2.add("stub",st2);

        st3.bag.name = xdata::String("MP-3");
        st3.bag.slot = xdata::UnsignedInteger(3);
        st3.bag.uri = xdata::String("ipbusudp-2.0://127.0.0.1:50003");

        p3.add("name",st3.bag.name);
        p3.add("stub",st3);
        p3.add("class",st3.bag.creator);
        
        ps_processors += p1,p2,p3;

        swsys::DaqTTCBag amc13Bag, fakeMCHBag;

        amc13Bag.bag.name    = xdata::String("AMC13-1");
        amc13Bag.bag.creator = xdata::String("swatch::system::test::DummyAMC13Service");
        amc13Bag.bag.uriT1   = xdata::String("ipbusudp-2.0://127.0.0.1:50014");
        amc13Bag.bag.addressTableT1 = xdata::String("/address/table/location");
        amc13Bag.bag.uriT2   = xdata::String("ipbusudp-2.0://127.0.0.1:50014");  
        amc13Bag.bag.addressTableT2 = xdata::String("/address/table/location");
        amc13Bag.bag.crate   = xdata::String("crateA");     
        amc13Bag.bag.slot    = xdata::UnsignedInteger(13);       
        
//        swco::XParameterSet srv1, srv2;
        srv1.add("name", amc13Bag.bag.name);
        srv1.add("class", amc13Bag.bag.creator);
        srv1.add("stub", amc13Bag);

        ps_daqttc.push_back(srv1);
        
        // fakeMCHBag = amc13Bag;
        // fakeMCHBag.bag.name = xdata::String("MCH-1");
        // fakeMCHBag.bag.slot = xdata::UnsignedInteger(14);
        
        // srv2.add("name", fakeMCHBag.bag.name);
        // srv2.add("class", fakeMCHBag.bag.creator);
        // srv2.add("stub", fakeMCHBag);

        // ps_services.push_back(srv2);

        ps_system.insert("crates", ps_crates);
        ps_system.insert("processors", ps_processors);
        ps_system.insert("daqttcs", ps_daqttc);
        ps_system.insert("services", ps_services);
        
  
    }
    ~Params(){

    }

    swco::XParameterSet ps_system, p1, p2, p3, cA, cB, srv1, srv2;
    xdata::Vector<swco::XParameterSet> ps_crates;
    xdata::Vector<swco::XParameterSet> ps_processors;
    xdata::Vector<swco::XParameterSet> ps_daqttc;
    xdata::Vector<swco::XParameterSet> ps_services;
};

BOOST_AUTO_TEST_SUITE( SystemTestSuite )

BOOST_FIXTURE_TEST_CASE(BuildSystemWithDefaultCreator, Params){
  LOG(kInfo) << "Running SystemTestSuite/BuildSystemWithDefaultCreator";
//    swsys::System * system = swsys::SystemFactory::get()->make("swatch::system::SystemLoggingCreator", ps_system.get<xdata::String>("name"), ps_system);
    swsys::System * system = swco::Factory::get()->bake<swsys::System>("swatch::system::SystemLoggingCreator", ps_system.get<xdata::String>("name"), ps_system);
    BOOST_CHECK_EQUAL(system->id(), "calol2");
    BOOST_CHECK_EQUAL(system->getProcessors().size(), size_t(3));
    BOOST_CHECK_EQUAL(system->getDaqTTC().size(), size_t(1));
    // detailed tests for the content of processors and services
    // should be done in the respective Creator tests.
}

BOOST_FIXTURE_TEST_CASE(AddCrate, Params) {
  LOG(kInfo) << "Running SystemTestSuite/AddCrate";
    swsys::Crate * crate = new swsys::Crate(cA.get("name").toString(), cA);
    swsys::System * system = new swsys::System("mySystem");
    system->add(crate);
    swsys::Crate * stored_crate = system->getObj<swsys::Crate>("crateA");
    BOOST_CHECK_EQUAL(crate->id(), stored_crate->id() );
}

BOOST_FIXTURE_TEST_CASE(AddCrateToMap, Params) {
  LOG(kInfo) << "Running SystemTestSuite/AddCrateToMap";
    swsys::Crate * crate = new swsys::Crate(cA.get("name").toString(), cA);
    swsys::System * system = new swsys::System("mySystem");
    system->add(crate);
    swsys::System::CratesMap crates = system->getCrates();
    // check if map contains the object ID
    bool is_crate_in_map = crates.find(crate->id()) != crates.end();
    BOOST_CHECK_EQUAL(is_crate_in_map, true );
}

BOOST_FIXTURE_TEST_CASE(AddCrateShouldNotOverwrite, Params) {
    swsys::Crate * crateA = new swsys::Crate(cA.get("name").toString(), cA);
    swsys::Crate * crateAprime = new swsys::Crate(cA.get("name").toString(), cA);


    swpro::ProcessorBag pBag;
    pBag.bag.crate = xdata::String("crateA");
    pBag.bag.slot  = xdata::UnsignedInteger(1);

    swco::XParameterSet params;
    params.insert
        ("requires", xdata::String("ttc;daq"))
        ("provides", xdata::String("trg"))
        ("stub",pBag);



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
    BOOST_CHECK_THROW(system->add((swsys::DaqTTCManager*) NULL), std::invalid_argument);
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

BOOST_FIXTURE_TEST_CASE(AddAMC13Service, Params) {
  LOG(kInfo) << "Running SystemTestSuite/AddAMC13Service";
//  swsys::DaqTTCService * service = swsys::DaqTTCFactory::get()->make(srv1);
  swsys::DaqTTCManager * service = swco::Factory::get()->bake<swsys::DaqTTCManager>(srv1);
  std::string service_name = srv1.get("name").toString();
  swsys::System * system = new swsys::System("mySystem");
  // before we can add service we need to add crateA
  swsys::Crate * crateA = new swsys::Crate(cA.get("name").toString(), cA);
  system->add(crateA);
  BOOST_CHECK_EQUAL(system->getServices().size(), size_t(0));
  BOOST_CHECK_EQUAL(system->getDaqTTC().size(), size_t(0));
  system->add(service);
  BOOST_CHECK_EQUAL(system->getServices().size(), size_t(0));
  BOOST_CHECK_EQUAL(system->getDaqTTC().size(), size_t(1));

  swsys::DaqTTCManager * stored_service = system->getObj<swsys::DaqTTCManager>(service_name);
  BOOST_CHECK_EQUAL(service->id(), stored_service->id() );
}

BOOST_AUTO_TEST_SUITE_END()
