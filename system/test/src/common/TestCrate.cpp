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
#include "swatch/processor/ProcessorStub.hpp"
#include "swatch/system/Crate.hpp"
#include "swatch/system/CrateStub.hpp"
#include "swatch/processor/test/DummyProcessor.hpp"
#include "swatch/system/test/DummyAMC13Manager.hpp"
#include "swatch/logger/Log.hpp"

// XDAQ Headers
#include "xdata/String.h"
#include "xdata/Integer.h"

#include <boost/test/unit_test.hpp>
using namespace boost::assign;
using namespace swatch::core;
using namespace swatch::logger;
using namespace swatch::system;
using namespace swatch::processor;
using namespace swatch::system::test;
using namespace std;

using swatch::processor::test::DummyProcessor;

BOOST_AUTO_TEST_SUITE( CrateTestSuite )

BOOST_AUTO_TEST_CASE(SlotCanOnlyBeTakenOnce)
{
  CrateStub cs("crateA");
	Crate* crate = new Crate(cs);

//	XParameterSet a1, a2;
	ProcessorStub ps1("proc1"), ps2("proc2");
	ps1.crate = "crateA";
	ps1.slot =  1;
	ps2.crate = "crateA";
	ps2.slot =  1;


	DummyProcessor* p1 = new DummyProcessor(ps1);
	DummyProcessor* p2 = new DummyProcessor(ps2);

	crate->add(p1);
	BOOST_CHECK_EQUAL(crate->isAMCSlotTaken(1), true);
	BOOST_CHECK_THROW(crate->add(p2), CrateSlotTaken);
}

BOOST_AUTO_TEST_CASE(SlotOutOfRange)
{
  CrateStub cs("crateA");
	Crate* crate = new Crate(cs);

  ProcessorStub ps("proc");
	ps.crate = "crateA";
	ps.slot =  999999;
	DummyProcessor* p1 = new DummyProcessor(ps);

	BOOST_CHECK_THROW(crate->add(p1), CrateSlotOutOfRange);
}

BOOST_AUTO_TEST_SUITE_END()
