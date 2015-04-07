// Boost Unit Test includes
#include <boost/test/unit_test.hpp>

// Swatch Headers
#include "swatch/core/XPointer.hpp"

// XDAQ Headers
#include "xdata/String.h"

BOOST_AUTO_TEST_SUITE( XPtrTestSuite )

BOOST_AUTO_TEST_CASE( SimpleInterfaceWithStr ) {
  
    using namespace swatch::core;

    XPointer<xdata::String> pStrA, pStrB;

    // Basic properties
    BOOST_CHECK_EQUAL(pStrA.type(), "string*");
    BOOST_CHECK_EQUAL(pStrA.toString(), "");

    // Setter
    pStrA.fromString("abcd");
    BOOST_CHECK_EQUAL(pStrA.toString(), "abcd");

    pStrA->setValue(xdata::String("efgh"));
    BOOST_CHECK_EQUAL(pStrA.toString(), "efgh");

    // Equal operator
    BOOST_CHECK_EQUAL(pStrA.equals(pStrA), 1);
    BOOST_CHECK_EQUAL((*pStrA).equals(*pStrA), 1);
    BOOST_CHECK_EQUAL(pStrA->equals(*pStrA), 1);
}

BOOST_AUTO_TEST_CASE( PointerLikeBehaviour ) {

    using namespace swatch::core;

    XPointer<xdata::String> pStrA, pStrB, pStrC;

    // A and C are set independently to the same value
    pStrA.fromString("abcd");
    pStrC.fromString("abcd");

    // B is copied from A
    BOOST_CHECK_EQUAL(pStrA.count(), 1);
    pStrB = pStrA;

    // So the shared pointer count increases
    BOOST_CHECK_EQUAL(pStrA.count(), 2);

    // And and A and B are the same
    BOOST_CHECK_EQUAL(pStrA.equals(pStrB), 1);
    BOOST_CHECK_EQUAL(pStrA == pStrB, 1);

    // But A and C are not the same, as pointers
    BOOST_CHECK_EQUAL(pStrA.equals(pStrC), 0);
    BOOST_CHECK_EQUAL(pStrA == pStrC, 0);

    // Even if the payload is not the same
    BOOST_CHECK_EQUAL(*pStrA == *pStrC, 1);

}

/*
BOOST_AUTO_TEST_CASE(AddAdoptSetGetTest) {

  // Table
  swco::XPointer<xdata::Table> pTable;
  xdata::Table& t = *pTable;

  xdata::String xstr;
  xdata::Integer xint;

  t.addColumn("a","string");
  t.addColumn("b","int");

  xstr = "xyz";
  t.setValueAt(0,"a", xstr);
  xint = 15;
  t.setValueAt(1,"b", xint);
  
  LOG(swlo::kDebug) << "pTable | type: " << pTable.type();
  LOG(swlo::kDebug) << "pTable | toString: " << pTable.toString();
  
  // Put the Table in a parameter set
  swco::XParameterSet ps, ps2;
  ps.add("s1", pStr);
  ps.add("t1", pTable);
  LOG(swlo::kDebug) << "pTable | toString: " << ps.toString();

  LOG(swlo::kDebug) << "pTable | counts " << pTable.count();

  ps2.add("s2", pStr);
  ps2.add("t2", pTable);
  
  LOG(swlo::kDebug) << "pSet | x-check: " << ps2["s2"].equals(ps["s1"]);
  LOG(swlo::kDebug) << "pSet | Counts " << pTable.count();

  // Compare pointers inside xpointers, extracted from parametersets
  LOG(swlo::kDebug) << "pSet | (&ps[\"s1\"] == &ps2[\"s2\"]): " 
      << ( ps.get< swco::XPointer<xdata::String> >("s1").operator->() == ps2.get< swco::XPointer<xdata::String> >("s2").operator->() );

  return 0;
}
 */

BOOST_AUTO_TEST_SUITE_END() // XPtrTestSuite
