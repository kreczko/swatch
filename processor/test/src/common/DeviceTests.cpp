// /*
//  * DeviceTests.cpp
//  *
//  *  Created on: 7 Oct 2014
//  *      Author: kreczko
//  */
// // Boost Unit Test includes
// #include <boost/test/unit_test.hpp>
// 
// //#include "xdata/String.h"
// // XDAQ Headers
// #include <xdata/String.h>
// 
// // Swatch Headers
// #include "swatch/core/XParameterSet.hpp"
// #include "swatch/processor/LinkInterface.hpp"
// #include "swatch/core/xoperators.hpp"
// 
// using namespace std;
// using namespace swatch::core;
// 
// BOOST_AUTO_TEST_SUITE( DeviceTestSuite )
// 
// BOOST_AUTO_TEST_CASE(DeviceBug801) {
// 	// https://svnweb.cern.ch/trac/cactus/ticket/801
// 	XParameterSet params  = XParameterSet();
// 	params.insert("name", xdata::String("dev1"));
// 	params.insert("type", xdata::String("test-dev"));
// 	Device * dev = new LinkInterface();
// 	const XParameterSet& pset = dev->pset();
// 	BOOST_CHECK_EQUAL(pset.has("name"), true);
// 	BOOST_CHECK_EQUAL(pset.has("type"), true);
//   BOOST_CHECK(pset.get<xdata::String>("name") == "dev1");
// 	BOOST_CHECK(pset.get<xdata::String>("type") == "test-dev");
// 
// }
// 
// BOOST_AUTO_TEST_SUITE_END()
// 
// 
