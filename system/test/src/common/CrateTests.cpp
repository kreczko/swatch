/*
 * CrateTests.cpp
 *
 *  Created on: 9 Oct 2014
 *      Author: kreczko
 */


// Boost Headers
#include <boost/assign.hpp>
#include <boost/shared_ptr.hpp>

// C++ Headers
#include <iomanip>

// Swatch Headers
#include "swatch/core/XParameterSet.hpp"
#include "swatch/processor/Processor.hpp"
#include "swatch/system/Crate.hpp"
#include "swatch/system/test/DummyProcessor.hpp"
#include "swatch/system/test/DummyAMC13Service.hpp"

// XDAQ Headers
#include "xdata/String.h"
#include "xdata/Integer.h"

#include <boost/test/unit_test.hpp>
using namespace boost::assign;
using namespace swatch::core;
using namespace swatch::system;
using namespace swatch::system::test;
using namespace std;

BOOST_AUTO_TEST_SUITE( CrateTestSuite )

BOOST_AUTO_TEST_CASE(SlotCanOnlyBeTakenOnce) {
	Crate* crate = new Crate("myCrate");

	XParameterSet a1, a2;
	a1.insert("myCrate", xdata::String("crateA")) ("slot", xdata::Integer(1));
	a2.insert("myCrate", xdata::String("crateA")) ("slot", xdata::Integer(1));
	DummyProcessor* p1 = new DummyProcessor("calol2-10", a1);
	DummyProcessor* p2 = new DummyProcessor("calol2-13", a2);

	crate->add(p1);
	BOOST_CHECK_EQUAL(crate->isSlotTaken(1), true);
	BOOST_CHECK_THROW(crate->add(p2), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(SlotOutOfRange) {
	Crate* crate = new Crate("myCrate");

	XParameterSet a1, a2;
	a1.insert("myCrate", xdata::String("crateA")) ("slot", xdata::Integer(99999));
	DummyProcessor* p1 = new DummyProcessor("calol2-10", a1);

	BOOST_CHECK_THROW(crate->add(p1), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(PassesParamsToObject) {
	XParameterSet params  = XParameterSet();
	params.insert("name", xdata::String("crate1"));
	params.insert("type", xdata::String("test-crate"));
	Crate * crate = new Crate("dev1", params);
	XParameterSet pset = crate->pset();
	BOOST_CHECK_EQUAL(pset.has("name"), true);
	BOOST_CHECK_EQUAL(pset.has("type"), true);
	BOOST_CHECK(pset.get<xdata::String>("name") == "crate1");
	BOOST_CHECK(pset.get<xdata::String>("type") == "test-crate");
}

BOOST_AUTO_TEST_SUITE_END()
