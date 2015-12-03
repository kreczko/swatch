// Boost Unit Test includes
#include <boost/test/unit_test.hpp>
// swatch headers
#include "swatch/logger/Log.hpp"
#include "swatch/xml/VectorSerializer.hpp"
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

BOOST_AUTO_TEST_SUITE( TestVectorSerializer )

BOOST_AUTO_TEST_CASE( TestUInt ) {
	LOG(kInfo) << "Running TestVectorSerializer/TestUInt";
	pugi::xml_document doc;
	doc.load("<entry id='test' type='vector:uint'>40, 30, 1, 2</entry>");
	pugi::xml_node lNode = doc.child("entry");
	VectorSerializer<xdata::UnsignedInteger>* serializer = new VectorSerializer<xdata::UnsignedInteger>;

	xdata::Serializable* result = serializer->import(lNode);
	BOOST_CHECK_EQUAL(result->type(), "vector");
	BOOST_CHECK_EQUAL(((xdata::Vector<xdata::UnsignedInteger>*)result)->getElementType(), "unsigned int");

	BOOST_CHECK_EQUAL(result->toString(), "[40,30,1,2]");
}

BOOST_AUTO_TEST_CASE( TestInt ) {
	LOG(kInfo) << "Running TestVectorSerializer/TestInt";
	pugi::xml_document doc;
	doc.load("<entry id='test' type='vector:int'>-2</entry>");
	pugi::xml_node lNode = doc.child("entry");
	VectorSerializer<xdata::Integer>* serializer = new VectorSerializer<xdata::Integer>;

	xdata::Serializable* result = serializer->import(lNode);
	BOOST_CHECK_EQUAL(result->type(), "vector");
	BOOST_CHECK_EQUAL(((xdata::Vector<xdata::Integer>*)result)->getElementType(), "int");
	BOOST_CHECK_EQUAL(result->toString(), "[-2]");
}

BOOST_AUTO_TEST_CASE( TestBool ) {
	LOG(kInfo) << "Running TestVectorSerializer/TestBool";
	pugi::xml_document doc;
	doc.load("<entry id='test' type='vector:bool'>true,false</entry>");
	pugi::xml_node lNode = doc.child("entry");
	VectorSerializer<xdata::Boolean>* serializer = new VectorSerializer<xdata::Boolean>;

	xdata::Serializable* result = serializer->import(lNode);
	BOOST_CHECK_EQUAL(result->type(), "vector");
	BOOST_CHECK_EQUAL(((xdata::Vector<xdata::Boolean>*)result)->getElementType(), "bool");
	BOOST_CHECK_EQUAL(result->toString(), "[true,false]");
}

BOOST_AUTO_TEST_CASE( TestFloat ) {
	LOG(kInfo) << "Running TestVectorSerializer/TestFloat";
	pugi::xml_document doc;
	doc.load("<entry id='test' type='vector:float'>2.0</entry>");
	pugi::xml_node lNode = doc.child("entry");
	VectorSerializer<xdata::Float>* serializer = new VectorSerializer<xdata::Float>;

	xdata::Serializable* result = serializer->import(lNode);
	BOOST_CHECK_EQUAL(result->type(), "vector");
	BOOST_CHECK_EQUAL(((xdata::Vector<xdata::Float>*)result)->getElementType(), "float");
	BOOST_CHECK_EQUAL(result->toString(), "[2.00000e+00]");
}

BOOST_AUTO_TEST_CASE( TestString ) {
	LOG(kInfo) << "Running TestVectorSerializer/TestString";
	pugi::xml_document doc;
	doc.load("<entry id='test' type='vector:string'>hello, world, and, all</entry>");
	pugi::xml_node lNode = doc.child("entry");
	VectorSerializer<xdata::String>* serializer = new VectorSerializer<xdata::String>;

	xdata::Serializable* result = serializer->import(lNode);
	BOOST_CHECK_EQUAL(result->type(), "vector");
	BOOST_CHECK_EQUAL(((xdata::Vector<xdata::String>*)result)->getElementType(), "string");
	BOOST_CHECK_EQUAL(result->toString(), "[hello,world,and,all]");
}

BOOST_AUTO_TEST_CASE( TestCustomDelimiter ) {
	LOG(kInfo) << "Running TestVectorSerializer/TestString";
	pugi::xml_document doc;
	doc.load("<entry id='test' type='vector:string' delimiter='|'>hello, world| and, all</entry>");
	pugi::xml_node lNode = doc.child("entry");
	VectorSerializer<xdata::String>* serializer = new VectorSerializer<xdata::String>;

	xdata::Serializable* result = serializer->import(lNode);
	BOOST_CHECK_EQUAL(result->type(), "vector");
	BOOST_CHECK_EQUAL(((xdata::Vector<xdata::String>*)result)->getElementType(), "string");
	BOOST_CHECK_EQUAL(result->toString(), "[hello, world,and, all]");
}


BOOST_AUTO_TEST_SUITE_END() // TestVectorSerializer

} //ns: test
}//ns: xml
} //ns: swatch
