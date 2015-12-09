#ifndef __SWATCH_XML_XMLSERIALIZER_HPP__
#define __SWATCH_XML_XMLSERIALIZER_HPP__

// Standard headers
#include "pugixml/pugixml.hpp"
// boost headers

//xdata
#include "xdata/Serializer.h"
#include "xdata/Serializable.h"

// SWATCH headers
#include "swatch/logger/Logger.hpp"
#include "swatch/core/exception.hpp"

namespace swatch {
namespace xml {

/**
 * Serializer class for converting XML elements into xdata::Serializable
 */
class XmlSerializer: public xdata::Serializer {
public:
	XmlSerializer();
	virtual ~XmlSerializer();
	// TODO change to const after XDAQ 13 migration ?
//	const xdata::Serializable* import(const pugi::xml_node& aNode);
	xdata::Serializable* import(const pugi::xml_node& aNode);

private:
	/**
	 * Normalises discrapancies between xdata type definitions and (shorter) definitions
	 * in the XML configuration files
	 */
	const std::string normaliseType(const std::string& aType) const;
	log4cplus::Logger mLogger;
};

DEFINE_SWATCH_EXCEPTION(UnknownDataType);

} // namespace xml
} // namespace swatch

#endif	/* __SWATCH_XML_XMLSERIALIZER_HPP__ */

