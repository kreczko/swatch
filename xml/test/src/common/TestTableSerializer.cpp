// Boost Unit Test includes
#include <boost/test/unit_test.hpp>
// swatch headers
#include "swatch/logger/Log.hpp"
#include <xdata/Table.h>
#include "swatch/xml/TableSerializer.hpp"
//others
#include "pugixml/pugixml.hpp"

using namespace swatch::logger;

namespace swatch {
namespace xml {
namespace test {

BOOST_AUTO_TEST_SUITE( TestTableSerializer )

BOOST_AUTO_TEST_CASE( TestTable )
{
	pugi::xml_document doc;
	doc.load(
        "<param id='test' type='table'>"
        "   <columns>aaa,bbb,ccc</columns>"
        "   <types>int,uint,float</types>"
        "   <rows>"
        "      <row>1,2,4</row>"
        "      <row>5,6,7</row>"
        "   </rows>"
        "</param>");
	pugi::xml_node lNode = doc.child("param");
	TableSerializer* serializer = new TableSerializer;
	BOOST_CHECK_EQUAL(serializer->type(), "table");

    xdata::Table* result = dynamic_cast<xdata::Table*>(serializer->import(lNode));
    // Check columns
    std::vector<std::string> expColumns = {"aaa","bbb","ccc"};
    std::vector<std::string> columns = result->getColumns();

    BOOST_CHECK_EQUAL_COLLECTIONS(columns.begin(),columns.end(),expColumns.begin(),expColumns.end());
    std::string expected="{\"rows\":2,\"cols\":3,\"definition\":[[\"aaa\",\"int\"],[\"bbb\",\"unsigned int\"],[\"ccc\",\"float\"]],\"data\":[[1,2,4.00000e+00],[5,6,7.00000e+00]]}";

    BOOST_CHECK_EQUAL(result->toString(), expected);

}

BOOST_AUTO_TEST_CASE( TestTableDelimiter )
{
    pugi::xml_document doc;
    doc.load(
        "<param id='test' type='table' delimiter='|'>"
        "   <columns>aaa|bbb|ccc</columns>"
        "   <types>uint|uint|uint</types>"
        "   <rows>"
        "      <row>1|2|4</row>"
        "      <row>5|6|7</row>"
        "   </rows>"
        "</param>");
    pugi::xml_node lNode = doc.child("param");
    TableSerializer* serializer = new TableSerializer;
    BOOST_CHECK_EQUAL(serializer->type(), "table");

    xdata::Serializable* result = serializer->import(lNode);
    
    std::string expected="{\"rows\":2,\"cols\":3,\"definition\":[[\"aaa\",\"unsigned int\"],[\"bbb\",\"unsigned int\"],[\"ccc\",\"unsigned int\"]],\"data\":[[1,2,4],[5,6,7]]}";
    
	BOOST_CHECK_EQUAL(result->toString(), expected);

}

BOOST_AUTO_TEST_CASE( TestIntruderTag )
{
    pugi::xml_document doc;
    doc.load(
        "<param id='test' type='table'>"
        "   <columns>aaa,bbb,ccc</columns>"
        "   <rows>"
        "      <row>1,2,4</row>"
        "      <not_a_row>5,6,7</not_a_row>"
        "   </rows>"
        "</param>");
    pugi::xml_node lNode = doc.child("param");
    TableSerializer* serializer = new TableSerializer;

    BOOST_CHECK_THROW(serializer->import(lNode), swatch::xml::ValueError);
    
}


BOOST_AUTO_TEST_CASE( TestMissingTag )
{
    pugi::xml_document doc;
    doc.load(
        "<param id='test' type='table'>"
        "   <columns>aaa,bbb,ccc</columns>"
        "   <rows>"
        "      <row>1,2,4</row>"
        "      <row>5,6,7</row>"
        "   </rows>"
        "</param>");
    pugi::xml_node lNode = doc.child("param");
    TableSerializer* serializer = new TableSerializer;

    BOOST_CHECK_THROW(serializer->import(lNode), swatch::xml::ValueError);
    
}

BOOST_AUTO_TEST_CASE( TestColRowMismatch )
{
    pugi::xml_document doc;
    doc.load(
        "<param id='test' type='table'>"
        "   <columns>aaa,bbb,ccc</columns>"
        "   <rows>"
        "      <row>1,2</row>"
        "      <row>5,6,7</row>"
        "   </rows>"
        "</param>");
    pugi::xml_node lNode = doc.child("param");
    TableSerializer* serializer = new TableSerializer;

    BOOST_CHECK_THROW(serializer->import(lNode), swatch::xml::ValueError);
    
}

BOOST_AUTO_TEST_SUITE_END() // TestTableSerializer

} // namespace swatch
} // namespace xml
} // namespace test
