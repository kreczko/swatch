// Boost Unit Test includes
#include <boost/test/unit_test.hpp>

// SWATCH headers
#include "swatch/logger/Log.hpp"
#include "swatch/xml/XmlGateKeeper.hpp"

using namespace swatch::logger;
using namespace swatch::core;

namespace swatch {
namespace xml {
namespace test {

struct TestXmlGateKeeperSetup {

  TestXmlGateKeeperSetup() :
          base_config_str(""),
          config_with_metrics_str(""),
          config_with_disabled_processor_str(""),
          base_config(),
          config_with_metrics(),
          config_with_disabled_processor() {
    base_config_str = "<db>"
        "<key id=\"RunKey1\">"
        "<!-- Common parameters for all processors -->"
        "<!-- to be used with ${SWATCH_ROOT}/system/test/cfg/dummy_s1tem.json -->		"
        "<context id=\"dummy_s1.processors\">"
        "	<!-- Default parameter values for all commands in all sequences -->"
        "	<param id=\"cmdDuration\" type=\"uint\">6</param>"
        "	<param id=\"clkErrorTimeout\" type=\"uint\">120</param>"
        "	<!-- Value for parameter 'cmdDuration' in 'reboot' command -->"
        "	<param id=\"reboot.cmdDuration\" type=\"uint\">8</param>"
        "</context>"
        ""
        "<!-- Different parameter values for processors with role 'dummy_p1' ; higher priority"
        "than 'dummy_s1.processors' context -->"
        "<context id=\"dummy_s1.dummy_p1\">"
        "<!-- Default parameter values for all commands in all sequences -->"
        "<param id=\"cmdDuration\" type=\"uint\">8</param>"
        "<param id=\"clkErrorTimeout\" type=\"uint\">60</param>"
        "<!-- Value for parameter \"cmdDuration\" in \"reboot\" command -->"
        "<param id=\"reboot.cmdDuration\" type=\"uint\">12</param>"
        "</context>"
        ""
        "<context id=\"dummy_s1.dummy_p2\">"
        "			<param id=\"clkErrorTimeout\" type=\"uint\">40</param>"
        "</context>"
        ""
        "<!-- AMC13s ... -->"
        "<context id=\"dummy_s1.daqttcs\">"
        "<param id=\"cmdDuration\" type=\"uint\">8</param>"
        "<param id=\"reboot.cmdDuration\" type=\"uint\">12</param>"
        "<param id=\"clkErrorTimeout\" type=\"uint\">120</param>"
        "</context>"
        "</key>"
        "</db>";

    config_with_metrics_str = "<db>"
        "<key id=\"RunKey1\">"
        "<context id=\"dummy_s1.processors\">"
        "<!-- Define metric status -->"
        "<state id=\"weird_state\">"
        "	<metric id=\"criticalMetric\" status=\"disabled\" />"
        "</state>"
        "		</context>"
        "	<context id=\"dummy_s1.dummy_p2\">"
        "		<state id=\"weird_state\">"
        "			<metric id=\"nonCriticalMetric\" status=\"enabled\" />"
        "		</state>"
        "	</context>"
        "</key>"
        "</db>";
    config_with_disabled_processor_str = "<db>"
        "	<key id=\"RunKey1\">"
        "		<context id=\"dummy_s1.dummy_p2\">"
        "			<param id=\"clkErrorTimeout\" type=\"uint\">40</param>"
        "			<state id=\"weird_state\">"
        "				<!-- disable whole processor -->"
        "				<mon-obj id=\"\" status=\"disabled\" />"
        "			</state>"
        "		</context>"
        "	</key>"
        "</db>";
    config_with_vector_parameters_str = "<db>"
        "   <key id=\"RunKey1\">"
        "       <context id=\"dummy_s1.dummy_p2\">"
        "           <param id=\"clkErrorTimeout\" type=\"uint\">40</param>"
        "           <param id=\"uintvector\" type=\"vector:uint\">5, 3, 42, 100</param>"
        "       </context>"
        "   </key>"
        "</db>";
    config_with_masks_str = "<db>"
        "   <key id=\"RunKey1\">"
        "       <context id=\"dummy_sys.common\">"
        "           <mask id=\"componentA\"/>"
        "       </context>"
        "       <context id=\"dummy_sys.childTypeA\">"
        "           <mask id=\"componentB\"/>"
        "       </context>"
        "       <context id=\"dummy_sys.childA1\">"
        "           <mask id=\"componentC\"/>"
        "       </context>"
        "   </key>"
        "   <key id=\"OtherRunKey\">"
        "   </key>"
        "</db>";
    config_with_exclusions_str = "<db>"
        "   <key id=\"RunKey1\">"
        "       <disable id=\"dummy_sys.childA1\"/>"
        "       <disable id=\"dummy_sys.child42\"/>"
        "   </key>"
        "   <key id=\"OtherRunKey\">"
        "   </key>"
        "</db>";

    // make sure we the strings are valid configs
    BOOST_REQUIRE_EQUAL(base_config.load(base_config_str.c_str()), true);
    BOOST_REQUIRE_EQUAL(config_with_metrics.load(config_with_metrics_str.c_str()), true);
    BOOST_REQUIRE_EQUAL(config_with_disabled_processor.load(config_with_disabled_processor_str.c_str()), true);
    BOOST_REQUIRE_EQUAL(config_with_vector_parameters.load(config_with_vector_parameters_str.c_str()), true);
    BOOST_REQUIRE_EQUAL(config_with_masks.load(config_with_masks_str.c_str()), true);
    BOOST_REQUIRE_EQUAL(config_with_exclusions.load(config_with_exclusions_str.c_str()), true);
  }

  std::string base_config_str, config_with_metrics_str, config_with_disabled_processor_str;
  std::string config_with_vector_parameters_str;
  std::string config_with_masks_str;
  std::string config_with_exclusions_str;
  pugi::xml_document base_config, config_with_metrics, config_with_disabled_processor, config_with_vector_parameters,
      config_with_masks, config_with_exclusions;
};

BOOST_AUTO_TEST_SUITE( TestXmlGateKeeper )

BOOST_FIXTURE_TEST_CASE ( VerifyBaseConfig, TestXmlGateKeeperSetup )
{
	XmlGateKeeper gk(base_config, "RunKey1");
	std::vector<std::string> contextsToLookIn;
	contextsToLookIn.push_back("dummy_s1.processors");

  BOOST_CHECK_EQUAL(gk.get("", "", "cmdDuration", contextsToLookIn)->toString(), "6");
  BOOST_CHECK_EQUAL(gk.get("", "", "clkErrorTimeout", contextsToLookIn)->toString(), "120");
  BOOST_CHECK_EQUAL(gk.get("", "reboot", "cmdDuration", contextsToLookIn)->toString(), "8");

  contextsToLookIn.clear();
  contextsToLookIn.push_back("dummy_s1.dummy_p1");
  BOOST_CHECK_EQUAL(gk.get("", "", "cmdDuration", contextsToLookIn)->toString(), "8");
  BOOST_CHECK_EQUAL(gk.get("", "", "clkErrorTimeout", contextsToLookIn)->toString(), "60");
  BOOST_CHECK_EQUAL(gk.get("", "reboot", "cmdDuration", contextsToLookIn)->toString(), "12");

  contextsToLookIn.clear();
  contextsToLookIn.push_back("dummy_s1.dummy_p2");
  BOOST_CHECK_EQUAL(gk.get("", "", "clkErrorTimeout", contextsToLookIn)->toString(), "40");

  contextsToLookIn.clear();
  contextsToLookIn.push_back("dummy_s1.daqttcs");
  BOOST_CHECK_EQUAL(gk.get("", "", "cmdDuration", contextsToLookIn)->toString(), "8");
  BOOST_CHECK_EQUAL(gk.get("", "", "clkErrorTimeout", contextsToLookIn)->toString(), "120");
  BOOST_CHECK_EQUAL(gk.get("", "reboot", "cmdDuration", contextsToLookIn)->toString(), "12");

}

BOOST_FIXTURE_TEST_CASE ( TestMetricSettings, TestXmlGateKeeperSetup )
{
	XmlGateKeeper gk(config_with_metrics, "RunKey1");
	std::vector<std::string> contextsToLookIn;
	contextsToLookIn.push_back("dummy_s1.processors");
	BOOST_CHECK_EQUAL(gk.getMonitoringSetting("weird_state", "criticalMetric", contextsToLookIn)->getStatus(),
			monitoring::kDisabled);

  contextsToLookIn.clear();
  contextsToLookIn.push_back("dummy_s1.dummy_p2");
  BOOST_CHECK_EQUAL(gk.getMonitoringSetting("weird_state", "nonCriticalMetric", contextsToLookIn)->getStatus(),
      monitoring::kEnabled);

}

BOOST_FIXTURE_TEST_CASE ( TestMonitoringSettings, TestXmlGateKeeperSetup )
{
//	std::string input_file = "${SWATCH_ROOT}/core/test/config/config_with_disabled_processor.xml";
	XmlGateKeeper gk(config_with_disabled_processor, "RunKey1");
	std::vector<std::string> contextsToLookIn;
	contextsToLookIn.push_back("dummy_s1.dummy_p2");
	BOOST_CHECK_EQUAL(gk.getMonitoringSetting("weird_state", "", contextsToLookIn)->getStatus(),
			monitoring::kDisabled);
}

BOOST_FIXTURE_TEST_CASE ( TestVectorParameters, TestXmlGateKeeperSetup )
{
	XmlGateKeeper gk(config_with_vector_parameters, "RunKey1");
	std::vector<std::string> contextsToLookIn;
	contextsToLookIn.push_back("dummy_s1.dummy_p2");
	BOOST_CHECK_EQUAL(gk.get("", "", "uintvector", contextsToLookIn)->toString(), "[5,3,42,100]");
}

BOOST_FIXTURE_TEST_CASE(TestMask, TestXmlGateKeeperSetup) // Same as code in core::GateKeeper's tests (at least in Dec 2015)
{
  XmlGateKeeper lGK(config_with_masks, "RunKey1");
  std::vector<std::string> lContextsToLookIn;

  // 1) When contexts vector is empty,  GateKeeper::getMask method should always return false
  BOOST_CHECK_EQUAL(lGK.getMask("componentA", lContextsToLookIn), false);
  BOOST_CHECK_EQUAL(lGK.getMask("componentB", lContextsToLookIn), false);
  BOOST_CHECK_EQUAL(lGK.getMask("componentC", lContextsToLookIn), false);
  BOOST_CHECK_EQUAL(lGK.getMask("otherComponent", lContextsToLookIn), false);

  // 2) Contexts vector only contains common context
  lContextsToLookIn.push_back("dummy_sys.common");
  BOOST_CHECK_EQUAL(lGK.getMask("componentA", lContextsToLookIn), true);
  BOOST_CHECK_EQUAL(lGK.getMask("componentB", lContextsToLookIn), false);
  BOOST_CHECK_EQUAL(lGK.getMask("componentC", lContextsToLookIn), false);
  BOOST_CHECK_EQUAL(lGK.getMask("otherComponent", lContextsToLookIn), false);

  // 3) Contexts vector contains proc1 and common contexts
  lContextsToLookIn = std::vector<std::string> { "dummy_sys.childA1", "dummy_sys.common" };
  BOOST_CHECK_EQUAL(lGK.getMask("componentA", lContextsToLookIn), true);
  BOOST_CHECK_EQUAL(lGK.getMask("componentB", lContextsToLookIn), false);
  BOOST_CHECK_EQUAL(lGK.getMask("componentC", lContextsToLookIn), true);
  BOOST_CHECK_EQUAL(lGK.getMask("otherComponent", lContextsToLookIn), false);

  // 4) Contexts vector contains processors and common contexts
  lContextsToLookIn = std::vector<std::string> { "dummy_sys.childTypeA", "dummy_sys.common" };
  BOOST_CHECK_EQUAL(lGK.getMask("componentA", lContextsToLookIn), true);
  BOOST_CHECK_EQUAL(lGK.getMask("componentB", lContextsToLookIn), true);
  BOOST_CHECK_EQUAL(lGK.getMask("componentC", lContextsToLookIn), false);
  BOOST_CHECK_EQUAL(lGK.getMask("otherComponent", lContextsToLookIn), false);

  // 5) Contexts vector contains all contexts
  lContextsToLookIn = std::vector<std::string> { "dummy_sys.childA1", "dummy_sys.childTypeA", "dummy_sys.common" };
  BOOST_CHECK_EQUAL(lGK.getMask("componentA", lContextsToLookIn), true);
  BOOST_CHECK_EQUAL(lGK.getMask("componentB", lContextsToLookIn), true);
  BOOST_CHECK_EQUAL(lGK.getMask("componentC", lContextsToLookIn), true);
  BOOST_CHECK_EQUAL(lGK.getMask("otherComponent", lContextsToLookIn), false);
}

BOOST_FIXTURE_TEST_CASE(TestDisabled, TestXmlGateKeeperSetup) // Same as code in core::GateKeeper's tests (at least in Jan 2016)
{
  XmlGateKeeper lGK(config_with_exclusions, "RunKey1");

  BOOST_CHECK_EQUAL(lGK.isEnabled("dummy_sys.childA1"), false);
  BOOST_CHECK_EQUAL(lGK.isEnabled("dummy_sys.childA2"), true);
  BOOST_CHECK_EQUAL(lGK.isEnabled("dummy_sys.unkownChild"), true);
  BOOST_CHECK_EQUAL(lGK.isEnabled("dummy_sys.child42"), false);
}

BOOST_AUTO_TEST_CASE( CreateFromConfigFile ) {
  std::string lTestFile("xml/test/etc/swatch/test/mainConfig.xml");
  XmlGateKeeper lGK(lTestFile, "MyDummyKey");

  std::vector<std::string> lContextsToLookIn;
  lContextsToLookIn = std::vector<std::string> { "system.processors", "system.processor1" };

  // from sub1.xml
  BOOST_CHECK_EQUAL(lGK.get("", "", "resetBoard.clockSource", lContextsToLookIn)->toString(), "external");
  // from sub2.xml
  BOOST_CHECK_EQUAL(lGK.get("", "", "electronLookUpTable", lContextsToLookIn)->toString(), "[56,345,954,310]");
  // from sub3.xml
  BOOST_CHECK_EQUAL(lGK.getMonitoringSetting("Halted", "ports.Rx00", lContextsToLookIn)->getStatus(),
      monitoring::kNonCritical);
  BOOST_CHECK_EQUAL(lGK.getMonitoringSetting("Halted", "ports.Rx01", lContextsToLookIn)->getStatus(),
      monitoring::kNonCritical);
  BOOST_CHECK_EQUAL(lGK.getMask("ports.Rx00", lContextsToLookIn), true);
  BOOST_CHECK_EQUAL(lGK.isEnabled("system.brokenProcessor"), false);
}

BOOST_AUTO_TEST_SUITE_END() // TestGateKeeper

}//ns: test
}//ns: xml
} //ns: swatch
