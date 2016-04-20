// Boost Unit Test includes
#include <boost/test/unit_test.hpp>

#include <algorithm>
#include <string>

// SWATCH headers
#include "swatch/logger/Log.hpp"
#include "swatch/xml/XmlSystem.hpp"

// external headers
#include "pugixml/pugixml.hpp"

namespace swatch {
namespace xml {
namespace system {
namespace test {

struct TestXmlSystemSetup {

  TestXmlSystemSetup() :
          mCrateXmlStr(""),
          mProc1XmlStr(""),
          mProc2XmlStr(""),
          mDaqTTCXmlStr(""),
          mLinkXmlStr(""),
          mFEDXmlStr(""),
          mSystemXmlStr(""),
          mSystemXml() {
    mCrateXmlStr = ""
        "  <crate id=\"S2D11-29\">"
        "    <location>Point5, S2D11-29</location>"
        "    <description>Calorimeter trigger layer 2 main processor crate</description>"
        "  </crate>";
    mProc1XmlStr = ""
        "  <processor id=\"MP8\">"
        "    <creator>calol2::MainProcessor</creator>"
        "    <hw-type>MP7-XE</hw-type>"
        "    <role>MainProcessor</role>"
        "    <uri>chtcp-2.0://ctrl-calol2:10203?target=amc-s2d11-29-09:50001</uri>"
        "    <address-table>file:///opt/cactus/etc/mp7/addrtab/xe_v2_0_0/mp7xe_infra.xml</address-table>"
        "    <crate>S2D11-29</crate>"
        "    <slot>9</slot>"
        "    <rx-port name=\"Rx[0:72]\" pid=\"[0:72]\"/>"
        "    <tx-port name=\"Tx[60:66]\" pid=\"[60:66]\"/>"
        "  </processor>";
    mProc2XmlStr = ""
        "  <processor id=\"DEMUX\">"
        "    <creator>calol2::Demux</creator>"
        "    <hw-type>MP7-XE</hw-type>"
        "    <role>Demux</role>"
        "    <uri>chtcp-2.0://ctrl-calol2:10203?target=amc-s2d11-29-12:50001</uri>"
        "    <address-table>file:///opt/cactus/etc/mp7/addrtab/xe_v2_0_0/mp7xe_infra.xml</address-table>"
        "    <crate>S2D11-29</crate>"
        "    <slot>12</slot>"
        "    <rx-port name=\"Rx[0:72]\" pid=\"[0:72]\"/>"
        "    <tx-port name=\"Tx[04:11]\" pid=\"[04:11]\"/>"
        "    </processor>";
    mDaqTTCXmlStr = ""
        "  <daqttc-mgr id=\"AMC13\">                                                   "
        "    <creator>swatch::amc13::AMC13Manager</creator>                            "
        "    <role>daqttc</role>                                                       "
        "    <crate>S2D11-29</crate>                                                   "
        "    <slot>13</slot>                                                           "
        "    <uri>chtcp-2.0://ctrl-calol2:10203?target=amc-s2d11-29-13-t1:50001</uri>  "
        "    <uri>chtcp-2.0://ctrl-calol2:10203?target=amc-s2d11-29-13-t2:50001</uri>  "
        "    <address-table>file:///opt/cactus/etc/amc13/AMC13XG_T1.xml</address-table>"
        "    <address-table>file:///opt/cactus/etc/amc13/AMC13XG_T2.xml</address-table>"
        "    <fed-id>1360</fed-id>                                                     "
        "  </daqttc-mgr>                                                               ";
    mLinkXmlStr = ""
        "  <link id=\"MP8_Tx_[60:66]\">                                                "
        "    <from>MP8</from>                                                          "
        "    <to>DEMUX</to>                                                            "
        "    <tx-port>Tx[60:66]</tx-port>                                              "
        "    <rx-port>Rx[39:72:6]</rx-port>                                            "
        "  </link>                                                                     ";
    mFEDXmlStr = ""
        "  <connected-fed id=\"1356\">                                                 "
        "    <port id=\"MP8.inputPorts.Rx[00:10]\"/>                                   "
        "    <port id=\"MP8.inputPorts.Rx[58:72]\"/>                                   "
        "  </connected-fed>                                                            "
        "                                                                              ";
    mSystemXmlStr = ""
        "<system id=\"calol2\">"
        "  <creator>calol2::System</creator>" + //
        mCrateXmlStr + mProc1XmlStr + mProc2XmlStr + //
        mDaqTTCXmlStr + mLinkXmlStr + mFEDXmlStr + //
        "</system>";
    // make sure we the strings are valid configs
    assert(mCrateXml.load(mCrateXmlStr.c_str()) == true);
    assert(mProc1Xml.load(mProc1XmlStr.c_str()) == true);
    assert(mProc2Xml.load(mProc2XmlStr.c_str()) == true);
    assert(mDaqTTCXml.load(mDaqTTCXmlStr.c_str()) == true);
    assert(mLinkXml.load(mLinkXmlStr.c_str()) == true);
    assert(mFEDXml.load(mFEDXmlStr.c_str()) == true);
    assert(mSystemXml.load(mSystemXmlStr.c_str()) == true);
  }

  std::string mCrateXmlStr, mProc1XmlStr, mProc2XmlStr;
  std::string mDaqTTCXmlStr, mLinkXmlStr, mFEDXmlStr;
  std::string mSystemXmlStr;

  pugi::xml_document mCrateXml, mProc1Xml, mProc2Xml;
  pugi::xml_document mDaqTTCXml, mLinkXml, mFEDXml;
  pugi::xml_document mSystemXml;
};

BOOST_AUTO_TEST_SUITE( TestXmlSystem )

BOOST_AUTO_TEST_CASE ( TestInvalid ) {
  //TODO: set logger to ignore ERRORS
  pugi::xml_document lInvalid;
  lInvalid.load("<system></system>");
  BOOST_REQUIRE_EQUAL(detail::validateSystemXml(lInvalid), false);
  BOOST_CHECK_THROW(xmlToSystemStub(lInvalid), InvalidSystemDescription);
}

BOOST_FIXTURE_TEST_CASE ( TestSystem, TestXmlSystemSetup ) {
  BOOST_REQUIRE_EQUAL(detail::validateSystemXml(mSystemXml), true);
  swatch::system::SystemStub lStub = xmlToSystemStub(mSystemXml);
  BOOST_REQUIRE_EQUAL(lStub.id, "calol2");
  BOOST_REQUIRE_EQUAL(lStub.creator, "calol2::System");

  for (unsigned int i = 0; i < lStub.processors.size(); ++i) {
    BOOST_REQUIRE_EQUAL(lStub.processors.at(i).loggerName, lStub.id + "." + lStub.processors.at(i).id);
  }
}

BOOST_FIXTURE_TEST_CASE ( TestCrate, TestXmlSystemSetup ) {
  pugi::xml_node lNode = mCrateXml.child("crate");
  swatch::system::CrateStub lStub = detail::xmlToCrateStub(lNode);

  BOOST_REQUIRE_EQUAL(lStub.id, "S2D11-29");
  BOOST_REQUIRE_EQUAL(lStub.location, "Point5, S2D11-29");
  BOOST_REQUIRE_EQUAL(lStub.description, "Calorimeter trigger layer 2 main processor crate");
}

BOOST_FIXTURE_TEST_CASE ( TestProcessor, TestXmlSystemSetup ) {
  pugi::xml_node lNode = mProc1Xml.child("processor");
  swatch::processor::ProcessorStub lStub = detail::xmlToProcessorStub(lNode);

  BOOST_REQUIRE_EQUAL(lStub.id, "MP8");
  BOOST_REQUIRE_EQUAL(lStub.creator, "calol2::MainProcessor");
  BOOST_REQUIRE_EQUAL(lStub.hwtype, "MP7-XE");
  BOOST_REQUIRE_EQUAL(lStub.role, "MainProcessor");
  BOOST_REQUIRE_EQUAL(lStub.crate, "S2D11-29");
  BOOST_REQUIRE_EQUAL(lStub.slot, uint32_t(9));
  BOOST_REQUIRE_EQUAL(lStub.uri, "chtcp-2.0://ctrl-calol2:10203?target=amc-s2d11-29-09:50001");
  // system id + . + processor id is only valid when reading the whole system
  // otherwise it is just the processor id
  BOOST_REQUIRE_EQUAL(lStub.loggerName, lStub.id);

  BOOST_REQUIRE_EQUAL(lStub.txPorts.size(), size_t(6));
  BOOST_REQUIRE_EQUAL(lStub.rxPorts.size(), size_t(72));

  lNode = mProc2Xml.child("processor");
  lStub = detail::xmlToProcessorStub(lNode);
  BOOST_REQUIRE_EQUAL(lStub.txPorts.size(), size_t(7));
  BOOST_REQUIRE_EQUAL(lStub.rxPorts.size(), size_t(72));
}

BOOST_FIXTURE_TEST_CASE ( TestDaqTTC, TestXmlSystemSetup ) {
  pugi::xml_node lNode = mDaqTTCXml.child("daqttc-mgr");
  swatch::dtm::DaqTTCStub lStub = detail::xmlToDaqTTCStub(lNode);

  BOOST_REQUIRE_EQUAL(lStub.id, "AMC13");
  BOOST_REQUIRE_EQUAL(lStub.role, "daqttc");
  BOOST_REQUIRE_EQUAL(lStub.crate, "S2D11-29");
  BOOST_REQUIRE_EQUAL(lStub.slot, uint32_t(13));
  BOOST_REQUIRE_EQUAL(lStub.creator, "swatch::amc13::AMC13Manager");
  BOOST_REQUIRE_EQUAL(lStub.fedId, uint16_t(1360));

  BOOST_REQUIRE_EQUAL(lStub.uriT1, "chtcp-2.0://ctrl-calol2:10203?target=amc-s2d11-29-13-t1:50001");
  BOOST_REQUIRE_EQUAL(lStub.uriT2, "chtcp-2.0://ctrl-calol2:10203?target=amc-s2d11-29-13-t2:50001");
  BOOST_REQUIRE_EQUAL(lStub.addressTableT1, "file:///opt/cactus/etc/amc13/AMC13XG_T1.xml");
  BOOST_REQUIRE_EQUAL(lStub.addressTableT2, "file:///opt/cactus/etc/amc13/AMC13XG_T2.xml");
  // system id + . + processor id is only valid when reading the whole system
  // otherwise it is just the processor id
  BOOST_REQUIRE_EQUAL(lStub.loggerName, lStub.id);
}

BOOST_FIXTURE_TEST_CASE ( TestLinks, TestXmlSystemSetup ) {
  pugi::xml_node lNode = mLinkXml.child("link");
  std::vector<swatch::system::LinkStub> lStubs(detail::xmlToLinkStubs(lNode));
  BOOST_REQUIRE_EQUAL(lStubs.size(), size_t(6));
}

BOOST_FIXTURE_TEST_CASE ( TestConnectedFeds, TestXmlSystemSetup ) {
  pugi::xml_node lNode = mFEDXml.child("connected-fed");
  swatch::system::SystemStub::FEDInputPortsMap lFedMap;
  detail::xmlToConnectedFeds(lNode, lFedMap);

  BOOST_REQUIRE_EQUAL(lFedMap.count(1356), size_t(1));
  BOOST_REQUIRE_EQUAL(lFedMap[1356].size(), size_t(10+14));
}

BOOST_AUTO_TEST_SUITE_END() // TestXmlSystem

}//ns: test
}//ns: system
} //ns: xml
} //ns: swatch
