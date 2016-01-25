// Boost Unit Test includes
#include <boost/test/unit_test.hpp>
// swatch headers
#include "swatch/logger/Log.hpp"
#include <xdata/Integer.h>
#include <xdata/UnsignedInteger.h>
#include <xdata/String.h>
#include <xdata/Boolean.h>
#include <xdata/Float.h>
#include "swatch/xml/SimpleSerializer.hpp"
//others
#include "pugixml/pugixml.hpp"

using namespace swatch::logger;

namespace swatch {
namespace xml {
namespace test {


BOOST_AUTO_TEST_SUITE( TestSimpleSerializer )
// Test all supported types
BOOST_AUTO_TEST_CASE( TestUInt )
{
	pugi::xml_document doc;
	doc.load("<param id='clkErrorTimeout' type='uint'>40</param>");
	pugi::xml_node lNode = doc.child("param");
	SimpleSerializer<xdata::UnsignedInteger>* serializer = new SimpleSerializer<xdata::UnsignedInteger>;
	BOOST_CHECK_EQUAL(serializer->type(), "unsigned int");

	xdata::Serializable* result = serializer->import(lNode);
	BOOST_CHECK_EQUAL(result->type(), "unsigned int");
	BOOST_CHECK_EQUAL(result->toString(), "40");
}

BOOST_AUTO_TEST_CASE( TestInt )
{
	pugi::xml_document doc;
	doc.load("<param id='test' type='int'>-2</param>");
	pugi::xml_node lNode = doc.child("param");
	SimpleSerializer<xdata::Integer>* serializer = new SimpleSerializer<xdata::Integer>;
	BOOST_CHECK_EQUAL(serializer->type(), "int");

	xdata::Serializable* result = serializer->import(lNode);
	BOOST_CHECK_EQUAL(result->type(), "int");
	BOOST_CHECK_EQUAL(result->toString(), "-2");
}

BOOST_AUTO_TEST_CASE( TestBool )
{
	pugi::xml_document doc;
	doc.load("<param id='test' type='bool'>true</param>");
	pugi::xml_node lNode = doc.child("param");
	SimpleSerializer<xdata::Boolean>* serializer = new SimpleSerializer<xdata::Boolean>;
	BOOST_CHECK_EQUAL(serializer->type(), "bool");

	xdata::Serializable* result = serializer->import(lNode);
	BOOST_CHECK_EQUAL(result->type(), "bool");
	BOOST_CHECK_EQUAL(result->toString(), "true");
}

BOOST_AUTO_TEST_CASE( TestFloat )
{
	pugi::xml_document doc;
	doc.load("<param id='test' type='float'>2.0</param>");
	pugi::xml_node lNode = doc.child("param");
	SimpleSerializer<xdata::Float>* serializer = new SimpleSerializer<xdata::Float>;
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

BOOST_AUTO_TEST_CASE( TestString )
{
	pugi::xml_document doc;
	doc.load("<param id='test' type='string'>hello</param>");
	pugi::xml_node lNode = doc.child("param");
	SimpleSerializer<xdata::String>* serializer = new SimpleSerializer<xdata::String>;
	BOOST_CHECK_EQUAL(serializer->type(), "string");

	xdata::Serializable* result = serializer->import(lNode);
	BOOST_CHECK_EQUAL(result->type(), "string");
	BOOST_CHECK_EQUAL(result->toString(), "hello");
}

BOOST_AUTO_TEST_CASE( TestInvalid )
{
	pugi::xml_document doc;
	doc.load("<param id='test' type='string'>hello</param>");
	pugi::xml_node lNode = doc.child("param");
	SimpleSerializer<xdata::Integer>* serializer = new SimpleSerializer<xdata::Integer>;
	BOOST_CHECK_EQUAL(serializer->type(), "int");

	BOOST_CHECK_THROW(serializer->import(lNode), swatch::xml::ValueError);
}


BOOST_AUTO_TEST_SUITE_END() // TestSimpleSerializer

} //ns: test
}//ns: xml
} //ns: swatch
