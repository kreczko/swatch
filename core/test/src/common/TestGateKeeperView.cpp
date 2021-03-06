/**
 * \brief Test suite for swatch/core/GateKeeperView
 * \author kreczko
 */
// Boost Unit Test includes
#include <boost/test/unit_test.hpp>

// swatch headers
#include "swatch/core/GateKeeper.hpp"
#include "swatch/core/GateKeeperView.hpp"
#include "swatch/core/test/DummyGateKeeper.hpp"
#include "swatch/logger/Log.hpp"

//xdaq headers
#include <xdata/Integer.h>
#include <xdata/String.h>

using namespace swatch::logger;

namespace swatch {
namespace core {
namespace test {

struct TestGateKeeperViewSetup {
  TestGateKeeperViewSetup() :
      mGk() {
    typedef GateKeeper::Parameters_t Parameters_t;
    typedef GateKeeper::MonitoringSettings_t MonSettings_t;
    typedef GateKeeper::Masks_t Masks_t;

    GateKeeper::ParametersContext_t lCommonParams(new Parameters_t{
        {"hello", new xdata::String("World")},
        {"answer", new xdata::Integer(42)}
    });
    mGk.addContext("dummy_sys.common", lCommonParams);

    GateKeeper::ParametersContext_t lChildA1Params(new Parameters_t{
        {"hello", new xdata::String("World! (childA1)")}
    });
    mGk.addContext("dummy_sys.childA1", lChildA1Params);

    GateKeeper::ParametersContext_t lChildTypeAParams(new GateKeeper::Parameters_t{
        {"sequence_1.command_1.parameter_1", new xdata::String("sequence")},
        {"command_1.parameter_1", new xdata::String("command")}
    });
    mGk.addContext("dummy_sys.childTypeA", lChildTypeAParams);

    // monitoring status
    MonSettings_t lMetricSettings {
        {"criticalMetric", new MonitoringSetting("criticalMetric", monitoring::kEnabled)},
        {"weird_state.criticalMetric", new MonitoringSetting("weird_state.criticalMetric", monitoring::kDisabled)},
        {"nonCriticalMetric", new MonitoringSetting("nonCriticalMetric", monitoring::kNonCritical)}
    };
    mGk.addSettingsContext("dummy_sys.common", lMetricSettings);

    MonSettings_t lA1Settings {
        {"criticalMetric", new MonitoringSetting("criticalMetric", monitoring::kEnabled)}
    };
    mGk.addSettingsContext("dummy_sys.childA1", lA1Settings);

    // Masks
    mGk.addMasksContext("dummy_sys.common", Masks_t{"componentA"});
    mGk.addMasksContext("dummy_sys.childTypeA", Masks_t{"componentB"});
    mGk.addMasksContext("dummy_sys.childA1", Masks_t{"componentC"});

    // Disable some IDs
    mGk.addDisabledId("dummy_sys.childA1");
    mGk.addDisabledId("dummy_sys.child42");
  }

  DummyGateKeeper mGk;
};

BOOST_AUTO_TEST_SUITE( TestGateKeeperView )

BOOST_FIXTURE_TEST_CASE ( TestDisabledFlagsView, TestGateKeeperViewSetup ) {
  GateKeeperView lView(mGk);
  BOOST_REQUIRE_EQUAL(lView.disabledFlagsSize(), size_t(2));
  unsigned int i = 0;
  for (GateKeeperView::DisabledFlags_t::const_iterator it = lView.disabledFlagsBegin(); it != lView.disabledFlagsEnd();
      ++it, ++i) {
    BOOST_CHECK_EQUAL(it->getContext(), "none");
    if (i == 0) {
      BOOST_CHECK_EQUAL(it->getObjectId(), "dummy_sys.childA1");
      BOOST_CHECK_EQUAL(it->getObject(), "dummy_sys.childA1");
    }
    if (i == 1) {
      BOOST_CHECK_EQUAL(it->getObjectId(), "dummy_sys.child42");
      BOOST_CHECK_EQUAL(it->getObject(), "dummy_sys.child42");
    }
  }
}

BOOST_FIXTURE_TEST_CASE ( TestMasksView, TestGateKeeperViewSetup ) {
  GateKeeperView lView(mGk);
  BOOST_REQUIRE_EQUAL(lView.masksSize(), size_t(3));
  unsigned int i = 0;
  std::vector<std::string> lContexts, lMasks;

  lContexts.push_back("dummy_sys.common");
  lContexts.push_back("dummy_sys.childTypeA");
  lContexts.push_back("dummy_sys.childA1");

  lMasks.push_back("componentA");
  lMasks.push_back("componentB");
  lMasks.push_back("componentC");

  for (GateKeeperView::Masks_t::const_iterator it = lView.masksBegin(); it != lView.masksEnd(); ++it, ++i) {
    BOOST_CHECK_EQUAL(it->getContext(), lContexts.at(i));
    BOOST_CHECK_EQUAL(it->getObjectId(), lMasks.at(i));
  }
}

BOOST_FIXTURE_TEST_CASE ( TestMasksViewWithContext, TestGateKeeperViewSetup ) {
  std::vector<std::string> lContexts, lMasks;
  lContexts.push_back("dummy_sys.common");
  lMasks.push_back("componentA");

  GateKeeperView lView(mGk, lContexts);
  BOOST_REQUIRE_EQUAL(lView.masksSize(), size_t(1));
  unsigned int i = 0;
  for (GateKeeperView::Masks_t::const_iterator it = lView.masksBegin(); it != lView.masksEnd(); ++it, ++i) {
    BOOST_CHECK_EQUAL(it->getContext(), lContexts.at(i));
    BOOST_CHECK_EQUAL(it->getObjectId(), lMasks.at(i));
  }
}

BOOST_FIXTURE_TEST_CASE ( TestMonitoringSettingsView, TestGateKeeperViewSetup ) {
  std::vector<std::string> lContexts, lSettings;
  std::vector<monitoring::Status> lValues;

  lContexts.push_back("dummy_sys.common");
  lContexts.push_back("dummy_sys.common");
  lContexts.push_back("dummy_sys.common");
  lContexts.push_back("dummy_sys.childA1");

  lSettings.push_back("weird_state.criticalMetric");
  lSettings.push_back("nonCriticalMetric");
  lSettings.push_back("criticalMetric");
  lSettings.push_back("criticalMetric");

  lValues.push_back(monitoring::kDisabled);
  lValues.push_back(monitoring::kNonCritical);
  lValues.push_back(monitoring::kEnabled);
  lValues.push_back(monitoring::kEnabled);

  GateKeeperView lView(mGk);
  BOOST_REQUIRE_EQUAL(lView.monitoringSettingsSize(), size_t(4));

  unsigned int i = 0;
  for (GateKeeperView::MonitoringSettings_t::const_iterator it = lView.monitoringSettingsBegin();
      it != lView.monitoringSettingsEnd(); ++it, ++i) {
    BOOST_CHECK_EQUAL(it->getContext(), lContexts.at(i));
    BOOST_CHECK_EQUAL(it->getObjectId(), lSettings.at(i));
    BOOST_CHECK_EQUAL(it->getObject()->getStatus(), lValues.at(i));
  }
}

BOOST_FIXTURE_TEST_CASE ( TestMonitoringSettingsViewWithContext, TestGateKeeperViewSetup ) {
  std::vector<std::string> lContexts, lSettings;
  std::vector<monitoring::Status> lValues;
  lContexts.push_back("dummy_sys.childA1");

  lSettings.push_back("criticalMetric");
  lSettings.push_back("weird_state.criticalMetric");
  lSettings.push_back("nonCriticalMetric");
  lSettings.push_back("criticalMetric");

  lValues.push_back(monitoring::kEnabled);
  lValues.push_back(monitoring::kDisabled);
  lValues.push_back(monitoring::kNonCritical);
  lValues.push_back(monitoring::kEnabled);

  GateKeeperView lView(mGk, lContexts);
  BOOST_REQUIRE_EQUAL(lView.monitoringSettingsSize(), size_t(1));

  unsigned int i = 0;
  for (GateKeeperView::MonitoringSettings_t::const_iterator it = lView.monitoringSettingsBegin();
      it != lView.monitoringSettingsEnd(); ++it, ++i) {
    BOOST_CHECK_EQUAL(it->getContext(), lContexts.at(i));
    BOOST_CHECK_EQUAL(it->getObjectId(), lSettings.at(i));
    BOOST_CHECK_EQUAL(it->getObject()->getStatus(), lValues.at(i));
  }
}

BOOST_FIXTURE_TEST_CASE ( TestParameterView, TestGateKeeperViewSetup ) {
  std::vector<std::string> lContexts, lParameters, lValues;

  lContexts.push_back("dummy_sys.common");
  lContexts.push_back("dummy_sys.common");
  lContexts.push_back("dummy_sys.childA1");
  lContexts.push_back("dummy_sys.childTypeA");
  lContexts.push_back("dummy_sys.childTypeA");

  lParameters.push_back("hello");
  lParameters.push_back("answer");
  lParameters.push_back("hello");
  lParameters.push_back("sequence_1.command_1.parameter_1");
  lParameters.push_back("command_1.parameter_1");

  lValues.push_back("World");
  lValues.push_back("42");
  lValues.push_back("World! (childA1)");
  lValues.push_back("sequence");
  lValues.push_back("command");

  GateKeeperView lView(mGk);
  BOOST_REQUIRE_EQUAL(lView.parametersSize(), size_t(5));

  unsigned int i = 0;
  for (GateKeeperView::Parameters_t::const_iterator it = lView.parametersBegin(); it != lView.parametersEnd();
      ++it, ++i) {
    BOOST_CHECK_EQUAL(it->getContext(), lContexts.at(i));
    BOOST_CHECK_EQUAL(it->getObjectId(), lParameters.at(i));
    BOOST_CHECK_EQUAL(it->getObject()->toString(), lValues.at(i));
  }
}

BOOST_FIXTURE_TEST_CASE ( TestParameterViewWithContext, TestGateKeeperViewSetup ) {
  std::vector<std::string> lContexts, lParameters, lValues;
  lContexts.push_back("dummy_sys.childA1");

  lParameters.push_back("hello");

  lValues.push_back("World! (childA1)");

  GateKeeperView lView(mGk, lContexts);
  BOOST_REQUIRE_EQUAL(lView.parametersSize(), size_t(1));

  unsigned int i = 0;
  for (GateKeeperView::Parameters_t::const_iterator it = lView.parametersBegin(); it != lView.parametersEnd();
      ++it, ++i) {
    BOOST_CHECK_EQUAL(it->getContext(), lContexts.at(i));
    BOOST_CHECK_EQUAL(it->getObjectId(), lParameters.at(i));
    BOOST_CHECK_EQUAL(it->getObject()->toString(), lValues.at(i));
  }
}


BOOST_AUTO_TEST_SUITE_END() // TestGateKeeper

}//ns: test
}//ns: core
} //ns: swatch
