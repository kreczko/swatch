#ifndef __SWATCH_XML_OBJECTSERIALIZER_HPP__
#define __SWATCH_XML_OBJECTSERIALIZER_HPP__

// Standard headers

// boost headers

//xdata
#include "xdata/Serializable.h"
#include "xdata/ObjectSerializer.h"
// SWATCH headers

// others
#include "pugixml/pugixml.hpp"

namespace swatch {
namespace xml {

/**
 * Abstract base XML serializer for xdata serializables
 */
class ObjectSerializer : public xdata::ObjectSerializer {
public:

	virtual ~ObjectSerializer(){
		// TODO: move this into hxx which currently does not work for some bizzare reason (multiple definitions)
	}

	//TODO: const only possible once we switch to XDAQ 13 (where Serializable::toString() finally becomes const)
//	const xdata::Serializable* import(const pugi::xml_node& aNode);
	// for now do these two
	/**
	 * Converts an XML element into a xdata::Serializable
	 */
	virtual xdata::Serializable* import(const pugi::xml_node& aNode) = 0;
	/**
	 * Same as ObjectSerializer::import but with dynamic cast to const
	 */
	virtual const xdata::Serializable* const_import(const pugi::xml_node& aNode) = 0;


};

} // namespace xml
} // namespace swatch

#include "swatch/xml/ObjectSerializer.hxx"

#endif	/* __SWATCH_XML_OBJECTSERIALIZER_HPP__ */

