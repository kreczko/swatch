/*
 * CrateTests.cpp
 *
 *  Created on: 9 Oct 2014
 *      Author: kreczko
 */


// Boost Headers
#include <boost/assign.hpp>
#include <boost/shared_ptr.hpp>

// Std Headers
#include <iomanip>

// Swatch Headers
#include "swatch/core/ParameterSet.hpp"
#include "swatch/processor/Processor.hpp"
#include "swatch/system/Crate.hpp"
#include "swatch/system/test/DummyProcessor.hpp"
#include "swatch/system/test/DummyAMC13Service.hpp"


#include <boost/test/unit_test.hpp>
using namespace boost::assign;
using namespace swatch::core;
using namespace swatch::system;
using namespace swatch::system::test;
using namespace std;

BOOST_AUTO_TEST_SUITE( CrateTestSuite )

BOOST_AUTO_TEST_CASE(SlotCanOnlyBeTakenOnce) {
	Crate* crate = new Crate("myCrate");

	ParameterSet a1, a2;
	a1.insert("myCrate", "crateA") ("slot", 1);
	a2.insert("myCrate", "crateA") ("slot", 1);
	DummyProcessor* p1 = new DummyProcessor("calol2-10", a1);
	DummyProcessor* p2 = new DummyProcessor("calol2-13", a2);

	crate->add(p1);
	BOOST_CHECK_EQUAL(crate->isSlotTaken(1), true);
	BOOST_CHECK_THROW(crate->add(p2), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(SlotOutOfRange) {
	Crate* crate = new Crate("myCrate");

	ParameterSet a1, a2;
	a1.insert("myCrate", "crateA") ("slot", 99999);
	DummyProcessor* p1 = new DummyProcessor("calol2-10", a1);

	BOOST_CHECK_THROW(crate->add(p1), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(PassesParamsToObject) {
	ParameterSet params  = ParameterSet();
	params.insert("name", "crate1");
	params.insert("type", "test-crate");
	Crate * crate = new Crate("dev1", params);
	ParameterSet pset = crate->pset();
	BOOST_CHECK_EQUAL(pset.has("name"), true);
	BOOST_CHECK_EQUAL(pset.has("type"), true);
	BOOST_CHECK_EQUAL(pset.get<string>("name"), "crate1");
	BOOST_CHECK_EQUAL(pset.get<string>("type"), "test-crate");
}

BOOST_AUTO_TEST_SUITE_END()
