// Boost Unit Test includes
#include <boost/test/unit_test.hpp>
// swatch headers
#include "swatch/logger/Log.hpp"
#include "swatch/xml/SingleObjectSerializer.hpp"
// xdaq
#include <xdata/Integer.h>
#include <xdata/UnsignedInteger.h>
#include <xdata/String.h>
#include <xdata/Boolean.h>
#include <xdata/Float.h>
//others
#include "pugixml/pugixml.hpp"

using namespace swatch::logger;

namespace swatch {
namespace xml {
namespace test {


BOOST_AUTO_TEST_SUITE( TestSingleObjectSerializer )
// Test all supported types
BOOST_AUTO_TEST_CASE( TestUInt ) {
	LOG(kInfo) << "Running TestSingleObjectSerializer/TestUInt";
	pugi::xml_document doc;
	doc.load("<entry id='clkErrorTimeout' type='uint'>40</entry>");
	pugi::xml_node lNode = doc.child("entry");
	SingleObjectSerializer<xdata::UnsignedInteger>* serializer = new SingleObjectSerializer<xdata::UnsignedInteger>;
	BOOST_CHECK_EQUAL(serializer->type(), "unsigned int");

	xdata::Serializable* result = serializer->import(lNode);
	BOOST_CHECK_EQUAL(result->type(), "unsigned int");
	BOOST_CHECK_EQUAL(result->toString(), "40");
}

BOOST_AUTO_TEST_CASE( TestInt ) {
	LOG(kInfo) << "Running TestSingleObjectSerializer/TestInt";
	pugi::xml_document doc;
	doc.load("<entry id='testInt' type='int'>-2</entry>");
	pugi::xml_node lNode = doc.child("entry");
	SingleObjectSerializer<xdata::Integer>* serializer = new SingleObjectSerializer<xdata::Integer>;
	BOOST_CHECK_EQUAL(serializer->type(), "int");

	xdata::Serializable* result = serializer->import(lNode);
	BOOST_CHECK_EQUAL(result->type(), "int");
	BOOST_CHECK_EQUAL(result->toString(), "-2");
}

BOOST_AUTO_TEST_CASE( TestBool ) {
	LOG(kInfo) << "Running TestSingleObjectSerializer/TestBool";
	pugi::xml_document doc;
	doc.load("<entry id='testInt' type='bool'>true</entry>");
	pugi::xml_node lNode = doc.child("entry");
	SingleObjectSerializer<xdata::Boolean>* serializer = new SingleObjectSerializer<xdata::Boolean>;
	BOOST_CHECK_EQUAL(serializer->type(), "bool");

	xdata::Serializable* result = serializer->import(lNode);
	BOOST_CHECK_EQUAL(result->type(), "bool");
	BOOST_CHECK_EQUAL(result->toString(), "true");
}

BOOST_AUTO_TEST_CASE( TestFloat ) {
	LOG(kInfo) << "Running TestSingleObjectSerializer/TestFloat";
	pugi::xml_document doc;
	doc.load("<entry id='testInt' type='float'>2.0</entry>");
	pugi::xml_node lNode = doc.child("entry");
	SingleObjectSerializer<xdata::Float>* serializer = new SingleObjectSerializer<xdata::Float>;
	BOOST_CHECK_EQUAL(serializer->type(), "float");

	xdata::Serializable* result = serializer->import(lNode);
	BOOST_CHECK_EQUAL(result->type(), "float");
	// xdata::Float will change the string to a more generic format
	BOOST_CHECK_EQUAL(result->toString(), "2.00000e+00");

	// further tests to understand xdata::Float
	xdata::Serializable* test2 = new xdata::Float();
	test2->fromString(result->toString());
	BOOST_ASSERT(result->equals(*test2));
	BOOST_CHECK_EQUAL(result->toString(), test2->toString());
}

BOOST_AUTO_TEST_CASE( TestString ) {
	LOG(kInfo) << "Running TestSingleObjectSerializer/TestString";
	pugi::xml_document doc;
	doc.load("<entry id='testInt' type='string'>hello</entry>");
	pugi::xml_node lNode = doc.child("entry");
	SingleObjectSerializer<xdata::String>* serializer = new SingleObjectSerializer<xdata::String>;
	BOOST_CHECK_EQUAL(serializer->type(), "string");

	xdata::Serializable* result = serializer->import(lNode);
	BOOST_CHECK_EQUAL(result->type(), "string");
	BOOST_CHECK_EQUAL(result->toString(), "hello");
}


BOOST_AUTO_TEST_SUITE_END() // TestSingleObjectSerializer

} //ns: test
}//ns: xml
} //ns: swatch