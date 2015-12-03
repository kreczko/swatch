#ifndef __SWATCH_XML_XMLSERIALIZER_HPP__
#define __SWATCH_XML_XMLSERIALIZER_HPP__

// Standard headers
#include "pugixml/pugixml.hpp"

// boost headers

//xdata
#include "xdata/Serializer.h"
#include "xdata/Serializable.h"

// SWATCH headers

namespace swatch {
namespace xml {

class XmlSerializer: public xdata::Serializer {
public:
	XmlSerializer();
	virtual ~XmlSerializer();
	// TODO change to const after XDAQ 13 migration ?
//	const xdata::Serializable* import(const pugi::xml_node& aNode);
	xdata::Serializable* import(const pugi::xml_node& aNode);
};

} // namespace xml
} // namespace swatch

#endif	/* __SWATCH_XML_XMLSERIALIZER_HPP__ */

