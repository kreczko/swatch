#ifndef __SWATCH_XML_SINGLEOBJECTSERIALIZER_HXX__
#define __SWATCH_XML_SINGLEOBJECTSERIALIZER_HXX__

// Standard headers

// boost headers

// SWATCH headers
#include "swatch/xml/SingleObjectSerializer.hpp"
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
SingleObjectSerializer<T>::SingleObjectSerializer() {
}

template<class T> SingleObjectSerializer<T>::~SingleObjectSerializer() {

}

//template<class T> const xdata::Serializable* SingleObjectSerializer<T>::import(const pugi::xml_node& aNode) {
template<class T> xdata::Serializable* SingleObjectSerializer<T>::import(const pugi::xml_node& aNode) {
	xdata::Serializable* lSerializable(new T());

	std::string lValue(aNode.child_value());
	lSerializable->fromString(lValue);

	return lSerializable;
}

template<class T>
const xdata::Serializable* SingleObjectSerializer<T>::const_import(const pugi::xml_node& aNode){
	return const_cast<xdata::Serializable*>(import(aNode));
}

template<class T>
std::string SingleObjectSerializer<T>::type() const {
	T tmp;
	return tmp.type();
}
} // namespace xml
} // namespace swatch

#endif	/* __SWATCH_XML_SINGLEOBJECTSERIALIZER_HXX__ */

