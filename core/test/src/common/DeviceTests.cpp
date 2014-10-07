/*
 * DeviceTests.cpp
 *
 *  Created on: 7 Oct 2014
 *      Author: kreczko
 */
// Boost Unit Test includes
#include <boost/test/unit_test.hpp>

// Swatch Headers
#include "swatch/core/ParameterSet.hpp"
#include "swatch/core/Device.hpp"

using namespace std;
using namespace swatch::core;

BOOST_AUTO_TEST_SUITE( DeviceTestSuite )

BOOST_AUTO_TEST_CASE(DeviceBug801) {
	// https://svnweb.cern.ch/trac/cactus/ticket/801
	ParameterSet params  = ParameterSet();
	params.insert("name", "dev1");
	params.insert("type", "test-dev");
	Device * dev = new Device("dev1", params);
	ParameterSet pset = dev->pset();
	BOOST_CHECK_EQUAL(pset.has("name"), true);
	BOOST_CHECK_EQUAL(pset.has("type"), true);
	BOOST_CHECK_EQUAL(pset.get<string>("name"), "dev1");
	BOOST_CHECK_EQUAL(pset.get<string>("type"), "test-dev");
}

BOOST_AUTO_TEST_SUITE_END()


