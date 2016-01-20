// Boost Unit Test includes
#include <boost/test/unit_test.hpp>

#include <algorithm>
#include <string>

// SWATCH headers
#include "swatch/logger/Log.hpp"
#include "swatch/xml/XmlReader.hpp"

// external headers
#include "pugixml/pugixml.hpp"

namespace swatch {
namespace xml {
namespace test {

struct TestXmlReaderSetup {

  TestXmlReaderSetup() :
          mMainConfigStr(""),
          mInvalidMainConfigStr(""),
          mSubConfigStr(""),
          mInvalidSubConfigStr(""),
          mMergedConfigStr(""),
          mMainConfig(),
          mInvalidMainConfig(),
          mSubConfig(),
          mInvalidSubConfig() {
    mMainConfigStr = "<db>"
        "<key id=\"MyDummyKey\">"
        "<load module=\"file://xml/test/etc/swatch/test/sub1.xml\" />"
        "</key>"
        "</db>";
    mInvalidMainConfigStr = "<db>"
        "<key id=\"MyDummyKey\">"
        "<load module=\"file://xml/test/etc/swatch/test/sub1.xml\" />"
        "<table id=\"system.processors\">"
        "<entry id=\"resetBoard.clockSource\" type=\"string\">external</entry>"
        "</table>"
        "</key>"
        "</db>";
    mSubConfigStr = "<module>"
        "<table id=\"system.processor1\">"
        "<state id=\"Halted\">"
        "<mon-obj id=\"ports.Rx00\" status=\"non-critical\" />"
        "</state>"
        "<mask id=\"ports.Rx00\" />"
        "</table>"
        "<disable id=\"system.brokenProcessor\" />"
        "</module>";
    mInvalidSubConfigStr = "<module>"
        "<load module=\"file://xml/test/etc/swatch/test/sub1.xml\" />"
        "</module>";
    // main config + sub config
    mMergedConfigStr = "<db>"
        "<key id=\"MyDummyKey\">"
        "<table id=\"system.processor1\">"
        "<state id=\"Halted\">"
        "<mon-obj id=\"ports.Rx00\" status=\"non-critical\" />"
        "</state>"
        "<mask id=\"ports.Rx00\"/>"
        "</table>"
        "<disable id=\"system.brokenProcessor\" />"
        "</key>"
        "</db>";

    // make sure we the strings are valid configs
    BOOST_REQUIRE_EQUAL(mMainConfig.load(mMainConfigStr.c_str()), true);
    BOOST_REQUIRE_EQUAL(mInvalidMainConfig.load(mInvalidMainConfigStr.c_str()), true);
    BOOST_REQUIRE_EQUAL(mSubConfig.load(mSubConfigStr.c_str()), true);
    BOOST_REQUIRE_EQUAL(mInvalidSubConfig.load(mInvalidSubConfigStr.c_str()), true);
  }

  std::string mMainConfigStr, mInvalidMainConfigStr, mSubConfigStr, mInvalidSubConfigStr;
  std::string mMergedConfigStr;

  pugi::xml_document mMainConfig, mInvalidMainConfig, mSubConfig, mInvalidSubConfig;
};

BOOST_AUTO_TEST_SUITE( TestXmlReader )

BOOST_FIXTURE_TEST_CASE ( VerifyMainConfig, TestXmlReaderSetup ) {
  XmlReader lReader;
  BOOST_REQUIRE_EQUAL(lReader.checkMainConfig(mMainConfig), true);
  BOOST_REQUIRE_EQUAL(lReader.checkMainConfig(mInvalidMainConfig), false);
}

BOOST_FIXTURE_TEST_CASE ( VerifySubConfig, TestXmlReaderSetup ) {
  XmlReader lReader;

  BOOST_REQUIRE_EQUAL(lReader.checkSubConfig(mSubConfig), true);
  BOOST_REQUIRE_EQUAL(lReader.checkSubConfig(mInvalidSubConfig), false);
}

BOOST_FIXTURE_TEST_CASE ( TestToString, TestXmlReaderSetup ) {
  XmlReader lReader;
  std::string lTestString(lReader.docToString(mSubConfig));
  // trim all tabs and new lines (mSubConfigStr is a one-liner without any tabs or new lines)
  lTestString.erase(std::remove(lTestString.begin(), lTestString.end(), '\t'), lTestString.end());
  lTestString.erase(std::remove(lTestString.begin(), lTestString.end(), '\n'), lTestString.end());
  // but pugi also adds '<?xml version="1.0"?>' at the beginning
  std::string lTestAgainst("<?xml version=\"1.0\"?>" + mSubConfigStr);
  // pugi also corrects missing spaces, e.g. <mask id=\"ports.Rx00\"/> vs <mask id=\"ports.Rx00\" />
  // so keep that in mind when comparing
  BOOST_REQUIRE_EQUAL(lTestString, lTestAgainst);
}

BOOST_AUTO_TEST_CASE( TestReadXmlConfig ) {
  std::string lTestFile("xml/test/etc/swatch/test/mainConfig.xml");
  XmlReader lReader;
  pugi::xml_document lMergedDoc;
  lMergedDoc.load(lReader.readXmlConfig(lTestFile).c_str());

  // the file itself will pass the check
  pugi::xml_document lMainConfig;
  lMainConfig.load_file(lTestFile.c_str());
  BOOST_REQUIRE_EQUAL(lReader.checkMainConfig(lMainConfig), true);
  // but the merged config, where <load> has been replaced with tables and 'disable' tag, will fail
  BOOST_REQUIRE_EQUAL(lReader.checkMainConfig(lMergedDoc), false);
  // the merged config should now have 2 tables (merged from 3) and one <disable> tag
  pugi::xml_node lKey(lMergedDoc.child("db").find_child_by_attribute("key", "id", "MyDummyKey"));

  int lResult = std::distance(lKey.children("table").begin(), lKey.children("table").end());
  BOOST_REQUIRE_EQUAL(lResult, 2);
  lResult = std::distance(lKey.children("disable").begin(), lKey.children("disable").end());
  BOOST_REQUIRE_EQUAL(lResult, 1);
}

BOOST_AUTO_TEST_SUITE_END() // TestXmlReader

}//ns: test
}//ns: xml
} //ns: swatch
