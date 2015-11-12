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
          mGk() {
    GateKeeper::tTable common(new GateKeeper::tParameters());
    common->insert(GateKeeper::tParameters::value_type("hello", GateKeeper::tParameter(new xdata::String("World"))));
    common->insert(GateKeeper::tParameters::value_type("answer", GateKeeper::tParameter(new xdata::Integer(42))));
    mGk.addTable("dummy_sys.common", common);

    GateKeeper::tTable child1(new GateKeeper::tParameters());
    child1->insert(
        GateKeeper::tParameters::value_type("monitoring_status", GateKeeper::tParameter(new xdata::Integer(0))));
    mGk.addTable("dummy_sys.child1", child1);

    GateKeeper::tTable processors(new GateKeeper::tParameters());
    processors->insert(
        GateKeeper::tParameters::value_type("sequence_1.command_1.parameter_1",
            GateKeeper::tParameter(new xdata::String("sequence"))));
    processors->insert(
        GateKeeper::tParameters::value_type("command_1.parameter_1",
            GateKeeper::tParameter(new xdata::String("command"))));
    mGk.addTable("dummy_sys.processors", processors);

    // monitoring status
    GateKeeper::tSettingsTable metricSettings(new GateKeeper::tMonitoringSettings());
    metricSettings->insert(
        GateKeeper::tMonitoringSettings::value_type("criticalMetric",
            GateKeeper::tMonitoringSetting(new MonitoringSetting("criticalMetric",monitoring::kEnabled))));
    metricSettings->insert(
        GateKeeper::tMonitoringSettings::value_type("weird_state.criticalMetric",
            GateKeeper::tMonitoringSetting(
                new MonitoringSetting("weird_state.criticalMetric", monitoring::kDisabled))));
    metricSettings->insert(
        GateKeeper::tMonitoringSettings::value_type("nonCriticalMetric",
            GateKeeper::tMonitoringSetting(new MonitoringSetting("nonCriticalMetric", monitoring::kNonCritical))));

    mGk.addSettingsTable("dummy_sys.processors", metricSettings);

  }
  DummyGateKeeper mGk;
};

BOOST_AUTO_TEST_SUITE( TestGateKeeper )

BOOST_FIXTURE_TEST_CASE ( TestCommonParameters, TestGateKeeperSetup ) {
  LOG(kInfo) << "Running TestGateKeeper/TestCommonParameters";

  std::vector<std::string> tablesToLookIn;
  tablesToLookIn.push_back("dummy_sys.common");

  GateKeeper::tParameter p(mGk.get("", "", "hello", tablesToLookIn));
  BOOST_CHECK_EQUAL(p->toString(), "World");

  GateKeeper::tParameter p2(mGk.get("", "", "answer", tablesToLookIn));
  BOOST_CHECK_EQUAL(p2->toString(), "42");
}

BOOST_FIXTURE_TEST_CASE ( TestChildParameters, TestGateKeeperSetup ) {
  LOG(kInfo) << "Running TestGateKeeper/TestChildParameters";

  std::vector<std::string> tablesToLookIn;
  tablesToLookIn.push_back("dummy_sys.child1");

  GateKeeper::tParameter p(
      mGk.get("", "", "monitoring_status", tablesToLookIn));
  BOOST_CHECK_EQUAL(p->toString(), "0");
}

BOOST_FIXTURE_TEST_CASE ( TestSequenceParameters, TestGateKeeperSetup ) {
  LOG(kInfo) << "Running TestGateKeeper/TestSequenceParameters";

  std::vector<std::string> tablesToLookIn;
  tablesToLookIn.push_back("dummy_sys.processors");

  GateKeeper::tParameter p(
      mGk.get("sequence_1", "command_1", "parameter_1", tablesToLookIn));
  BOOST_CHECK_EQUAL(p->toString(), "sequence");

  GateKeeper::tParameter p2(
      mGk.get("", "command_1", "parameter_1", tablesToLookIn));
  BOOST_CHECK_EQUAL(p2->toString(), "command");
}

BOOST_FIXTURE_TEST_CASE ( TestMonitoringSettings, TestGateKeeperSetup ) {
  LOG(kInfo) << "Running TestGateKeeper/TestMonitoringSettings";

  std::vector<std::string> tablesToLookIn;
  tablesToLookIn.push_back("dummy_sys.processors");

  GateKeeper::tMonitoringSetting p(
      mGk.getMonitoringSetting("", "nonCriticalMetric", tablesToLookIn));
  BOOST_CHECK_EQUAL(p->getStatus(), monitoring::kNonCritical);

  GateKeeper::tMonitoringSetting p2(
      mGk.getMonitoringSetting("", "criticalMetric", tablesToLookIn));
  BOOST_CHECK_EQUAL(p2->getStatus(), monitoring::kEnabled);

  GateKeeper::tMonitoringSetting p3(
      mGk.getMonitoringSetting("weird_state", "criticalMetric",
          tablesToLookIn));
  BOOST_CHECK_EQUAL(p3->getStatus(), monitoring::kDisabled);
}

BOOST_AUTO_TEST_SUITE_END() // TestGateKeeper

}//ns: test
}//ns: core
} //ns: swatch
