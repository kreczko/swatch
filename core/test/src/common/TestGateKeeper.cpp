/**
 * \brief Test suite for swatch/core/GateKeeper
 * \author kreczko
 */
// Boost Unit Test includes
#include <boost/test/unit_test.hpp>

// swatch headers
#include "swatch/core/GateKeeper.hpp"
#include "swatch/core/test/DummyGateKeeper.hpp"
#include "swatch/logger/Log.hpp"

//xdaq headers
#include <xdata/Integer.h>
#include <xdata/String.h>

using namespace swatch::logger;

namespace swatch {
namespace core {
namespace test {

struct TestGateKeeperSetup {
  TestGateKeeperSetup() :
    mGk()
  {
    typedef GateKeeper::Parameters_t Parameters_t;
    typedef GateKeeper::MonitoringSettings_t MonSettings_t;
    typedef GateKeeper::Masks_t Masks_t;

    GateKeeper::ParametersContext_t commonParams(new Parameters_t());
    commonParams->insert(Parameters_t::value_type("hello", new xdata::String("World")));
    commonParams->insert(Parameters_t::value_type("answer", new xdata::Integer(42)));
    mGk.addContext("dummy_sys.common", commonParams);

    GateKeeper::ParametersContext_t childA1Params(new Parameters_t());
    childA1Params->insert( Parameters_t::value_type("hello", new xdata::String("World! (childA1)")));
    mGk.addContext("dummy_sys.childA1", childA1Params);

    GateKeeper::ParametersContext_t childTypeAParams(new GateKeeper::Parameters_t());
    childTypeAParams->insert( Parameters_t::value_type("sequence_1.command_1.parameter_1", new xdata::String("sequence")));
    childTypeAParams->insert( Parameters_t::value_type("command_1.parameter_1", new xdata::String("command")));
    mGk.addContext("dummy_sys.childTypeA", childTypeAParams);

    // monitoring status
    GateKeeper::SettingsContext_t metricSettings(new GateKeeper::MonitoringSettings_t());
    metricSettings->insert( MonSettings_t::value_type("criticalMetric", new MonitoringSetting("criticalMetric",monitoring::kEnabled)));
    metricSettings->insert( MonSettings_t::value_type("weird_state.criticalMetric",
        new MonitoringSetting("weird_state.criticalMetric", monitoring::kDisabled)));
    metricSettings->insert( MonSettings_t::value_type("nonCriticalMetric", 
        new MonitoringSetting("nonCriticalMetric", monitoring::kNonCritical)));
    mGk.addSettingsContext("dummy_sys.childTypeA", metricSettings);

    // Masks
    GateKeeper::MasksContext_t commonMasks(new Masks_t());
    commonMasks->insert( "componentA" );
    mGk.addMasksContext("dummy_sys.common", commonMasks);

    GateKeeper::MasksContext_t procsMasks(new Masks_t());
    procsMasks->insert( "componentB" );
    mGk.addMasksContext("dummy_sys.childTypeA", procsMasks);
    
    GateKeeper::MasksContext_t proc1Masks(new Masks_t());
    proc1Masks->insert( "componentC" );
    mGk.addMasksContext("dummy_sys.childA1", proc1Masks);
    
    // Disable some IDs
    mGk.addDisabledId("dummy_sys.childA1");
    mGk.addDisabledId("dummy_sys.child42");
  }

  DummyGateKeeper mGk;
};


BOOST_AUTO_TEST_SUITE( TestGateKeeper )


BOOST_FIXTURE_TEST_CASE ( TestCommonParameters, TestGateKeeperSetup )
{
  std::vector<std::string> lContextsToLookIn;
  lContextsToLookIn.push_back("dummy_sys.common");

  GateKeeper::Parameter_t p(mGk.get("", "", "hello", lContextsToLookIn));
  BOOST_REQUIRE(p != NULL);
  BOOST_CHECK_EQUAL(p->toString(), "World");

  GateKeeper::Parameter_t p2(mGk.get("", "", "answer", lContextsToLookIn));
  BOOST_REQUIRE(p2 != NULL);
  BOOST_CHECK_EQUAL(p2->toString(), "42");
}

BOOST_FIXTURE_TEST_CASE ( TestChildParameters, TestGateKeeperSetup )
{
  std::vector<std::string> lContextsToLookIn {"dummy_sys.childA1", "dummy_sys.common"};

  GateKeeper::Parameter_t p(mGk.get("", "", "hello", lContextsToLookIn));
  BOOST_REQUIRE(p != NULL);
  BOOST_CHECK_EQUAL(p->toString(), "World! (childA1)");
}

BOOST_FIXTURE_TEST_CASE ( TestSequenceParameters, TestGateKeeperSetup )
{
  std::vector<std::string> lContextsToLookIn;
  lContextsToLookIn.push_back("dummy_sys.childTypeA");

  GateKeeper::Parameter_t p(
      mGk.get("sequence_1", "command_1", "parameter_1", lContextsToLookIn));
  BOOST_REQUIRE(p != NULL);
  BOOST_CHECK_EQUAL(p->toString(), "sequence");

  GateKeeper::Parameter_t p2(
      mGk.get("", "command_1", "parameter_1", lContextsToLookIn));
  BOOST_REQUIRE(p != NULL);
  BOOST_CHECK_EQUAL(p2->toString(), "command");
}

BOOST_FIXTURE_TEST_CASE ( TestMonitoringSettings, TestGateKeeperSetup )
{
  std::vector<std::string> lContextsToLookIn;
  lContextsToLookIn.push_back("dummy_sys.childTypeA");

  GateKeeper::MonitoringSetting_t p(
      mGk.getMonitoringSetting("", "nonCriticalMetric", lContextsToLookIn));
  BOOST_REQUIRE(p != NULL);
  BOOST_CHECK_EQUAL(p->getStatus(), monitoring::kNonCritical);

  GateKeeper::MonitoringSetting_t p2(
      mGk.getMonitoringSetting("", "criticalMetric", lContextsToLookIn));
  BOOST_REQUIRE(p2 != NULL);
  BOOST_CHECK_EQUAL(p2->getStatus(), monitoring::kEnabled);

  GateKeeper::MonitoringSetting_t p3(
      mGk.getMonitoringSetting("weird_state", "criticalMetric",
          lContextsToLookIn));
  BOOST_REQUIRE(p3 != NULL);
  BOOST_CHECK_EQUAL(p3->getStatus(), monitoring::kDisabled);
}


BOOST_FIXTURE_TEST_CASE(TestMask, TestGateKeeperSetup)
{
  std::vector<std::string> lContextsToLookIn;
  
  // 1) When tables vector is empty,  GateKeeper::getMask method should always return false
  BOOST_CHECK_EQUAL(mGk.getMask("componentA", lContextsToLookIn), false);
  BOOST_CHECK_EQUAL(mGk.getMask("componentB", lContextsToLookIn), false);
  BOOST_CHECK_EQUAL(mGk.getMask("componentC", lContextsToLookIn), false);
  BOOST_CHECK_EQUAL(mGk.getMask("otherComponent", lContextsToLookIn), false);
  
  // 2) Contexts vector only contains common table
  lContextsToLookIn.push_back("dummy_sys.common");
  BOOST_CHECK_EQUAL(mGk.getMask("componentA", lContextsToLookIn), true);
  BOOST_CHECK_EQUAL(mGk.getMask("componentB", lContextsToLookIn), false);
  BOOST_CHECK_EQUAL(mGk.getMask("componentC", lContextsToLookIn), false);
  BOOST_CHECK_EQUAL(mGk.getMask("otherComponent", lContextsToLookIn), false);

  // 3) Contexts vector contains proc1 and common tables
  lContextsToLookIn = std::vector<std::string>{"dummy_sys.childA1", "dummy_sys.common"};
  BOOST_CHECK_EQUAL(mGk.getMask("componentA", lContextsToLookIn), true);
  BOOST_CHECK_EQUAL(mGk.getMask("componentB", lContextsToLookIn), false);
  BOOST_CHECK_EQUAL(mGk.getMask("componentC", lContextsToLookIn), true);
  BOOST_CHECK_EQUAL(mGk.getMask("otherComponent", lContextsToLookIn), false);

  // 4) Contexts vector contains processors and common tables
  lContextsToLookIn = std::vector<std::string>{"dummy_sys.childTypeA", "dummy_sys.common"};
  BOOST_CHECK_EQUAL(mGk.getMask("componentA", lContextsToLookIn), true);
  BOOST_CHECK_EQUAL(mGk.getMask("componentB", lContextsToLookIn), true);
  BOOST_CHECK_EQUAL(mGk.getMask("componentC", lContextsToLookIn), false);
  BOOST_CHECK_EQUAL(mGk.getMask("otherComponent", lContextsToLookIn), false);

  // 5) Contexts vector contains all tables
  lContextsToLookIn = std::vector<std::string>{"dummy_sys.childA1", "dummy_sys.childTypeA", "dummy_sys.common"};
  BOOST_CHECK_EQUAL(mGk.getMask("componentA", lContextsToLookIn), true);
  BOOST_CHECK_EQUAL(mGk.getMask("componentB", lContextsToLookIn), true);
  BOOST_CHECK_EQUAL(mGk.getMask("componentC", lContextsToLookIn), true);
  BOOST_CHECK_EQUAL(mGk.getMask("otherComponent", lContextsToLookIn), false);
}


BOOST_FIXTURE_TEST_CASE(TestDisabled, TestGateKeeperSetup)
{
  BOOST_CHECK_EQUAL(mGk.isEnabled("dummy_sys.childA1"), false);
  BOOST_CHECK_EQUAL(mGk.isEnabled("dummy_sys.childA2"), true);
  BOOST_CHECK_EQUAL(mGk.isEnabled("dummy_sys.unkownChild"), true);
  BOOST_CHECK_EQUAL(mGk.isEnabled("dummy_sys.child42"), false);
}


BOOST_AUTO_TEST_SUITE_END() // TestGateKeeper

}//ns: test
}//ns: core
} //ns: swatch
