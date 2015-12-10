#ifndef __SWATCH_XML_VECTORSERIALIZER_HXX__
#define __SWATCH_XML_VECTORSERIALIZER_HXX__

// Standard headers

// boost headers
#include <boost/algorithm/string.hpp>
// SWATCH headers
#include "swatch/xml/VectorSerializer.hpp"
#include "swatch/core/exception.hpp"
// xdata
#include <xdata/Integer.h>
#include <xdata/UnsignedInteger.h>
#include <xdata/String.h>
#include <xdata/Boolean.h>
#include <xdata/Float.h>

namespace swatch {
namespace xml {

template<class T>
VectorSerializer<T>::VectorSerializer() {
}

template<class T> VectorSerializer<T>::~VectorSerializer() {

}

//template<class T> const xdata::Serializable* VectorSerializer<T>::import(const pugi::xml_node& aNode) {
template<class T> xdata::Serializable* VectorSerializer<T>::import(const pugi::xml_node& aNode) {
	std::string lType(aNode.attribute("type").value());
	std::string lValue(aNode.child_value());
	pugi::xml_attribute lDelimiterAttr(aNode.attribute("delimiter"));
	std::string lDelimiter(lDelimiterAttr.empty() ? "," : lDelimiterAttr.value());
	xdata::Vector<T> lVector = convertString(lValue, lDelimiter);
	xdata::Serializable* lSerializable(new xdata::Vector<T>());
	lSerializable->setValue(lVector);

	return lSerializable;
}

template<class T>
const xdata::Serializable* VectorSerializer<T>::const_import(const pugi::xml_node& aNode) {
	return const_cast<xdata::Serializable*>(import(aNode));
}

template<class T>
xdata::Vector<T> VectorSerializer<T>::convertString(const std::string& aStr, const std::string& aDeliminter) const {
	// split string by ","
	// remove spaces
	xdata::Vector<T> lVector;
	std::vector<std::string> lStrings;
	boost::split(lStrings, aStr, boost::is_any_of(aDeliminter));
	for (std::vector<std::string>::iterator it = lStrings.begin(); it != lStrings.end(); ++it) {
		std::string lTmp = boost::trim_copy(*it);
		T lSerializable;
		lSerializable.fromString(lTmp);
		lVector.push_back(lSerializable);
	}
	return lVector;
}

template<class T>
std::string VectorSerializer<T>::type() const {
	T tmp;
	return "vector:" + tmp.type();
}
} // namespace xml
} // namespace swatch

#endif	/* __SWATCH_XML_VECTORSERIALIZER_HXX__ */

