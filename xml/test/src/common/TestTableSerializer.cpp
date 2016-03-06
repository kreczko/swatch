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
        "   <types>uint,uint,uint</types>"
        "   <rows>"
        "      <row>1,2,4</row>"
        "      <row>5,6,7</row>"
        "   </rows>"
        "</param>");
	pugi::xml_node lNode = doc.child("param");
	TableSerializer* serializer = new TableSerializer;
	BOOST_CHECK_EQUAL(serializer->type(), "table");

    xdata::Serializable* result = serializer->import(lNode);
    
    std::cout << result->toString() << std::endl;

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
    
    std::cout << result->toString() << std::endl;

}

    
BOOST_AUTO_TEST_SUITE_END() // TestTableSerializer

} // namespace swatch
} // namespace xml
} // namespace test
