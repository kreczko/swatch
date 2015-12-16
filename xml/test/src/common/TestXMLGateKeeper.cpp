
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
    config_with_disabled_processor()
  {
		base_config_str = "<db>"
				"<run key=\"RunKey1\">"
				"<!-- Common parameters for all processors -->"
				"<!-- to be used with ${SWATCH_ROOT}/system/test/cfg/dummy_s1tem.json -->		"
				"<table id=\"dummy_s1.processors\">"
				"	<!-- Default parameter values for all commands in all sequences -->"
				"	<entry id=\"cmdDuration\" type=\"uint\">6</entry>"
				"	<entry id=\"clkErrorTimeout\" type=\"uint\">120</entry>"
				"	<!-- Value for parameter 'cmdDuration' in 'reboot' command -->"
				"	<entry id=\"reboot.cmdDuration\" type=\"uint\">8</entry>"
				"</table>"
				""
				"<!-- Different parameter values for processors with role 'dummy_p1' ; higher priority"
				"than 'dummy_s1.processors' table -->"
				"<table id=\"dummy_s1.dummy_p1\">"
				"<!-- Default parameter values for all commands in all sequences -->"
				"<entry id=\"cmdDuration\" type=\"uint\">8</entry>"
				"<entry id=\"clkErrorTimeout\" type=\"uint\">60</entry>"
				"<!-- Value for parameter \"cmdDuration\" in \"reboot\" command -->"
				"<entry id=\"reboot.cmdDuration\" type=\"uint\">12</entry>"
				"</table>"
				""
				"<table id=\"dummy_s1.dummy_p2\">"
				"			<entry id=\"clkErrorTimeout\" type=\"uint\">40</entry>"
				"</table>"
				""
				"<!-- AMC13s ... -->"
				"<table id=\"dummy_s1.daqttcs\">"
				"<entry id=\"cmdDuration\" type=\"uint\">8</entry>"
				"<entry id=\"reboot.cmdDuration\" type=\"uint\">12</entry>"
				"<entry id=\"clkErrorTimeout\" type=\"uint\">120</entry>"
				"</table>"
				"</run>"
				"</db>";

		config_with_metrics_str = "<db>"
				"<run key=\"RunKey1\">"
				"<table id=\"dummy_s1.processors\">"
				"<!-- Define metric status -->"
				"<state id=\"weird_state\">"
				"	<metric id=\"criticalMetric\" status=\"disabled\" />"
				"</state>"
				"		</table>"
				"	<table id=\"dummy_s1.dummy_p2\">"
				"		<state id=\"weird_state\">"
				"			<metric id=\"nonCriticalMetric\" status=\"enabled\" />"
				"		</state>"
				"	</table>"
				"</run>"
				"</db>";
		config_with_disabled_processor_str = "<db>"
				"	<run key=\"RunKey1\">"
				"		<table id=\"dummy_s1.dummy_p2\">"
				"			<entry id=\"clkErrorTimeout\" type=\"uint\">40</entry>"
				"			<state id=\"weird_state\">"
				"				<!-- disable whole processor -->"
				"				<mon-obj id=\"\" status=\"disabled\" />"
				"			</state>"
				"		</table>"
				"	</run>"
				"</db>";
		config_with_vector_parameters_str = "<db>"
				"   <run key=\"RunKey1\">"
				"       <table id=\"dummy_s1.dummy_p2\">"
				"           <entry id=\"clkErrorTimeout\" type=\"uint\">40</entry>"
				"           <entry id=\"uintvector\" type=\"vector:uint\">5, 3, 42, 100</entry>"
				"       </table>"
				"   </run>"
				"</db>";
    config_with_masks_str = "<db>"
        "   <run key=\"RunKey1\">"
        "       <table id=\"dummy_sys.common\">"
        "           <mask id=\"componentA\"/>"
        "       </table>"
        "       <table id=\"dummy_sys.childTypeA\">"
        "           <mask id=\"componentB\"/>"
        "       </table>"
        "       <table id=\"dummy_sys.childA1\">"
        "           <mask id=\"componentC\"/>"
        "       </table>"
        "   </run>"
        "   <run key=\"OtherRunKey\">"
        "   </run>"
        "</db>";

    // make sure we the strings are valid configs
    BOOST_REQUIRE_EQUAL(base_config.load(base_config_str.c_str()), true);
    BOOST_REQUIRE_EQUAL(config_with_metrics.load(config_with_metrics_str.c_str()), true);
    BOOST_REQUIRE_EQUAL(config_with_disabled_processor.load(config_with_disabled_processor_str.c_str()), true);
    BOOST_REQUIRE_EQUAL(config_with_vector_parameters.load(config_with_vector_parameters_str.c_str()), true);
    BOOST_REQUIRE_EQUAL(config_with_masks.load(config_with_masks_str.c_str()), true);
  }

  std::string base_config_str, config_with_metrics_str, config_with_disabled_processor_str;
  std::string config_with_vector_parameters_str;
  std::string config_with_masks_str;
  pugi::xml_document base_config, config_with_metrics, config_with_disabled_processor, config_with_vector_parameters, config_with_masks;
};

BOOST_AUTO_TEST_SUITE( TestXmlGateKeeper )

BOOST_FIXTURE_TEST_CASE ( VerifyBaseConfig, TestXmlGateKeeperSetup ) {
	LOG(kInfo) << "Running TestXmlGateKeeper/VerifyBaseConfig";

	XmlGateKeeper gk(base_config, "RunKey1");
	std::vector<std::string> tablesToLookIn;
	tablesToLookIn.push_back("dummy_s1.processors");

	BOOST_CHECK_EQUAL(gk.get("", "", "cmdDuration", tablesToLookIn)->toString(), "6");
	BOOST_CHECK_EQUAL(gk.get("", "", "clkErrorTimeout", tablesToLookIn)->toString(), "120");
	BOOST_CHECK_EQUAL(gk.get("", "reboot", "cmdDuration", tablesToLookIn)->toString(), "8");

	tablesToLookIn.clear();
	tablesToLookIn.push_back("dummy_s1.dummy_p1");
	BOOST_CHECK_EQUAL(gk.get("", "", "cmdDuration", tablesToLookIn)->toString(), "8");
	BOOST_CHECK_EQUAL(gk.get("", "", "clkErrorTimeout", tablesToLookIn)->toString(), "60");
	BOOST_CHECK_EQUAL(gk.get("", "reboot", "cmdDuration", tablesToLookIn)->toString(), "12");

	tablesToLookIn.clear();
	tablesToLookIn.push_back("dummy_s1.dummy_p2");
	BOOST_CHECK_EQUAL(gk.get("", "", "clkErrorTimeout", tablesToLookIn)->toString(), "40");

	tablesToLookIn.clear();
	tablesToLookIn.push_back("dummy_s1.daqttcs");
	BOOST_CHECK_EQUAL(gk.get("", "", "cmdDuration", tablesToLookIn)->toString(), "8");
	BOOST_CHECK_EQUAL(gk.get("", "", "clkErrorTimeout", tablesToLookIn)->toString(), "120");
	BOOST_CHECK_EQUAL(gk.get("", "reboot", "cmdDuration", tablesToLookIn)->toString(), "12");

}

BOOST_FIXTURE_TEST_CASE ( TestMetricSettings, TestXmlGateKeeperSetup ) {
	LOG(kInfo) << "Running TestXmlGateKeeper/TestMetricSettings";

	XmlGateKeeper gk(config_with_metrics, "RunKey1");
	std::vector<std::string> tablesToLookIn;
	tablesToLookIn.push_back("dummy_s1.processors");
	BOOST_CHECK_EQUAL(gk.getMonitoringSetting("weird_state", "criticalMetric", tablesToLookIn)->getStatus(),
			monitoring::kDisabled);

	tablesToLookIn.clear();
	tablesToLookIn.push_back("dummy_s1.dummy_p2");
	BOOST_CHECK_EQUAL(gk.getMonitoringSetting("weird_state", "nonCriticalMetric", tablesToLookIn)->getStatus(),
			monitoring::kEnabled);

}

BOOST_FIXTURE_TEST_CASE ( TestMonitoringSettings, TestXmlGateKeeperSetup ) {
	LOG(kInfo) << "Running TestXmlGateKeeper/TestMonitoringSettings";
//	std::string input_file = "${SWATCH_ROOT}/core/test/config/config_with_disabled_processor.xml";
	XmlGateKeeper gk(config_with_disabled_processor, "RunKey1");
	std::vector<std::string> tablesToLookIn;
	tablesToLookIn.push_back("dummy_s1.dummy_p2");
	BOOST_CHECK_EQUAL(gk.getMonitoringSetting("weird_state", "", tablesToLookIn)->getStatus(),
			monitoring::kDisabled);
}

BOOST_FIXTURE_TEST_CASE ( TestVectorParameters, TestXmlGateKeeperSetup ) {
	LOG(kInfo) << "Running TestXmlGateKeeper/TestVectorParameters";
	XmlGateKeeper gk(config_with_vector_parameters, "RunKey1");
	std::vector<std::string> tablesToLookIn;
	tablesToLookIn.push_back("dummy_s1.dummy_p2");
	BOOST_CHECK_EQUAL(gk.get("", "", "uintvector", tablesToLookIn)->toString(), "[5,3,42,100]");
}


BOOST_FIXTURE_TEST_CASE(TestMask, TestXmlGateKeeperSetup) // Same as code in core::GateKeeper's tests (at least in Dec 2015)
{
  LOG(kInfo) << "Running TestXmlGateKeeperSetup/TestMask";
  
  XmlGateKeeper lGK(config_with_masks, "RunKey1");
  std::vector<std::string> lTablesToLookIn;

  // 1) When tables vector is empty,  GateKeeper::getMask method should always return false
  BOOST_CHECK_EQUAL(lGK.getMask("componentA", lTablesToLookIn), false);
  BOOST_CHECK_EQUAL(lGK.getMask("componentB", lTablesToLookIn), false);
  BOOST_CHECK_EQUAL(lGK.getMask("componentC", lTablesToLookIn), false);
  BOOST_CHECK_EQUAL(lGK.getMask("otherComponent", lTablesToLookIn), false);
  
  // 2) Tables vector only contains common table
  lTablesToLookIn.push_back("dummy_sys.common");
  BOOST_CHECK_EQUAL(lGK.getMask("componentA", lTablesToLookIn), true);
  BOOST_CHECK_EQUAL(lGK.getMask("componentB", lTablesToLookIn), false);
  BOOST_CHECK_EQUAL(lGK.getMask("componentC", lTablesToLookIn), false);
  BOOST_CHECK_EQUAL(lGK.getMask("otherComponent", lTablesToLookIn), false);

  // 3) Tables vector contains proc1 and common tables
  lTablesToLookIn = std::vector<std::string>{"dummy_sys.childA1", "dummy_sys.common"};
  BOOST_CHECK_EQUAL(lGK.getMask("componentA", lTablesToLookIn), true);
  BOOST_CHECK_EQUAL(lGK.getMask("componentB", lTablesToLookIn), false);
  BOOST_CHECK_EQUAL(lGK.getMask("componentC", lTablesToLookIn), true);
  BOOST_CHECK_EQUAL(lGK.getMask("otherComponent", lTablesToLookIn), false);

  // 4) Tables vector contains processors and common tables
  lTablesToLookIn = std::vector<std::string>{"dummy_sys.childTypeA", "dummy_sys.common"};
  BOOST_CHECK_EQUAL(lGK.getMask("componentA", lTablesToLookIn), true);
  BOOST_CHECK_EQUAL(lGK.getMask("componentB", lTablesToLookIn), true);
  BOOST_CHECK_EQUAL(lGK.getMask("componentC", lTablesToLookIn), false);
  BOOST_CHECK_EQUAL(lGK.getMask("otherComponent", lTablesToLookIn), false);

  // 5) Tables vector contains all tables
  lTablesToLookIn = std::vector<std::string>{"dummy_sys.childA1", "dummy_sys.childTypeA", "dummy_sys.common"};
  BOOST_CHECK_EQUAL(lGK.getMask("componentA", lTablesToLookIn), true);
  BOOST_CHECK_EQUAL(lGK.getMask("componentB", lTablesToLookIn), true);
  BOOST_CHECK_EQUAL(lGK.getMask("componentC", lTablesToLookIn), true);
  BOOST_CHECK_EQUAL(lGK.getMask("otherComponent", lTablesToLookIn), false);
}


BOOST_AUTO_TEST_SUITE_END() // TestGateKeeper

}//ns: test
}//ns: xml
} //ns: swatch
