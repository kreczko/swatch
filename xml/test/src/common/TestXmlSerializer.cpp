// Boost Unit Test includes
#include <boost/test/unit_test.hpp>
// swatch headers
#include "swatch/logger/Log.hpp"
#include "swatch/xml/XmlSerializer.hpp"
#include "swatch/xml/VectorSerializer.hpp"
// xdaq
#include <xdata/Integer.h>
#include <xdata/UnsignedInteger.h>
#include <xdata/String.h>
#include <xdata/Boolean.h>
#include <xdata/Float.h>
#include <xdata/Vector.h>
//others
#include "pugixml/pugixml.hpp"

using namespace swatch::logger;

namespace swatch {
namespace xml {
namespace test {

struct XmlSerializerTestSetup {

	XmlSerializer* mSerializer;
	std::vector<std::string> mInputs, mOutputs, mTypes;

	XmlSerializerTestSetup() :
			mSerializer(new XmlSerializer()), mInputs(), mOutputs(), mTypes() {
		mInputs = {
			// vectors
			"<entry id='testUintV' type='vector:uint'>40, 30, 1, 2</entry>",
			"<entry id='testIntV' type='vector:int'>-2, 2, -42</entry>",
			"<entry id='testFloatV' type='vector:float'>-2.2, 2.3, -42.0</entry>",
			"<entry id='testBoolV' type='vector:bool'>false, false, true</entry>",
			"<entry id='testStringV' type='vector:string'>hello, is, it, me, you're, looking, for</entry>",
			// single objects
			"<entry id='testUint' type='uint'>40</entry>",
			"<entry id='testInt' type='int'>-2</entry>",
			"<entry id='testFloat' type='float'>-2.2</entry>",
			"<entry id='testBool' type='bool'>false</entry>",
			"<entry id='testString' type='string'>hello</entry>"
		};
		mOutputs = {
			"[40,30,1,2]",
			"[-2,2,-42]",
			"[-2.20000e+00,2.30000e+00,-4.20000e+01]",
			"[false,false,true]",
			"[hello,is,it,me,you're,looking,for]",
			"40",
			"-2",
			"-2.20000e+00",
			"false",
			"hello"
		};
		mTypes = {
			"vector",
			"vector",
			"vector",
			"vector",
			"vector",
			"unsigned int",
			"int",
			"float",
			"bool",
			"string"
		};
		BOOST_REQUIRE_EQUAL(mInputs.size(), mOutputs.size());
		BOOST_REQUIRE_EQUAL(mInputs.size(), mTypes.size());
	}
};

BOOST_AUTO_TEST_SUITE( TestXmlSerializer )

BOOST_FIXTURE_TEST_CASE(TestImport, XmlSerializerTestSetup)
{
	for(unsigned int i = 0; i < mInputs.size(); ++i){
		pugi::xml_document doc;
		BOOST_REQUIRE_EQUAL(doc.load(mInputs.at(i).c_str()), true);
		pugi::xml_node lNode = doc.child("entry");
		BOOST_REQUIRE_EQUAL(lNode.empty(), false);
		xdata::Serializable* result = mSerializer->import(lNode);

		BOOST_CHECK_EQUAL(result->type(), mTypes.at(i));
		BOOST_CHECK_EQUAL(result->toString(), mOutputs.at(i));
	}
}

BOOST_FIXTURE_TEST_CASE(TestInvalid, XmlSerializerTestSetup)
{
	std::string lInput = "<entry id='test' type='vector:uint'>40, error, 1, 2</entry>";
	pugi::xml_document doc;
	BOOST_REQUIRE_EQUAL(doc.load(lInput.c_str()), true);
	pugi::xml_node lNode = doc.child("entry");

	BOOST_CHECK_THROW(mSerializer->import(lNode), swatch::xml::ValueError);
}

BOOST_FIXTURE_TEST_CASE(TestUnkownType, XmlSerializerTestSetup)
{
	std::string lInput = "<entry id='test' type='unknown'>I can't do that Bob.</entry>";
	pugi::xml_document doc;
	BOOST_REQUIRE_EQUAL(doc.load(lInput.c_str()), true);
	pugi::xml_node lNode = doc.child("entry");

	BOOST_CHECK_THROW(mSerializer->import(lNode), swatch::xml::UnknownDataType);
}

BOOST_AUTO_TEST_SUITE_END() // TestXmlSerializer

}//ns: test
}//ns: xml
} //ns: swatch
